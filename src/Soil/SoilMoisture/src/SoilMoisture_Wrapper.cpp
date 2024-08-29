
#include "SoilMoistureMeasurement.h"
#include "Rte_Type_Def.h"


static const tUInt8 m_ui8_FourthOctet = 201;
tUInt8 m_aui8_DataToSend[3];
tUInt8 ui8_PumpActivationRequested = 0x0;
tUInt8 ui8_MoisSensorValue = 0x0;
tBool m_b_ACKReceived = true;
static const tUInt16 ui16_Dry = 30;
tBool m_b_InitDone = false;
tChar ssid[21] = {'W','i','F','i','_','C','l','e','v','e','r','T','r','e','e','\0'};
tChar passwd[16] = {'0','2','6','5','5','8','2','6','\0'};
WiFiClient m_c_WiFiClient;
WiFi_Server m_c_WiFi_Server(m_c_WiFiClient, m_ui8_FourthOctet, passwd, ssid, WiFi);
SoilMoistureMeasurement m_c_SoilMoistureMeasurement(m_c_WiFi_Server);

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  setup
----------------------------------------------------------------
@parameter: --
@Returnvalue: 
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void setup() 
{
  m_b_InitDone = m_c_SoilMoistureMeasurement.b_OnEntry();
  if(m_b_InitDone)
  {
    WiFi_Server::m_c_Logger.v_Print("SM_Wrapper : INIT DONE\n");
  }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  loop
----------------------------------------------------------------
@parameter: --
@Returnvalue: 
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void loop() 
{
  if(!m_b_InitDone)
  {
    m_b_InitDone = m_c_SoilMoistureMeasurement.b_OnEntry();
    Serial.println("SM_Wrapper: Init failed, Retry...");
  }
  else
  {
    m_c_SoilMoistureMeasurement.b_OnDuring();
  }
  delay(100);
}

