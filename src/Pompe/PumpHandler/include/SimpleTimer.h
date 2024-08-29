#include "additional_types.h"
#include "Rte_Type_Def.h"


class SimpleTimer
{
public:

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  constructor
----------------------------------------------------------------
@parameter: tUInt32 ui32_TimeouInMilliSec
@Returnvalue: --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
SimpleTimer(tUInt32 ui32_TimeouInMilliSec)
: m_ui32_TimeoutInMilliSec(ui32_TimeouInMilliSec)
, m_b_IsRunning(false)
{

}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  destructor
----------------------------------------------------------------
@parameter: -
@Returnvalue: --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
virtual ~SimpleTimer()
{

}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  start the timer if it's not running
----------------------------------------------------------------
@parameter: -
@Returnvalue: --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_Start()
{
   if(!m_b_IsRunning)
   {
      m_b_IsRunning = true;
   }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  tick function
----------------------------------------------------------------
@parameter: -
@Returnvalue: true if the timer is running otherwise false
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_Tick()
{
   tUInt32 ui32_CurrentTime = 0;
   Rte_Call_ESP8266_TimeService_Read_ms(ui32_CurrentTime);
   if(ui32_CurrentTime > m_ui32_TimeoutInMilliSec)
   {
      v_Stop();
      v_TimeOut();
   }
   return m_b_IsRunning;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  stop timer
----------------------------------------------------------------
@parameter: -
@Returnvalue: --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_Stop()
{
   m_b_IsRunning = false;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  check if the timer is running
----------------------------------------------------------------
@parameter: -
@Returnvalue: true the timer is running otherwise false
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_IsRunning()
{
   return m_b_IsRunning;
}

// pure virtual function
virtual tVoid v_TimeOut() = 0;

private:

   tUInt32 m_ui32_TimeoutInMilliSec;
   tBool m_b_IsRunning;

};

