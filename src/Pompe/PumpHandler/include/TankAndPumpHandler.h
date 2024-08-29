#ifndef TANKANDPUMPHANDLER_H
#define TANKANDPUMPHANDLER_H
#include "PumpHandler.h"
#include "WiFi_Server.h"
#include "TankHandler.h"



class TankAndPumpHandler
{

private:
TankHandler& m_rc_TankHandler;
PumpHandler& m_rc_PumpHandler;
WiFi_Server& m_rc_WiFi_Server;
tUInt8 m_aui8_DataToSend[3] = {0};
tUInt8 m_ui8_WaterLevel ;
tBool m_b_PumpIsActivated;
tUInt32 m_ui32_ElapsedTime ;
tBool m_b_AcknowledgeIsPending;
tUInt32 m_ui32_ElapsedTimePumpActivation;
tUInt32 m_ui32_OldTimePumpActivation;
tBool m_b_PumpRequest;
tBool m_b_TryToActivateThePump;
tUInt32 m_ui32_SendMsgCounter;
tUInt32 m_ui32_RecvMsgCounter;
tBool m_b_OldPumpisEnableViaWebInterface;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  The function reads data from the serial
bus and if the pump has been enabled, then set m_ui8_PumpActivation Requested
to true
----------------------------------------------------------------
@parameter: --
@Returnvalue: --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_PumpIsEnabledViaWebInterface()
{
  tBool b_IsActivated = m_rc_WiFi_Server.b_PumpActivationRequestedViaWebInterface();
  WiFi_Server::m_c_Logger.v_Print("TAPH: b_IsActivated ");
  Serial.println(b_IsActivated);
  if(!m_b_OldPumpisEnableViaWebInterface && b_IsActivated)
  {
    WiFi_Server::m_c_Logger.v_Print("TAPH: Pump enable via webinterface\n");
    m_b_OldPumpisEnableViaWebInterface = true;
    return true;
  }
  return false;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  The function reads data from the serial
bus and if the pump has been enabled, then set m_ui8_PumpActivation Requested
to true
----------------------------------------------------------------
@parameter: --
@Returnvalue: --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_PumpIsDisableViaWebInterface()
{
  tBool b_IsDeactivated = m_rc_WiFi_Server.b_PumpActivationRequestedViaWebInterface();
  if(m_b_OldPumpisEnableViaWebInterface && !b_IsDeactivated)
  {
    WiFi_Server::m_c_Logger.v_Print("TAPH: Pump disable via webinterface\n");
    m_b_OldPumpisEnableViaWebInterface = false;
    return true;
  }
  return false;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  
----------------------------------------------------------------
@parameter: --
@Returnvalue: --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_ReadTheTankLevelEach5000MS()
{

  if(m_ui32_ElapsedTime >= TIMER_5_MIN)
  {
    v_SetDataToSend();
    char c_DebugMsg[64];
    sprintf(c_DebugMsg, "TAPH: WaterTank Level: %d Percent\n", m_aui8_DataToSend[2] );
    WiFi_Server::m_c_Logger.v_Print(c_DebugMsg);
  }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  @Author: Gerald Emvoutou
  @Create Date: 24/06/2023
  ---------------------------------------------------------------------
  @Function description: 
  ---------------------------------------------------------------------
  @Parameter: tUInt32& ui32_OldTime
  @Return value:  
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tUInt32 ui32_GetElapsedTime(tUInt32& ui32_OldTime)
{
  tUInt32 ui32_CurrentTime = millis();
  tUInt32 ui32_ElapsedTime = 0;
  if(ui32_OldTime > ui32_CurrentTime)
  {
    ui32_ElapsedTime = ui32_CurrentTime + (UINT32_MAX - ui32_OldTime);
  }
  else
  {
    ui32_ElapsedTime = ui32_CurrentTime - ui32_OldTime;
  }
  ui32_OldTime += ui32_ElapsedTime;
  return ui32_ElapsedTime;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  Get if a pump activation is requested
----------------------------------------------------------------
@parameter: --
@Returnvalue:   True -- the pump activation was requested via 
                    Server or Webinterface
                False -- the pump activation wasn't requested via 
                    Server or Webinterface
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_ActivationRequested()
{
   return (b_PumpIsEnabledViaWebInterface() || m_rc_WiFi_Server.ui8_GetPumpActivationRequested());
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  set data in the buffer to send
----------------------------------------------------------------
@parameter: --
@Returnvalue:  --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_SetDataToSend()
{
  tUInt8 ui8_Position = 0;
  m_aui8_DataToSend[ui8_Position++] = MSG_ID_PUMP_STATUS_MESSAGE;
  m_aui8_DataToSend[ui8_Position++] = m_rc_PumpHandler.ui8_GetPumpStatus();
  m_aui8_DataToSend[ui8_Position] = m_rc_TankHandler.ui8_GetTankLevel();
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  
----------------------------------------------------------------
@parameter: -
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_Reset()
{
  m_b_PumpRequest = false;
  m_b_PumpIsActivated = false;
  m_ui32_ElapsedTimePumpActivation = 0;
  m_b_TryToActivateThePump = false;
}

public:
TankAndPumpHandler(TankHandler& rc_TankHandler , PumpHandler& rc_PumpHandler, WiFi_Server& rc_WiFi_Server)
: m_rc_TankHandler(rc_TankHandler)
, m_rc_PumpHandler(rc_PumpHandler)
, m_rc_WiFi_Server(rc_WiFi_Server)
, m_ui8_WaterLevel(0)
, m_b_PumpIsActivated(false)
, m_ui32_ElapsedTime(0)
, m_b_AcknowledgeIsPending(false)
, m_ui32_ElapsedTimePumpActivation(0)
, m_ui32_OldTimePumpActivation(0)
, m_b_PumpRequest(false)
, m_b_TryToActivateThePump(false)
, m_ui32_SendMsgCounter(0)
, m_ui32_RecvMsgCounter(0)
, m_b_OldPumpisEnableViaWebInterface(false)
{

}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  set the elapsed time
----------------------------------------------------------------
@parameter: tUInt32 ui32_ElapsedTime
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_SetElapsedTime(tUInt32 ui32_ElapsedTime)
{
  m_ui32_ElapsedTime = ui32_ElapsedTime;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  The function will be call in main setup
----------------------------------------------------------------
@parameter: --
@Returnvalue: True -- all Init are done
              False -- Init failed
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_OnEntry()
{
  tBool b_RetVal;
  m_rc_PumpHandler.v_Init();
  b_RetVal = m_rc_WiFi_Server.b_StartServer();
  // Read the Tank level at the beginning of the sws
  v_SetDataToSend();
  return b_RetVal;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  The function will be call cyclically in loop
----------------------------------------------------------------
@parameter: --
@Returnvalue: return always true
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_OnDuring()
{

  if(!m_b_PumpRequest)
  {
    m_b_PumpRequest = b_ActivationRequested();
  }
  // Read the water level each 5 min and save in the array
  v_ReadTheTankLevelEach5000MS();
  
  /*
  * When the pump is request, then try to activate the pump
  */
  if(m_b_PumpRequest && !m_b_TryToActivateThePump)
  {
    m_b_TryToActivateThePump = true;
    WiFi_Server::m_c_Logger.v_Print("TAPH: Pump activation requested\n");
  }

  if(m_b_TryToActivateThePump)
  {
   
    /*
        Activate the pump when the tank level is greater than 5% otherwise remain close
        save the data in the buffer
    */
    if(!m_b_PumpIsActivated)
    {
      if((MIN_TANKLEVEL_FOR_TANK_ACTIVATION <= m_aui8_DataToSend[2]))
      {
        m_rc_PumpHandler.v_OpenPump();
        v_SetDataToSend();
        m_b_PumpIsActivated = true;
        m_ui32_OldTimePumpActivation = millis();
        WiFi_Server::m_c_Logger.v_Print("TAPH: Pump has been activated\n\n");
      }
      else
      {
        v_Reset();
        tChar c_Debug[128];
        sprintf(c_Debug, "TAPH: Pump activation is requested, but the tank level is less than 5 percent current value=%d\n", m_aui8_DataToSend[2]);
        WiFi_Server::m_c_Logger.v_Print(c_Debug);
      }
    }
    else
    {
      //WiFi_Server::m_c_Logger.v_Print("TAPH: The pump is already opened\n");
      m_ui32_ElapsedTimePumpActivation = millis() - m_ui32_OldTimePumpActivation;
      Serial.println(m_ui32_ElapsedTimePumpActivation);
      if(m_ui32_ElapsedTimePumpActivation >= TIMER_PUMP || b_PumpIsDisableViaWebInterface())
      {
        tChar c_Debug[64];
        sprintf(c_Debug, "\nTAPH: The pump was closed after %02dms\n\n", m_ui32_ElapsedTimePumpActivation);
        WiFi_Server::m_c_Logger.v_Print(c_Debug);
        m_rc_PumpHandler.v_ClosePump();
        m_ui32_OldTimePumpActivation += m_ui32_ElapsedTimePumpActivation;
        v_Reset();
        v_SetDataToSend();
        m_rc_WiFi_Server.v_PumpActivationRequestedReset();

      }
    }
  }

  // send data to the Server and read the ACK
  if(m_rc_WiFi_Server.b_ClientIsRunning(m_b_AcknowledgeIsPending))
  {
    if (!m_b_AcknowledgeIsPending)                                       // If the ACK is not pending, we was able to send a message to the server
    {
      if( m_rc_WiFi_Server.b_SendMessage(&m_aui8_DataToSend[0], sizeof(m_aui8_DataToSend)))
      {
        tChar c_DebugMsgCouter[64];
        sprintf(c_DebugMsgCouter," Message counter Send: %.2X, Received: %.2X\n" , m_ui32_SendMsgCounter++, m_ui32_RecvMsgCounter);
        WiFi_Server::m_c_Logger.v_Print(c_DebugMsgCouter);
        m_b_AcknowledgeIsPending = true;
      }
      else
      {
        WiFi_Server::m_c_Logger.v_Print("TAPH: Message can not be send\n");
      }
    }
    else 
    {
        WiFi_Server::m_c_Logger.v_Print("TAPH: Acknowldge is pending\n");
        if (m_rc_WiFi_Server.b_HandlerReceivedMessage())
        {
          tChar c_DebugMsgCouter[64];
          sprintf(c_DebugMsgCouter," Message counter Send: %.2X, Received: %.2X\n" , m_ui32_SendMsgCounter, m_ui32_RecvMsgCounter++);
          WiFi_Server::m_c_Logger.v_Print(c_DebugMsgCouter);
          m_b_AcknowledgeIsPending = false;
        }
    }
  }
  else
  {
    WiFi_Server::m_c_Logger.v_Print("TAPH: Client isn't connected\n");
  }

  return true;
}


};
#endif
