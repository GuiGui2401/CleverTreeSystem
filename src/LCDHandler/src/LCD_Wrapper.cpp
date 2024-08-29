#include "ClientHandler.h"

WiFiClient      m_c_WiFiClient;
ClientHandler   m_c_ClientHandler(WiFi, m_c_WiFiClient);
tBool m_b_InitDone = false;
tUInt32 m_ui32_ElapsedTime;
tUInt32 m_ui32_OldTime;

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
  m_b_InitDone = m_c_ClientHandler.b_OnEntry();
  if(m_b_InitDone)
  {
    ClientHandler::m_c_Logger.v_Print("INIT DONE\n");
  }
  m_ui32_OldTime = millis();
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
  tUInt8 ui8_Internalcounter = 0;
  if(!m_b_InitDone)
  {
    m_b_InitDone = m_c_ClientHandler.b_OnEntry(true);
  }
  else
  {
    m_c_ClientHandler.b_OnDuring();
    m_ui32_ElapsedTime = millis() - m_ui32_OldTime;
    m_ui32_OldTime += m_ui32_ElapsedTime;
    if(m_ui32_ElapsedTime > DISPLAY_TIME)
    {
      ui8_Internalcounter = (ui8_Internalcounter + 1) % MAX_DISPLAY_NUMBER;
    }
    if(ui8_Internalcounter == 0)
    {
      m_c_ClientHandler.v_LiquidCrystalDisplayPrint_02();
    }
    else if(ui8_Internalcounter == 1)
    {
      m_c_ClientHandler.v_LiquidCrystalDisplayPrint_01();
    }
    else
    {

    }
    delay(WAIT_500_MS);
  }
}

