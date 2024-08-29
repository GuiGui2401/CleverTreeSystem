#include "WindSpeedMeasurement.h"

WiFiClient      m_c_WiFiClient;
ClientHandler m_c_ClientHandler(WiFi, m_c_WiFiClient);
WindSpeedMeasurement m_c_WindSpeedMeasurement(m_c_ClientHandler);
tBool m_b_InitDone = false;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:   Guiyoba Jonathan | Digital Transformation & Technology
                                Alliance
@Creation: 23.06.2023
----------------------------------------------------------------
@Function Description:  void
----------------------------------------------------------------
@parameter: -
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void setup() 
{
    m_b_InitDone = m_c_WindSpeedMeasurement.b_OnEntry();
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Guiyoba Jonathan | Digital Transformation & Technology
                                Alliance
@Creation: 23.06.2023
----------------------------------------------------------------
@Function Description:  loop
----------------------------------------------------------------
@parameter: -
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void loop() 
{
    if(!m_b_InitDone)
    {
        m_b_InitDone = m_c_WindSpeedMeasurement.b_OnEntry();
    }
    else
    {
        m_c_WindSpeedMeasurement.b_OnDuring();
    }

}