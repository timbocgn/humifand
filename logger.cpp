////////////////////////////////////////////////////////////////////////////////////////
//	
//	humifand 	- 	the humidity fan deamon
//
//	Please check www.way2.net for more information
//
//  (c) Copyright 2014 by way2.net Services. 
//
////////////////////////////////////////////////////////////////////////////////////////

#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "logger.h"

////////////////////////////////////////////////////////////////////////////////////////

CLogger* CLogger::m_pThis = NULL;
ofstream CLogger::m_Logfile;

////////////////////////////////////////////////////////////////////////////////////////

CLogger::CLogger()
{
 
}

////////////////////////////////////////////////////////////////////////////////////////

CLogger* CLogger::getLogger()
{
    if (m_pThis == NULL)
    {
        m_pThis = new CLogger();
    }
    
    return m_pThis;
}

////////////////////////////////////////////////////////////////////////////////////////

string CLogger::CurrentDateTime(void)
{
    time_t t = time(0);   
    
    struct tm *now = localtime(&t);
    
    stringstream l_ss;
    
    l_ss << setfill('0') << setw(4) << (now->tm_year + 1900) << '-' 
         << setfill('0') << setw(2) << (now->tm_mon + 1) << '-'
         << setfill('0') << setw(2) << now->tm_mday << ' '
         << setfill('0') << setw(2) << now->tm_hour << ':'
         << setfill('0') << setw(2) << now->tm_min << ':'
         << setfill('0') << setw(2) << now->tm_sec;
        
	return l_ss.str(); 
}

////////////////////////////////////////////////////////////////////////////////////////

bool CLogger::OpenLog(const char *f_filename)
{
    m_Logfile.open(f_filename, ios::out | ios::app );
    
    return m_Logfile.is_open();
}

////////////////////////////////////////////////////////////////////////////////////////

void CLogger::Log( const char * format, ... )
{
    char sMessage[256];
    va_list args;

    va_start (args, format);
    vsprintf (sMessage,format, args);

    m_Logfile << CurrentDateTime() << " ";
    m_Logfile << sMessage;
    m_Logfile << "\n";
    
    m_Logfile.flush();

    va_end (args);
}
 
////////////////////////////////////////////////////////////////////////////////////////

void CLogger::Log( const string& sMessage )
{
    m_Logfile << CurrentDateTime() << " ";
    m_Logfile << sMessage;
    m_Logfile << "\n";

    m_Logfile.flush();

}
 
////////////////////////////////////////////////////////////////////////////////////////

CLogger& CLogger::operator<<(const string& sMessage )
{
    m_Logfile << CurrentDateTime() << " ";
    m_Logfile << sMessage;
    m_Logfile << "\n";

    m_Logfile.flush();

    return *this;
}