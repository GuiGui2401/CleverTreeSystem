#include "TankAndPumpHandler.h"
#include <SoftwareSerial.h>

/*-------------------------------
      GLOBAL DEFINE HERE
--------------------------------*/


/*-------------------------------
      GLOBAL VARIABLE HERE
--------------------------------*/
SoftwareSerial m_c_SoftwareSerial_0(RX_PIN0, TX_PIN0);
SoftwareSerial m_c_SoftwareSerial_1(RX_PIN1, TX_PIN1);
static const tUInt8 m_ui8_RelaiPort = D4;
static const tUInt8 m_ui8_FourthOctet = 210;
tChar ssid[21] = {'W','i','F','i','_','C','l','e','v','e','r','T','r','e','e','\0'};
tChar passwd[16] = {'0','2','6','5','5','8','2','6','\0'};
WiFiClient m_c_WiFiClient;
TankHandler m_c_TankHandler;
PumpHandler m_c_PumpHandler(m_ui8_RelaiPort, e_Normaly_Close);
WiFi_Server m_c_WiFi_Server(m_c_WiFiClient, m_ui8_FourthOctet, passwd, ssid, WiFi, m_c_SoftwareSerial_0 , m_c_SoftwareSerial_1);
TankAndPumpHandler m_c_TankAndPumpHandler(m_c_TankHandler, m_c_PumpHandler, m_c_WiFi_Server);
tBool m_b_InitDone = false;
tUInt32 ui32_ElapsedTime = 0;
tUInt32 ui32_OldTime = 0; 

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 23.04.2023
----------------------------------------------------------------
@Function Description:  setup
----------------------------------------------------------------
@parameter: -
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void setup() 
{
  m_b_InitDone = m_c_TankAndPumpHandler.b_OnEntry(); 
  if(m_b_InitDone)
  {
    WiFi_Server::m_c_Logger.v_Print("PH: INIT DONE\n");
  }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 23.04.2023
----------------------------------------------------------------
@Function Description:  loop
----------------------------------------------------------------
@parameter: -
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void loop() 
{
  if(m_b_InitDone)
  {
    
    tUInt32 ui32_CurrentTime = millis();

    if(ui32_OldTime > ui32_CurrentTime)
    {
        ui32_CurrentTime += UINT32_MAX - ui32_OldTime;        // uint32 overflow 
    }
    ui32_ElapsedTime = ui32_CurrentTime - ui32_OldTime;
    m_c_TankAndPumpHandler.v_SetElapsedTime(ui32_ElapsedTime);      // handover the current process time

    if(ui32_ElapsedTime  >= TIMER_5_MIN)
    {
      ui32_OldTime += ui32_ElapsedTime;
      ui32_ElapsedTime = 0;
    }
    (tVoid)m_c_TankAndPumpHandler.b_OnDuring();
    
  }
  else
  {
    m_b_InitDone = m_c_TankAndPumpHandler.b_OnEntry();
  }
  delay(100);
}

