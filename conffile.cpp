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
#include <string.h>
#include <cctype>
#include <locale>

#include "conffile.h"

////////////////////////////////////////////////////////////////////////////////////////

#define MAXLINELENGTH 1024

////////////////////////////////////////////////////////////////////////////////////////

const char *g_AllowedOptions[]
{
	"log_file",
	"rrd_file",
	"pid_file",
	"min_inside_temp",
	"min_outside_temp",
	"humi_diff_on",
	"humi_diff_off",
	"min_inside_rh",
	"cycle_time",
	"active_time_1",
	"active_time_2",
	"active_time_3",
	"active_time_4",
	"active_time_5",
	"debug_ot",
	"debug_orh",
	"max_daily_fan_runtime",
	
	NULL
};


////////////////////////////////////////////////////////////////////////////////////////

ConfFile* ConfFile::m_pThis = NULL;

////////////////////////////////////////////////////////////////////////////////////////

ConfFile::ConfFile()
{
	m_VerboseMode = false;
	m_DaemonMode = false;
}

////////////////////////////////////////////////////////////////////////////////////////

ConfFile* ConfFile::getConfFile()
{
    if (m_pThis == NULL)
    {
        m_pThis = new ConfFile();
    }
    
    return m_pThis;
}

////////////////////////////////////////////////////////////////////////////////////////

void ConfFile::Clear(void)
{
	myMap.empty();
}

////////////////////////////////////////////////////////////////////////////////////////

bool ConfFile::isValid(const char *f_key)
{
	return myMap.find(f_key) != myMap.end();
}

////////////////////////////////////////////////////////////////////////////////////////

std::string ConfFile::GetOption(const char *f_key)
{
	return myMap[f_key];
}

////////////////////////////////////////////////////////////////////////////////////////

int ConfFile::GetOptionInt(const char *f_key)
{
	return atoi(myMap[f_key].c_str());
}

////////////////////////////////////////////////////////////////////////////////////////

float ConfFile::GetOptionFloat(const char *f_key)
{
	return atof(myMap[f_key].c_str());
}

////////////////////////////////////////////////////////////////////////////////////////

void ConfFile::PrintConfMap(void)
{
	StringStringMap::iterator myIterator = myMap.begin();
	while (myIterator != myMap.end())
	{
		printf("\t'%s'\t'%s'\n", myIterator->first.c_str(), myIterator->second.c_str());
		myIterator++;
	}
}
  
////////////////////////////////////////////////////////////////////////////////////////

int ConfFile::processConfigFile(const char *f_cfpath)
{
    char input[MAXLINELENGTH];
    FILE *fin;

    if ((fin = fopen(f_cfpath, "r")) == NULL)
    {
        fprintf(stderr, "Unable to open config file!\n");
        return false;
    }

    while (fgets(input, MAXLINELENGTH, fin))
    {
    	std::string l_s(input);
    	
    	trim(l_s);

		// --- skip trivial cases
		    
        if (l_s[0] == '#') 		continue;
        if (l_s.length() < 2) 	continue;
        
		// --- split along the equal sign
        
        std::size_t found = l_s.find("=");
	  	if ( found != std::string::npos)
	  	{
			// --- get left and right value and trim
			
	  		std::string l_left 	= l_s.substr(0,found-1);
	  		std::string l_right = l_s.substr(found+1);
	  		
	  		l_left  = trim(l_left);
	  		l_right = trim(l_right);
	  
	  		// --- check if we find this in the allowed options

        	const char **s = g_AllowedOptions;       
       		while (*s)
       		{
       			if (strcmp(*s,l_left.c_str()) == 0)
       			{
			  		// --- yes: so add in the map and leave the loop
			  		
		            myMap[l_left] = l_right;
		            break;
       			}		
       			
       			s++;
       		}
       		
	  		// --- if we are past the last option, abort

       		if (*s == NULL)
       		{
		        fprintf(stderr, "Unknown option '%s'\n",l_left.c_str());
        		return false;
       		}
		}
		else
		{
	        fprintf(stderr, "Syntax error '%s'\n",input);
    		return false;
		}		
    }

    fclose(fin);

    return true;
}
