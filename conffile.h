////////////////////////////////////////////////////////////////////////////////////////
//	
//	humifand 	- 	the humidity fan deamon
//
//	Please check www.way2.net for more information
//
//  (c) Copyright 2014 by way2.net Services. 
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef CONFFILE_H
#define	CONFFILE_H

////////////////////////////////////////////////////////////////////////////////////////

#include <map>
#include <string>
#include <algorithm> 
#include <functional> 

////////////////////////////////////////////////////////////////////////////////////////

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

////////////////////////////////////////////////////////////////////////////////////////

#define CONF ConfFile::getConfFile()

////////////////////////////////////////////////////////////////////////////////////////

class ConfFile
{

public:
	
	// --- config file reader
	
	int processConfigFile(const char *f_cfpath);
	
	// --- command line accessors
	
	void coSetVerbose(bool f_d)
	{
	 	m_VerboseMode = f_d;
	}
	
	bool coGetVerbose(void)
	{
		return m_VerboseMode;
	}

	void coSetDaemonMode(bool f_d)
	{
	 	m_DaemonMode = f_d;
	}
	
	bool coGetDaemonMode(void)
	{
		return m_DaemonMode;
	}
	
	
	// --- options accessors
	
	void Clear(void);
	std::string GetOption(const char *f_key);
	int GetOptionInt(const char *f_key);
    int GetOptionIntDefault(const char *f_key,int f_default);    
	float GetOptionFloat(const char *f_key);
    float GetOptionFloatDefault(const char *f_key,float f_default);
	
	bool isValid(const char *f_key);
	
	// --- debugging
	
	void PrintConfMap(void);
	
	// --- singleton accessor
	
    static ConfFile* getConfFile();

private:

    ConfFile();
    ConfFile( const ConfFile&){};
    ConfFile& operator=(const ConfFile& )
    { 
    	return *this;
    };
    
    static ConfFile* m_pThis;

	typedef std::map < std::string, std::string > StringStringMap;
	
	StringStringMap myMap;
	
	bool m_VerboseMode;
	bool m_DaemonMode;
	
};

////////////////////////////////////////////////////////////////////////////////////////

#endif 