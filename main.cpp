////////////////////////////////////////////////////////////////////////////////////////
//	
//	humifand 	- 	the humidity fan deamon
//
//	Please check www.way2.net for more information
//
//  (c) Copyright 2014 by way2.net Services. 
//
////////////////////////////////////////////////////////////////////////////////////////

#include <bcm2835.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <errno.h>


#include <map>
#include <string>

#include "conffile.h"
#include "RPi_SHT1x.h"
#include "fan.h"
#include "logger.h"
#include "ProcessValues.h"

////////////////////////////////////////////////////////////////////////////////////////
        
//bool opt_getADaemon 		= false;
//bool opt_verbose 			= false;

const char *opt_ConfigFile 	= "/etc/humifand.conf";

bool 	 	g_QuitMainLoop 	= false;

// --- here are the last values read!

float		g_InsideTemp;
float		g_InsideHumi;
float		g_OutsideTemp;
float		g_OutsideHumi;

// --- out sensor instances

SHT1x 		g_InsideSensor(RPI_BPLUS_GPIO_J8_38, RPI_BPLUS_GPIO_J8_40);
SHT1x 		g_OutsideSensor(RPI_BPLUS_GPIO_J8_35, RPI_BPLUS_GPIO_J8_37);

// --- signal handlign

struct sigaction sigact;

////////////////////////////////////////////////////////////////////////////////////////

void printTempAndHumidity(SHT1x &f_Sensor)
{
	unsigned char noError = 1;  
	value humi_val,temp_val;
    
	// Set up the SHT1x Data and Clock Pins
	
	f_Sensor.SHT1x_InitPins();
	
	// Reset the SHT1x
	f_Sensor.SHT1x_Reset();
	
	// Request Temperature measurement
	noError = f_Sensor.SHT1x_Measure_Start(SHT1xMeaT );
	if (!noError) {
		printf("SHT1x_Measure_Start failed\n");
		return;
		}
		
	// Read g_InsideSensor measurement
	noError = f_Sensor.SHT1x_Get_Measure_Value((unsigned short int*) &temp_val.i );
	if (!noError) {
		printf("SHT1x_Get_Measure_Value failed\n");
		return;
		}

	// Request Humidity Measurement
	noError = f_Sensor.SHT1x_Measure_Start(SHT1xMeaRh );
	if (!noError) {
		printf("SHT1x_Measure_Start failed\n");
		return;
		}
		
	// Read Humidity measurement
	noError = f_Sensor.SHT1x_Get_Measure_Value((unsigned short int*) &humi_val.i );
	if (!noError) {
		printf("SHT1x_Get_Measure_Value failed\n");
		return;
		}

	// Convert intergers to float and calculate true values
	temp_val.f = (float)temp_val.i;
	humi_val.f = (float)humi_val.i;
	
	// Calculate Temperature and Humidity
	f_Sensor.SHT1x_Calc(&humi_val.f, &temp_val.f);
	
	// Calc Dew Point
	
	float fDP = f_Sensor.SHT1x_CalcDewpoint(humi_val.f,temp_val.f);


	//Print the Temperature to the console
	printf("Temperature: %0.2f C\n",temp_val.f);

	//Print the Humidity to the console
	printf("Humidity: %0.2f%%\n",humi_val.f);

	//Print the Temperature to the console
	printf("Dew Point: %0.2f C\n",fDP);
}

////////////////////////////////////////////////////////////////////////////////////////

void CheckFan(void)
{
    FAN->InitPins(RPI_BPLUS_GPIO_J8_36);

    for (int i=0;i<10;++i)
    {
        printf("Starting fan....\n");
        FAN->Start();
        sleep(5);
        
        printf("Stopping fan....\n");
        FAN->Stop();
        sleep(5);
    }
}


////////////////////////////////////////////////////////////////////////////////////////

bool AskSensor(SHT1x &f_Sensor,float &f_Humi, float &f_Temp)
{
	unsigned short int l_TempDigit;
	unsigned short int l_HumiDigit;
	
    delay(100);

    int noError = f_Sensor.SHT1x_Measure_Start(SHT1xMeaT );
	if (!noError) 
	{
		LOGGER->Log("SHT1x_Measure_Start failed");
		return false;
	}
		
    delay(100);
    
	// Read Temperature measurement
	noError = f_Sensor.SHT1x_Get_Measure_Value(&l_TempDigit);
	if (!noError) 
	{
		LOGGER->Log("SHT1x_Get_Measure_Value failed");
		return false;
	}

    delay(100);

    // Request Humidity Measurement
	noError = f_Sensor.SHT1x_Measure_Start(SHT1xMeaRh );
	if (!noError) 
	{
		LOGGER->Log("SHT1x_Measure_Start failed");
		return false;
	}
		
    delay(100);

    // Read Humidity measurement
	noError = f_Sensor.SHT1x_Get_Measure_Value(&l_HumiDigit);
	if (!noError) 
	{
		LOGGER->Log("SHT1x_Get_Measure_Value failed");
		return false;
	}

	// Convert intergers to float and calculate true values
	
	f_Temp = (float)l_TempDigit;
	f_Humi = (float)l_HumiDigit;
	
	// Calculate Temperature and Humidity
	
	f_Sensor.SHT1x_Calc(&f_Humi, &f_Temp);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////

void MainLoop(void)
{
	int l_CycleTime;
	
	if (!CONF->isValid("cycle_time"))
	{
		LOGGER->Log("cycle_time missing in config file. Default to 30 sec");
		
		l_CycleTime = 30;
	}
	else
	{
		l_CycleTime = CONF->GetOptionInt("log_file");
	}

	// --- do some init (fan and sensors)

	delay(100);

	g_InsideSensor.SHT1x_InitPins();
	g_InsideSensor.SHT1x_Reset();

    delay(100);

    g_OutsideSensor.SHT1x_InitPins();
	g_OutsideSensor.SHT1x_Reset();
	
    delay(100);

    FAN->InitPins(RPI_BPLUS_GPIO_J8_36);
	
    delay(100);

	// --- the magic loop
	
	int l_LoopCounter = 0;
	
	while (!g_QuitMainLoop)
	{
		//LOGGER->Log("Loop %d",l_LoopCounter);
		
		// --- bounce every 1 sec through the loop to react more fast e.g. on quit signal
		
		l_LoopCounter++;
		
		// --- if we hit the cycle time, process!
		
		if (l_LoopCounter == l_CycleTime)
		{
			// --- get humi and temp for both sensors
			
			bool l_insideok = AskSensor(g_InsideSensor, g_InsideHumi, g_InsideTemp);
			bool l_outsideok = AskSensor(g_OutsideSensor, g_OutsideHumi, g_OutsideTemp);
			
			if (!l_insideok) 
				LOGGER->Log("Failed to get inside sensor value");

			if (!l_outsideok) 
				LOGGER->Log("Failed to get outside sensor value");

			if (CONF->coGetVerbose())
			{
				if (l_insideok) 
					LOGGER->Log("Inside Temp %0.2f °C Humi %0.2f%%",g_InsideTemp,g_InsideHumi);
				else
					LOGGER->Log("Inside Temp invalid");
				
				if (l_outsideok)				
					LOGGER->Log("Outside Temp %0.2f °C Humi %0.2f%%",g_OutsideTemp,g_OutsideHumi);
				else
					LOGGER->Log("Outside Temp invalid");

			}
			
			// --- this is where rrd gets updated and the fans get controlled
			
			ProcessValues(l_insideok,g_InsideTemp,g_InsideHumi,l_outsideok,g_OutsideTemp,g_OutsideHumi);
			
			l_LoopCounter = 0;
		}
			
		sleep(1);
	}	
}

////////////////////////////////////////////////////////////////////////////////////////

bool ReadConfigFile(void)
{
	CONF->Clear();

	// --- try to read the config file
	
	if (CONF->coGetVerbose()) printf("humifand: use config file '%s'\n",opt_ConfigFile);
	
	if (!CONF->processConfigFile(opt_ConfigFile))
	{
		printf("Error reading config file. Abort.\n");
		return false;
	}

	/*
	// --- print values in case we are in verbose mode
	
	if (CONF->coGetVerbose())
	{
		printf("Successfully read the config file. Following values:\n");
		CONF->PrintConfMap();		
	}
	*/
		
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////

static void signal_handler(int sig)
{
    if (sig == SIGINT) 
    {
		LOGGER->Log("CTRL_C signal detected. Exit gracefully");
		g_QuitMainLoop = true;
    }
    
    if (sig == SIGHUP) 
    {
		LOGGER->Log("SIGHUP detected. Read config file again!");
		ReadConfigFile();
    }

    if (sig == SIGTERM) 
    {
		LOGGER->Log("SIGTERM detected. Exit gracefully!");
		g_QuitMainLoop = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////

void cleanup(void)
{
    sigemptyset(&sigact.sa_mask);
}

////////////////////////////////////////////////////////////////////////////////////////

void init_signals(void)
{
    sigact.sa_handler = signal_handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;

    sigaction(SIGINT, &sigact, (struct sigaction *)NULL);
    sigaction(SIGTERM, &sigact, (struct sigaction *)NULL);
    sigaction(SIGHUP, &sigact, (struct sigaction *)NULL);
}

////////////////////////////////////////////////////////////////////////////////////////

void start_daemon(void) 
{
    pid_t pid, sid;

   	// --- Fork the Parent Process
    pid = fork();

    if (pid < 0)
    { 
    	fprintf (stderr, "Fork for daemon failed (%d)\n", errno);
    	exit(EXIT_FAILURE); 
    }

    // --- We got a good pid, Close the Parent Process
    
    if (pid > 0)
    { 
    	exit(EXIT_SUCCESS); 
    }

    // --- Change File Mask
    
    umask(0);

    // --- reate a new Signature Id for our child
    
    sid = setsid();
    if (sid < 0)
    { 
    	fprintf (stderr, "setsid for daemon failed (%d)\n", errno);
    	exit(EXIT_FAILURE); 
    }

    // --- Change Directory
    // --- If we cant find the directory we exit with failure.
    
    if ((chdir("/")) < 0)
    { 
    	fprintf (stderr, "chdir to root failed (%d)\n", errno);
    	exit(EXIT_FAILURE); 
    }

    // --- Close Standard File Descriptors
    
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

}

////////////////////////////////////////////////////////////////////////////////////////

void CreatePIDFile()
{
	if (!CONF->isValid("pid_file")) return;
	std::string l_pf = CONF->GetOption("pid_file");

	std::ofstream l_file;
	
	l_file.open(l_pf.c_str(), ios::out);
	l_file << getpid() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////

void RemovePIDFile()
{
	if (!CONF->isValid("pid_file")) return;
	std::string l_pf = CONF->GetOption("pid_file");
	remove(l_pf.c_str());	
}

////////////////////////////////////////////////////////////////////////////////////////

int main (int argc, char **argv)
{
// --- process options
		
	int c;

	opterr = 0;
    
    while ((c = getopt (argc, argv, "dc:tvf")) != -1)
		switch (c)
		{
	   		case 'v':
		 			CONF->coSetVerbose(true);
		 			break;

	   		case 'd':
		 			CONF->coSetDaemonMode(true);
		 			break;
		 
	   		case 't':
                    bcm2835_init();
                    delay(20);
                
                    printf("Inside:\n");
 	   				printTempAndHumidity(g_InsideSensor);
                    printf("Outside:\n");
                    printTempAndHumidity(g_OutsideSensor);
	   				return 255;
		 			break;

            case 'f':
                    bcm2835_init();
                    delay(20);
                
                    CheckFan();
                    return 255;
                    break;

	   		case 'c':
		 			opt_ConfigFile = optarg;
		 			break;
		 			
	   		case '?':
		 			if (optopt == 'c')
		   					fprintf (stderr, "Option -%c requires an argument. Abort.\n", optopt);
		 			else if (isprint (optopt))
		   					fprintf (stderr, "Unknown option `-%c'. Abort.\n", optopt);
		 			else
		   					fprintf (stderr,"Unknown option character `\\x%x'. Abort.\n",optopt);
					return 255;
	   default:
		 			abort();
	   }

	// --- if there are more arguments, this is an error 
	
	if (optind != argc)
	{
		printf ("Too many arguments supplied: '%s'. Abort.\n", argv[optind]);
		return 255;
	}
	
	// --- read the config file
	
	if (!ReadConfigFile())
	{
		return 255;
	}
	
	// --- open the log file

	if (!CONF->isValid("log_file"))
	{
		printf("Missing log file parameter in config file. Abort.\n");
		return 255;
	}
	
	std::string l_logfile = CONF->GetOption("log_file");
	if (!LOGGER->OpenLog(l_logfile))
	{
		printf("Unable to open logfile '%s'. Abort.\n",l_logfile.c_str());
		return 255;
	}

	// --- Initialise the Raspberry Pi GPIO
	
	if(!bcm2835_init())
	{
		fprintf (stderr, "bcm2835_init failed. Are you root? Abort.\n");
		return 255;
	}

	// --- init signals
	
    atexit(cleanup);
    init_signals();
    	
	// --- get a daemon?
	
	if (CONF->coGetDaemonMode())
	{
		start_daemon();
		CreatePIDFile();
	}

	// --- startup messages
	
	LOGGER->Log("humifand deamon starting up...let's make it less wet!");
	
	// --- Enter the main loop
	
	MainLoop();
	
	// --- remove PID file

	if (CONF->coGetDaemonMode())
	{
		RemovePIDFile();
	}

	// --- shutdown messages
    
	LOGGER->Log("humifand deamon shutting down...let the drought be with you!");
	
	return 0;
}
