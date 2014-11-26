/*
Raspberry Pi SHT1x communication library.
By:      John Burns (www.john.geek.nz)
Date:    01 November 2012
License: CC BY-SA v3.0 - http://creativecommons.org/licenses/by-sa/3.0/

Adapted by Tim Hagemann (tim@way2.net) in Sep'14 to support multiple sensors

This is a derivative work based on
	Name: Nice Guy SHT11 library
	By: Daesung Kim
	Date: 04/04/2011
	Source: http://www.theniceguy.net/2722
	License: Unknown - Attempts have been made to contact the author
*/

////////////////////////////////////////////////////////////////////////////////////////

#ifndef RPI_SHT1x_H_
#define	RPI_SHT1x_H_

////////////////////////////////////////////////////////////////////////////////////////

#include <bcm2835.h>
#include <unistd.h>
#include <stdio.h>

// Defines
#define	TRUE	1
#define	FALSE	0

/* Definitions of all known SHT1x commands */

#define SHT1x_MEAS_T	0x03			// Start measuring of temperature.
#define SHT1x_MEAS_RH	0x05			// Start measuring of humidity.
#define SHT1x_STATUS_R	0x07			// Read status register.
#define SHT1x_STATUS_W	0x06			// Write status register.
#define SHT1x_RESET		0x1E			// Perform a sensor soft reset.

////////////////////////////////////////////////////////////////////////////////////////

/* Enum to select between temperature and humidity measuring */
typedef enum _SHT1xMeasureType {
	SHT1xMeaT	= SHT1x_MEAS_T,		// Temperature
	SHT1xMeaRh	= SHT1x_MEAS_RH		// Humidity
} SHT1xMeasureType;

////////////////////////////////////////////////////////////////////////////////////////

typedef union 
{
	unsigned short int i;
	float f;
} value;

////////////////////////////////////////////////////////////////////////////////////////

class SHT1x
{

public:

	SHT1x(int f_sck,int f_data);

	void SHT1x_Transmission_Start(void);
	unsigned char SHT1x_Readbyte(unsigned char sendAck );
	unsigned char SHT1x_Sendbyte(unsigned char value );
	void SHT1x_InitPins(void);
	unsigned char SHT1x_Measure_Start(SHT1xMeasureType type );
	unsigned char SHT1x_Get_Measure_Value(unsigned short int * value );
	void SHT1x_Reset(void);
	
	static void SHT1x_Calc(float *p_humidity ,float *p_temperature);
	static float SHT1x_CalcDewpoint(float fRH ,float fTemp);
	static float SHT1x_CalcAbsHumidity(float r ,float T);

private:
	unsigned char SHT1x_Mirrorbyte(unsigned char value);
	void SHT1x_Crc_Check(unsigned char value);
	
	int SHT1x_pin_sck;
	int SHT1x_pin_data;
	
	unsigned char SHT1x_crc;
	unsigned char SHT1x_status_reg;
};

////////////////////////////////////////////////////////////////////////////////////////

#endif

