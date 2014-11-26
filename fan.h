////////////////////////////////////////////////////////////////////////////////////////
//	
//	humifand 	- 	the humidity fan deamon
//
//	Please check www.way2.net for more information
//
//  (c) Copyright 2014 by way2.net Services. 
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef FAN_H
#define FAN_H

////////////////////////////////////////////////////////////////////////////////////////

#define FAN CFan::getFan()

////////////////////////////////////////////////////////////////////////////////////////

class CFan
{
public:
	
	void InitPins(int f_Fan_pin_sck);
    static CFan* getFan();
    
    void Start(void);
    void Stop(void);
    
    bool IsFanSpinning(void)
    {
    	return m_FanSpinning;
    }

	double GetRuntime(void);
		
	void ClearRuntime(void)
	{
		m_RunTime = 0;
	}
	
private:

    CFan();
    CFan( const CFan&){};
    CFan& operator=(const CFan& )
    { 
    	return *this;
    };
    
    double CalcRuntime(void);
    
    static CFan* m_pThis;
    
    int 	m_Fan_pin_sck;
    bool 	m_FanSpinning;
    
    time_t	m_LastStartTime;
    double	m_RunTime; 
};




////////////////////////////////////////////////////////////////////////////////////////


#endif

