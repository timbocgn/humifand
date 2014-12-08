////////////////////////////////////////////////////////////////////////////////////////
//	
//	humifand 	- 	the humidity fan deamon
//
//	Please check www.way2.net for more information
//
//  (c) Copyright 2014 by way2.net Services. 
//
////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <bcm2835.h>
#include <time.h>

#include "fan.h"
#include "logger.h"
#include "conffile.h"

////////////////////////////////////////////////////////////////////////////////////////

CFan* CFan::m_pThis = NULL;

////////////////////////////////////////////////////////////////////////////////////////

CFan::CFan()
{
	m_Fan_pin_sck 	= -1; 
	m_FanSpinning	= false;
	
	m_LastStartTime	= 0;
	m_RunTime		= 0;
}

////////////////////////////////////////////////////////////////////////////////////////

CFan* CFan::getFan()
{
    if (m_pThis == NULL)
    {
        m_pThis = new CFan();
    }
    
    return m_pThis;
}

////////////////////////////////////////////////////////////////////////////////////////

void CFan::InitPins(int f_Fan_pin_sck) 
{	
	m_Fan_pin_sck = f_Fan_pin_sck;
	
	// --- config as output, but set low first to avoid starting fans
		
	bcm2835_gpio_write(m_Fan_pin_sck, LOW);
	bcm2835_gpio_fsel(m_Fan_pin_sck, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(m_Fan_pin_sck, LOW);
}

////////////////////////////////////////////////////////////////////////////////////////

double CFan::GetRuntime(void)
{
	if (m_FanSpinning)
    {
        // --- calculate the runtime of this "run"
        
        double l_dt = difftime(time(0),m_LastStartTime);

        // --- and return the total runtine
        
        return m_RunTime+l_dt;
    }
    else
    {
		return m_RunTime;
    }
}

////////////////////////////////////////////////////////////////////////////////////////

void CFan::Start(void)
{
	// --- if already spinning - do nothing
	
	if (m_FanSpinning) return;

	if (CONF->coGetVerbose()) LOGGER->Log("Fan starting up");
	
	// --- remember the time when we started the fan
	
	m_LastStartTime = time(0);
	
	// --- and switch it on
	
	bcm2835_gpio_write(m_Fan_pin_sck, HIGH);
	
	// -- remember the new state
	
	m_FanSpinning = true;
	
}

////////////////////////////////////////////////////////////////////////////////////////

void CFan::Stop(void)
{
	// --- if already stopped - do nothing

	if (!m_FanSpinning) return;

	if (CONF->coGetVerbose()) LOGGER->Log("Fan shutting down");

	// --- and switch it off
	
	bcm2835_gpio_write(m_Fan_pin_sck, LOW);

    // --- set the total runtime to the old runtime + time of the current "run"
    
    double l_oldRT = m_RunTime;
	
    m_RunTime = GetRuntime();
    
    double l_thisRT = m_RunTime-l_oldRT;

	if (CONF->coGetVerbose()) LOGGER->Log("Fan was running for %f seconds, total runtime %f",l_thisRT,m_RunTime);

	// -- remember the new state
	
	m_FanSpinning = false;
}
