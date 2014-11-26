////////////////////////////////////////////////////////////////////////////////////////
//	
//	humifand 	- 	the humidity fan deamon
//
//	Please check www.way2.net for more information
//
//  (c) Copyright 2014 by way2.net Services. 
//
////////////////////////////////////////////////////////////////////////////////////////

#include "ProcessValues.h"
#include "RPi_SHT1x.h"
#include "conffile.h"
#include "logger.h"
#include "fan.h"

#include <rrd.h>
#include <iostream>
#include <iomanip>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////////////

int GetTimeValue(int h,int m)
{
	return h*60 + m;
}

////////////////////////////////////////////////////////////////////////////////////////

int GetTimeValueStr(std::string &f_s)
{
	std::size_t found = f_s.find(":");
	if ( found == std::string::npos) return -1;

	std::string l_sh = f_s.substr(0,found); 	trim(l_sh);
	std::string l_sm = f_s.substr(found+1);		trim(l_sm);
	
	//printf("GetTimeValueStr '%s' '%s' '%s'\n",f_s.c_str(),l_sh.c_str(),l_sm.c_str());

	return GetTimeValue(atoi(l_sh.c_str()),atoi(l_sm.c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////

bool IsWeekdayOk(std::string &f_s,struct tm *f_now)
{
	bool l_ret;
	
	switch (f_now->tm_wday)
	{
		case 0:
				 l_ret = (f_s.find("So") != std::string::npos);
				 break;
		case 1:
				 l_ret = (f_s.find("Mo") != std::string::npos);
				 break;
		case 2:
				 l_ret = (f_s.find("Tu") != std::string::npos);
				 break;
		case 3:
				 l_ret = (f_s.find("We") != std::string::npos);
				 break;
		case 4:
				 l_ret = (f_s.find("Th") != std::string::npos);
				 break;
		case 5:
				 l_ret = (f_s.find("Fr") != std::string::npos);
				 break;
		case 6:
				 l_ret = (f_s.find("Sa") != std::string::npos);
				 break;
	}
	
	return l_ret;
}

////////////////////////////////////////////////////////////////////////////////////////

bool CheckTimeString(void)
{
	char s[50];
	
	// --- get the current time
	
	time_t t = time(0);   
    struct tm *now = localtime(&t);
 
	// --- there are max five values allowed. They have this format:
	//     active_time_1 = 8:00-16:00;Mo-Su
	
	bool l_foundone 	= false;
	bool l_foundoption 	= false;
	
	for (int i=0;i<5;++i)
	{
		// --- create config name
		
		sprintf(s,"active_time_%d",i);
		
		//LOGGER->Log(s);
		
		// --- check if there
		
		if (CONF->isValid(s))
		{
			// --- remember that we found one option
			
			l_foundoption = true;
			
			// --- get the option string 
			
			std::string l_ts = CONF->GetOption(s);

		//LOGGER->Log(l_ts);
			
			std::size_t found_dash = l_ts.find("-");
	  		if ( found_dash != std::string::npos)
	  		{
	  			std::string l_stime;
	  			std::string l_dtime;
	  			std::string l_WeekDays;
	  			
	  			// --- this is the start time
	  			
	  			l_stime = l_ts.substr(0,found_dash);
	 		
	 			// --- check if weekdays supplied or not
	  			
				std::size_t found_semi = l_ts.find(";");
		  		if ( found_semi != std::string::npos)
	  			{
	  				l_dtime 	= l_ts.substr(found_dash+1,found_semi);
	  				l_WeekDays	= l_ts.substr(found_semi+1);
				}	
				else
				{
	  				l_dtime 	= l_ts.substr(found_dash+1);	
				}
				
				
				//LOGGER->Log("start %d",l_start_t);
				//LOGGER->Log("end %d",l_end_t);
				//LOGGER->Log(l_WeekDays);

				// --- analyze if the weekday fits

 				bool l_WeekdayOk = true;
				if (!l_WeekDays.empty()) l_WeekdayOk = IsWeekdayOk(l_WeekDays,now);
				
				//LOGGER->Log("weekday %d",l_WeekdayOk);
				
				if (l_WeekdayOk)
				{
					// --- calculate time values from the strings
				
					int l_start_t 	= GetTimeValueStr(l_stime);
					int l_end_t 	= GetTimeValueStr(l_dtime);
					int l_nowtime_t = GetTimeValue(now->tm_hour,now->tm_min);

				//LOGGER->Log("weekday %d",l_start_t);
				//LOGGER->Log("weekday %d",l_end_t);
				//LOGGER->Log("weekday %d",l_nowtime_t);

				//LOGGER->Log("weekday %d",now->tm_hour);
				//LOGGER->Log("weekday %d",now->tm_min);
					
					// --- is the now time within the range?
					
					if ( (l_nowtime_t >= l_start_t) && (l_nowtime_t <= l_end_t) ) l_foundone = true;
				}
			}
		}
	}	
	
	// --- if there is no allow time option, fan is allowed every time
	
	if (l_foundoption)
		return l_foundone;
	else
		return true;
}

////////////////////////////////////////////////////////////////////////////////////////

bool IsFanAllowed(float f_it,float f_ih, float f_ot, float f_oh)
{
	// --- get some valus from the config file
	
	float l_min_inside_temp 		= CONF->GetOptionFloat("min_inside_temp");
	float l_min_outside_temp 		= CONF->GetOptionFloat("min_outside_temp");
	float l_min_inside_rh 			= CONF->GetOptionFloat("min_inside_rh");
	float l_max_daily_fan_runtime 	= CONF->GetOptionFloat("max_daily_fan_runtime")*60;
	
	// --- l_min_inside_temp
	
	if (f_it <= l_min_inside_temp)
	{
		if (CONF->coGetVerbose()) LOGGER->Log("Fan not allowed as inside temp (%f) below threshold (%f)",f_it,l_min_inside_temp);
		return false;
	}

	// --- l_min_outside_temp
	
	if (f_ot <= l_min_outside_temp)
	{
		if (CONF->coGetVerbose()) LOGGER->Log("Fan not allowed as outside temp (%f) below threshold (%f)",f_ot,l_min_outside_temp);
		return false;
	}

	// --- l_min_inside_rh
	
	if (f_ih <= l_min_inside_rh)
	{
		if (CONF->coGetVerbose()) LOGGER->Log("Fan not allowed as inside rH (%f) below threshold (%f)",f_ih,l_min_inside_rh);
		return false;
	}

	// --- l_max_daily_fan_runtime
	
	if (CONF->isValid("max_daily_fan_runtime"))
	{
		LOGGER->Log("-------------------------------- (%f) : (%f)",l_max_daily_fan_runtime,FAN->GetRuntime());
			
		if (FAN->GetRuntime() > l_max_daily_fan_runtime)
		{
			if (CONF->coGetVerbose()) LOGGER->Log("Fan not allowed as max runtime (%f) : (%f)",l_max_daily_fan_runtime,FAN->GetRuntime());
			return false;
		}
	}
	
	// --- check if the time fits

	if (!CheckTimeString())
	{
		if (CONF->coGetVerbose()) LOGGER->Log("Fan not allowed as current time is not allowed");
		return false;
	}

	// --- if the time fits, we are allowed to switch on!
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////

void UpdateRRD(bool f_iok, float f_it,float f_ih,float f_iah,bool f_ook, float f_ot, float f_oh,float f_oah)
{
	string l_s_it,l_s_ot, l_s_ih, l_s_oh, l_s_idp, l_s_odp, l_s_iah, l_s_oah;
	char s[255];

	// --- calculate internal and external dew point
	
	float l_iDP = SHT1x::SHT1x_CalcDewpoint(f_ih,f_it);
	float l_oDP = SHT1x::SHT1x_CalcDewpoint(f_oh,f_ot);

	// --- replace invalid values with "U"
	
	if (f_iok)
	{
		snprintf(s,30,"%f",f_it); 	l_s_it 	= s;
		snprintf(s,30,"%f",f_ih); 	l_s_ih 	= s;
		snprintf(s,30,"%f",l_iDP); l_s_idp = s;
		snprintf(s,30,"%f",f_iah); l_s_iah = s;
	}
	else
	{
		l_s_it 	= "U";
		l_s_ih 	= "U";
		l_s_idp = "U";
		l_s_iah = "U";
	}

	if (f_ook)
	{
		snprintf(s,30,"%f",f_ot); 	l_s_ot 	= s;
		snprintf(s,30,"%f",f_oh); 	l_s_oh 	= s;
		snprintf(s,30,"%f",l_oDP); l_s_odp = s;
		snprintf(s,30,"%f",f_oah); l_s_oah = s;
	}
	else
	{
		l_s_ot 	= "U";
		l_s_oh 	= "U";
		l_s_odp = "U";
		l_s_oah = "U";
	}

	// --- open the rrd file

	if (CONF->isValid("rrd_file"))
	{
		std::string l_rrdfile = CONF->GetOption("rrd_file");
		
		stringstream l_ss;

		// --- format: temp;temp;rh;rh;ah:ah:dp:dp;fan
				
		l_ss << "N:" << l_s_it << ":" << l_s_ot << ":" << l_s_ih << ":" << l_s_oh << ":"
			 << l_s_iah << ":" << l_s_oah << ":"
			 << l_s_idp << ":" << l_s_odp << ":";
			 
		if (FAN->IsFanSpinning()) 
			 l_ss << "1";
		else
			 l_ss << "0";
		
		strncpy(s,l_ss.str().c_str(),255);	
		const char *rrd_param[1];
		rrd_param[0] = s;

		if (CONF->coGetVerbose()) LOGGER->Log(l_ss.str());
					
		int l_rrd_err = rrd_update_r(l_rrdfile.c_str(),NULL,1,rrd_param);
		if (l_rrd_err == -1)
		{
			char *l_err = rrd_get_error();
			LOGGER->Log("RRD ret code %s",l_err);
			rrd_clear_error();
		}	
	}
	else
	{
		LOGGER->Log("Missing rrd file parameter in config file. No rrd update will happen.");
	}
}

////////////////////////////////////////////////////////////////////////////////////////

int f_oldWeekday = -1;

////////////////////////////////////////////////////////////////////////////////////////

void ProcessValues(bool f_iok, float f_it,float f_ih,bool f_ook, float f_ot, float f_oh)
{
	// TODO: debug fake

	if (CONF->isValid("debug_ot"))
	{
		f_ook 	= true;
		f_ot 	= CONF->GetOptionFloat("debug_ot");
		f_oh 	= CONF->GetOptionFloat("debug_orh");
	}

	// --- calculate internal and external absolute hum
	
	float l_iah = SHT1x::SHT1x_CalcAbsHumidity(f_ih,f_it);
	float l_oah = SHT1x::SHT1x_CalcAbsHumidity(f_oh,f_ot);

	// --- update the RRD file for the fancy graphics
	
	UpdateRRD(f_iok,f_it,f_ih,l_iah,f_ook, f_ot, f_oh,l_oah);
	
	// --- get some conf vars

	float l_humi_diff_on 	= CONF->GetOptionFloat("humi_diff_on");
	float l_humi_diff_off 	= CONF->GetOptionFloat("humi_diff_off");

	if (CONF->coGetVerbose()) LOGGER->Log("diff on %f diff off %f",l_humi_diff_on,l_humi_diff_off);
		
	// --- check if we can continue (both sensors need to be ok)
	
	if (f_iok == true && f_ook == true)
	{
		if (IsFanAllowed(f_it,f_ih,f_ot,f_oh))
		{
			if ( l_iah > (l_oah+l_humi_diff_on) )
			{			
				FAN->Start();
			}
			
			if ( l_iah < (l_oah+l_humi_diff_off) )
			{
				FAN->Stop();
			}
		}
		else
		{
			// --- fan not allowed --> stop it
			
			FAN->Stop();
		}
	}
	else
	{
		if (CONF->coGetVerbose()) LOGGER->Log("Fan update not performed as one sensor value invalid");
	}
	
	// --- check for day change to reset the fan runtime
	
	time_t t = time(0);   
    struct tm *now = localtime(&t);
  
  	if (f_oldWeekday != now->tm_wday)
  	{
		if (CONF->coGetVerbose()) LOGGER->Log("Day change detected. Runtime was %f Reset to zero.",FAN->GetRuntime());

		// --- clear it
		
  		FAN->ClearRuntime();
  		
  		// --- remember the new day
  		
  		f_oldWeekday = now->tm_wday;
  	}	
}

