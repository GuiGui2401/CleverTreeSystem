#include "DHT11Measurement.h"

MQ2                         m_c_MQ2Sensor(MQ2Pin);
MQ2Measurement              m_c_MQ2Measurement(m_c_MQ2Sensor);
WiFiClient                  m_c_WiFiClient;
ClientHandler               m_c_ClientHandler(WiFi, m_c_WiFiClient);
SI114X                      m_c_SI114X = SI114X();
SunlightSensorMeasurement   m_c_SunlightSensorMeasurement(m_c_SI114X);
DHT11Measurement            m_c_DHT11Measurement(m_c_ClientHandler, m_c_MQ2Measurement, m_c_SunlightSensorMeasurement);
tBool                       m_b_InitDone = false;


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 23.06.2023
----------------------------------------------------------------
@Function Description:  setup
----------------------------------------------------------------
@parameter: -
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void setup() 
{
    m_b_InitDone = m_c_DHT11Measurement.b_OnEntry();
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
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
        m_b_InitDone = m_c_DHT11Measurement.b_OnEntry();
        Serial.println("DHT11_Wrapper: Init failed, Retry...\n");
    }
    else
    {
        m_c_DHT11Measurement.b_Onduring();
    }
}
