////////////////////////////////////////////////////////////////////////////////////////
//	
//	humifand 	- 	the humidity fan deamon
//
//	Please check www.way2.net for more information
//
//  (c) Copyright 2014 by way2.net Services. 
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef CUSTOM_CLogger_H
#define CUSTOM_CLogger_H

#include <fstream>
#include <iostream>
#include <cstdarg>
#include <string>
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////

#define LOGGER CLogger::getLogger()

////////////////////////////////////////////////////////////////////////////////////////

class CLogger
{
public:
	
	bool OpenLog(const std::string &f_filename)
	{
		return OpenLog(f_filename.c_str());
	}
	
	bool OpenLog(const char *f_filename);
    
    void Log(const std::string& sMessage);
    void Log(const char *format, ... );

    CLogger& operator<<(const string& sMessage );

    static CLogger* getLogger();

private:

    CLogger();
    CLogger( const CLogger&){};
    CLogger& operator=(const CLogger& )
    { 
    	return *this;
    };
    
    string CurrentDateTime(void);
    
    static CLogger* m_pThis;
    static ofstream m_Logfile;
};

////////////////////////////////////////////////////////////////////////////////////////

#endif