#ifndef WIFI_SERVER_H
#define WIFI_SERVER_H
#include "Logger.h"
#include "ESP8266WiFi.h"
#include "SimpleTimer.h"

#define TEST


class WiFiHandler 
{
public:
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  constructor
----------------------------------------------------------------
@parameter: ESP8266WiFiClass& rc_ESP8266WiFiClass
@Returnvalue: --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
WiFiHandler(ESP8266WiFiClass& rc_ESP8266WiFiClass)
:m_rc_ESP8266WiFiClass(rc_ESP8266WiFiClass)
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
~WiFiHandler()
{

}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  Get WiFiHandler
----------------------------------------------------------------
@parameter: -
@Returnvalue: return a ESP8266WiFiClass object
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
ESP8266WiFiClass rc_GetESP8266WiFiClass()
{
    return m_rc_ESP8266WiFiClass;
}

private:
    ESP8266WiFiClass& m_rc_ESP8266WiFiClass;
};


class WiFi_Server : public SimpleTimer
{
public:
static Logger m_c_Logger;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  constructor
----------------------------------------------------------------
@parameter: WiFiClient& rc_WiFiClient, uint8 ui8_FourthOctet, tChar* passwd, tChar* ssid,
            ESP8266WiFiClass& rc_ESP8266WiFiClass, SoftwareSerial& rc_SoftwareSerial_0, SoftwareSerial& rc_SoftwareSerial_1
@Returnvalue: --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
WiFi_Server(WiFiClient& rc_WiFiClient, uint8 ui8_FourthOctet, tChar* passwd, tChar* ssid,
                 ESP8266WiFiClass& rc_ESP8266WiFiClass, SoftwareSerial& rc_SoftwareSerial_0, SoftwareSerial& rc_SoftwareSerial_1)
: 
  SimpleTimer(1000)
, m_str_ssid(ssid)
, m_str_passwd(passwd)
, m_rc_WiFiClient(rc_WiFiClient)
, m_c_local_IP(192,168,1,ui8_FourthOctet)
, m_c_gateway(192,168,1,1)
, m_c_subnet(255,255,255,0)
, m_ui8_PumpActivationRequested(PUMP_CLOSED)
, m_ui8_PumpActivatioReqViaWebInterface(PUMP_CLOSED)
, m_c_WiFiHandler(rc_ESP8266WiFiClass)
, m_rc_SoftwareSerial_0(rc_SoftwareSerial_0)
, m_rc_SoftwareSerial_1(rc_SoftwareSerial_1)
#ifdef TIME_MEASUREMENT_SERIAL_PORT
, m_ui32_ElapsedTimeSendUARTMsgAndRcvACK(0)
, m_ui32_OldElapsedTimeSendUARTMsgAndRcvACK(0)
#endif
#ifdef TIME_MEASUREMENT
, m_ui32_ElapsedTimeSendAndREceiveMsg(0)
, m_ui32_OldElapsedTimeSendAndREceiveMsg(0)
#endif
{
    v_Reset();
    memset(m_aui8_Data, 0, sizeof(m_aui8_Data));
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
~WiFi_Server()
{

}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  timeout function calls the reset function
----------------------------------------------------------------
@parameter: --
@Returnvalue: --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_TimeOut() CLVT_DECL_OVERRIDE
{
    v_Reset();
    //m_c_Logger.v_Print("WS: Timeout called\n");
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  start the connection to WiFi and the server
----------------------------------------------------------------
@parameter: --
@Returnvalue: --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_StartServer()
{
    tBool b_ServerIsRunning = false;
    tUInt8 ui8_Counter = 0;
    m_rc_SoftwareSerial_0.begin(BAUDRATE_TXRX);
    m_rc_SoftwareSerial_1.begin(BAUDRATE_TXRX);
    m_c_Logger.v_Init();

    m_c_WiFiHandler.rc_GetESP8266WiFiClass().mode(WIFI_STA);
    m_c_WiFiHandler.rc_GetESP8266WiFiClass().begin(m_str_ssid, m_str_passwd);
    if (!m_c_WiFiHandler.rc_GetESP8266WiFiClass().config(m_c_local_IP, m_c_gateway, m_c_subnet)) 
    {
        m_c_Logger.v_Print("WS: STA Failed to configured\n");
        b_ServerIsRunning = false;
    } 
    m_c_Logger.v_Print("IP Address= ");
    Serial.print(m_c_local_IP);
    m_c_Logger.v_Print("\n", false);

    // Waiting to connect to the WiFi
    m_c_Logger.v_Print("WS: Try to connect to WiFi\n");
    tUInt8 ui8_ResetCounter = 0;
    while((ui8_ResetCounter < 5) && !b_StartWiFiConnection())
    {
        ui8_ResetCounter++;
        m_c_Logger.v_Print("WS: tried to start...\n");
        delay(10);
    }
    
    // Waiting to connect to the server 5S seconds
    while((!m_rc_WiFiClient.connect(host, port)) && (ui8_Counter < TIME_OUT_CONNECTION_SERVER)) 
    {
        ui8_Counter++;
        m_c_Logger.v_Print("WS: try to connect to the server\n");
        delay(10);
    }
    if(ui8_Counter < TIME_OUT_CONNECTION_SERVER)
    {
        b_ServerIsRunning = true;
        m_c_Logger.v_Print("WS: Connected to server\n");
    }
    else
    {
        m_c_Logger.v_Print("WS: Timeout\n");
    }
    return b_ServerIsRunning;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  send a message to server
----------------------------------------------------------------
@parameter: --
@Returnvalue: true -- the message was successful sent
              false -- sending of message failed
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_SendMessage(const tUInt8* p_aui8_Data, tUInt8 ui8_DataLength)
{
    String str_Data = "";
    tBool b_MessageWasSent = false;
    if(NULL != p_aui8_Data)
    {
        for(tUInt8 ui8_Index = 0; ui8_Index < ui8_DataLength; ++ui8_Index)
        {
            str_Data += String(p_aui8_Data[ui8_Index]);
            if((ui8_DataLength-1) > ui8_Index)
            {
                str_Data += ".";
            }
        }
        tChar c_Debug[128];
        m_rc_WiFiClient.print(str_Data);
        b_MessageWasSent = true;
        #ifdef TIME_MEASUREMENT
		m_ui32_OldElapsedTimeSendAndREceiveMsg = millis();
		#endif
        sprintf(c_Debug, "WS: [TX] ID=%.2X OK  PumpStatus=%d, TankLevel=%d\n", p_aui8_Data[0], p_aui8_Data[1], p_aui8_Data[2]);
        m_c_Logger.v_Print(c_Debug);
    }

    return b_MessageWasSent;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  Handle a message received from the server
----------------------------------------------------------------
@parameter: --
@Returnvalue:   True -- the ACK was received
                False -- the ACK wasn't received
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_HandlerReceivedMessage()
{   
    tBool b_ACKReceived = false;
    tUInt8 aui8_Data[MSG_LENGTH_ACK_MESSAGE_SERVER] = {0};
    tUInt8 aui8_CyclicMessage[MSG_LENGTH_CYCLIC_MESSAGE] = {0};
    String str_ACK ;
    tUInt8 ui8_TimerWaitForMessage = 0;
    tUInt32 ui32_NumbOfPrint = 0;

    if(b_IsRunning())
    {
        this->b_Tick();
        m_c_Logger.v_Print("WS: Timer tick\n", ui32_NumbOfPrint, true);
    }
    while((TIMEOUT_WAIT_FOR_SERVER_MESSAGE > ui8_TimerWaitForMessage)&& (m_rc_WiFiClient.available() == 0))
    {
        ui8_TimerWaitForMessage++;
        delay(10);
    }
    if( ui8_TimerWaitForMessage < TIMEOUT_WAIT_FOR_SERVER_MESSAGE)
    {
        m_c_Logger.v_Print("WS: Server available, message received\n");
    }
    else
    {
        m_c_Logger.v_Print("WS: Server not available\n");
    }

    str_ACK = m_rc_WiFiClient.readStringUntil('\r');

    if (0 < str_ACK.length())
    {
        v_MessageHandler(str_ACK, (MSG_LENGTH_CYCLIC_MESSAGE + MSG_LENGTH_ACK_MESSAGE_SERVER), &aui8_Data[0], &aui8_CyclicMessage[0]);
        tChar c_Debug[64];
        if((aui8_Data[0] == 0xC0) && (aui8_Data[2] == MSG_ID_PUMP_STATUS_MESSAGE))
        {
            
            if(aui8_Data[3] == 0)
            {
                sprintf(c_Debug, "WS: Rx[ACK] for ID:%.2X OK\n",aui8_Data[2]);
                b_ACKReceived = true;
                #ifdef TIME_MEASUREMENT
                tChar c_DebugTimeMeasurement[128];
                m_ui32_ElapsedTimeSendAndREceiveMsg = ui32_GetElapsedTime(m_ui32_OldElapsedTimeSendAndREceiveMsg);
                sprintf(c_DebugTimeMeasurement, "WS: Elapsed Time between send and ack receiving %d ms\n", m_ui32_ElapsedTimeSendAndREceiveMsg);
                m_c_Logger.v_Print(c_DebugTimeMeasurement);
                #endif
            }
            else
            {
                sprintf(c_Debug, "WS: Rx[ACK] for ID:%.2X NOK ErrorCode=%d\n",aui8_Data[2], aui8_Data[3]);
            }
        }
        else
        {
            sprintf(c_Debug, "WS: Received msg. don't match MSG ID= %.2X and ACK_MSG ID= %.2X\n",aui8_Data[0], aui8_Data[2]);
        }
        m_c_Logger.v_Print(c_Debug);
        // checks the incoming message id and hand over to WebInterface
        if(aui8_CyclicMessage[0] == 0xC5)
        {
            m_ui8_PumpActivationRequested = aui8_CyclicMessage[1];
            String str_CyclicMsg  = "";
            for(tUInt8 i = 0; i < sizeof(aui8_CyclicMessage); ++i)
            {
                str_CyclicMsg += String(aui8_CyclicMessage[i]);
                if(i < (sizeof(aui8_CyclicMessage) - 1))
                {
                    str_CyclicMsg += ".";
                }
            }
            m_c_Logger.v_Print("WS: Cyclic message = ");
            m_c_Logger.v_Print(str_CyclicMsg, false);
            m_c_Logger.v_Print("\n", false);
            tUInt8 ui8_Length = 0;
            // convert string in array buffer to send it out
            v_ConvertStringToArrayChar(str_CyclicMsg, '0', ui8_Length);
            if(m_b_ACKForSeq3Received)
            {
                Rte_Call_Write_Serial_Port_PT_ESP8266_Serial_Port(m_aui8_Data, ui8_Length, m_rc_SoftwareSerial_0);
                #ifdef TIME_MEASUREMENT_SERIAL_PORT
                m_ui32_ElapsedTimeSendUARTMsgAndRcvACK = millis();
                #endif
                delay(100);
                if(m_rc_SoftwareSerial_0.available() > 0)
                {
                    String c_ACKMsgSeq0 = m_rc_SoftwareSerial_0.readStringUntil('\r');
                    if(c_ACKMsgSeq0[0] == '1')
                    {
                        m_c_Logger.v_Print("WS: ACK receive for Seq0\n");
                        m_b_ACKForSeq3Received = false;
                        m_b_ACKForSeq0Received = true;
                    }
                    else
                    {
                        m_c_Logger.v_Print("WS: ACK missing for Seq0 value=");
                        m_c_Logger.v_Print(c_ACKMsgSeq0[0], false);
                        m_c_Logger.v_Print("\n", false);
                    }
                } 
                else
                {
                    m_c_Logger.v_Print("WS: Software serial 0 not available\n");
                }
                m_rc_SoftwareSerial_0.flush();
            }
            else
            {
                m_c_Logger.v_Print("WS: The acknowledge for sequence message number 3 is pending\n");
            }
            

            delay(100);
            // convert string in array buffer to send it out
            v_ConvertStringToArrayChar(str_CyclicMsg, '1', ui8_Length);
            if(m_b_ACKForSeq0Received)
            {
                Rte_Call_Write_Serial_Port_PT_ESP8266_Serial_Port(m_aui8_Data, ui8_Length, m_rc_SoftwareSerial_0);
                delay(100);
                if(m_rc_SoftwareSerial_0.available() > 0)
                {
                    String c_ACKMsgSeq1 = m_rc_SoftwareSerial_0.readStringUntil('\r');
                    if(c_ACKMsgSeq1[0] == '1')
                    {
                        m_c_Logger.v_Print("WS: ACK receive for Seq1\n");
                        m_b_ACKForSeq0Received = false;
                        m_b_ACKForSeq1Received = true;
                    }
                    else
                    {
                        m_c_Logger.v_Print("WS: ACK missing for Seq1 value=");
                        m_c_Logger.v_Print(c_ACKMsgSeq1[0], false);
                        m_c_Logger.v_Print("\n", false);
                    }
                } 
                else
                {
                    m_c_Logger.v_Print("WS: Software serial 0 not available\n");
                }
                m_rc_SoftwareSerial_0.flush();
            }
            else
            {
                m_c_Logger.v_Print("WS: The acknowledge for sequence message number 0 is pending\n");
            }
            
            
            delay(100);
            // convert string in array buffer to send it out
            v_ConvertStringToArrayChar(str_CyclicMsg, '2', ui8_Length);
            if(m_b_ACKForSeq1Received)
            {
                Rte_Call_Write_Serial_Port_PT_ESP8266_Serial_Port(m_aui8_Data, ui8_Length, m_rc_SoftwareSerial_1);
                delay(100);
                if(m_rc_SoftwareSerial_1.available() > 0)
                {
                    String str_RecMsg = m_rc_SoftwareSerial_1.readStringUntil('\r');
                    if(str_RecMsg[1] == '0')
                    {
                        if(str_RecMsg[2] == '1')
                        {
                            m_ui8_PumpActivatioReqViaWebInterface = PUMP_OPEN;
                            m_c_Logger.v_Print("WS: Pump activation requested\n");
                        }
                        else
                        {
                            m_ui8_PumpActivatioReqViaWebInterface = PUMP_CLOSE;
                            m_c_Logger.v_Print("WS: Pump deactivation requested\n");
                        }
                    }
                    if(str_RecMsg[0] == '1')
                    {
                        m_c_Logger.v_Print("WS: ACK receive for Seq2\n");
                        m_b_ACKForSeq1Received = false;
                        m_b_ACKForSeq2Received = true;
                    }
                    else
                    {
                        m_c_Logger.v_Print("WS: ACK missing for Seq2 value=");
                        m_c_Logger.v_Print(str_RecMsg[0], false);
                        m_c_Logger.v_Print("\n", false);
                    }
                } 
                else
                {
                    m_c_Logger.v_Print("WS: Software serial 1 not available\n");
                }
                m_rc_SoftwareSerial_1.flush();
            }
            else
            {
                m_c_Logger.v_Print("WS: The acknowledge for sequence message number 1 is pending\n");
            }
            delay(100);
            // convert string in array buffer to send it out
            v_ConvertStringToArrayChar(str_CyclicMsg, '3', ui8_Length);
            if(m_b_ACKForSeq2Received)
            {
                Rte_Call_Write_Serial_Port_PT_ESP8266_Serial_Port(m_aui8_Data, ui8_Length, m_rc_SoftwareSerial_1);
                delay(100);
                if(m_rc_SoftwareSerial_1.available() > 0)
                {
                    String str_RecMsg = m_rc_SoftwareSerial_1.readStringUntil('\r');
                    if(str_RecMsg[1] == '0')
                    {
                        if(str_RecMsg[2] == '1')
                        {
                            m_ui8_PumpActivatioReqViaWebInterface = PUMP_OPEN;
                            m_c_Logger.v_Print("WS: Pump activation requested\n");
                        }
                        else
                        {
                            m_ui8_PumpActivatioReqViaWebInterface = PUMP_CLOSE;
                            m_c_Logger.v_Print("WS: Pump deactivation requested\n");
                        }
                    }
                    if(str_RecMsg[0] == '1')
                    {
                        m_c_Logger.v_Print("WS: ACK receive for Seq3", true);
                        m_b_ACKForSeq2Received = false;
                        m_b_ACKForSeq3Received = true;
                        #ifdef TIME_MEASUREMENT_SERIAL_PORT
                        m_ui32_ElapsedTimeSendUARTMsgAndRcvACK = ui32_GetElapsedTime(m_ui32_OldElapsedTimeSendUARTMsgAndRcvACK);
                        tChar c_Debug[128];
                        sprintf(c_Debug, "WS: Elapsed Time send msg and recv all the ack via serial port %dms\n", m_ui32_ElapsedTimeSendUARTMsgAndRcvACK);
                        m_c_Logger.v_Print(c_Debug);
                        #endif
                    }
                    else
                    {
                        m_c_Logger.v_Print("WS: ACK missing for Seq3 value=");
                        m_c_Logger.v_Print(str_RecMsg[0], false);
                        m_c_Logger.v_Print("\n", false);
                    }
                } 
                else
                {
                    m_c_Logger.v_Print("WS: Software serial 1 not available\n");
                }
                m_rc_SoftwareSerial_1.flush();
            }
            else
            {
                m_c_Logger.v_Print("WS: The acknowledge for sequence message number 2 is pending\n");
            }
        }
        else
        {
            sprintf(c_Debug, "WS: No Handler for MSG ID= %.2X\n",aui8_CyclicMessage[0]);
        }
    }

    return b_ACKReceived;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 23.04.2023
----------------------------------------------------------------
@Function Description:  convert a decimal value to his char
----------------------------------------------------------------
@parameter: tUInt8 ui8_Number
@Returnvalue: char of decimal value
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tChar c_ConvertIntToChar(tUInt8 ui8_Number)
{
    return ui8_Number + 48;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 23.04.2023
----------------------------------------------------------------
@Function Description:  convert a decimal value to his char
----------------------------------------------------------------
@parameter: tUInt8 ui8_Number
@Returnvalue: char of decimal value
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_PumpActivationRequestedViaWebInterface()
{
    return static_cast<tBool>(m_ui8_PumpActivatioReqViaWebInterface);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 23.04.2023
----------------------------------------------------------------
@Function Description:  The function extracts the message that
                     matches the given sequence number.
----------------------------------------------------------------
@parameter: String s, tChar ui8_SeqNr, tUInt8& ui8_length
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_ConvertStringToArrayChar(String s, tChar ui8_SeqNr, tUInt8& ui8_length)
{
  tChar charArray[s.length()];
  uint8_t aui8_IndexPosition[MAX_NUMB_DOT];    // array to store the position of char '.'
  uint8_t ui8_currentPosition = 0;
  uint8_t ui8_Size = 0;

  // Initialize the array
  memset(m_aui8_Data, 0, sizeof(m_aui8_Data));

  /*
    Copy the whole content of the string into in char array and 
    save the posiiton on the delimiter '.' in an another array
  */
  for(unsigned int i = 0; i < sizeof(charArray); i++)
  {
    charArray[i] = s.charAt(i);
    if(charArray[i] == '.')
    {
      aui8_IndexPosition[ui8_currentPosition++] = i;
    }
  }
  
  // calculate the size to the given sequence number
  if(ui8_SeqNr == '0')
  {
    ui8_Size = aui8_IndexPosition[4] - aui8_IndexPosition[0];
  }
  else if(ui8_SeqNr == '1')
  {
    ui8_Size = aui8_IndexPosition[8] - aui8_IndexPosition[4];
  }
  else if(ui8_SeqNr == '2')
  {
    ui8_Size = aui8_IndexPosition[12] - aui8_IndexPosition[8];
  }
  else if(ui8_SeqNr == '3')
  {
    ui8_Size = sizeof(charArray) - aui8_IndexPosition[12];
  }
  else
  {
    m_c_Logger.v_Print("WS: No Valid Sequence number\n");
  }

  /*
    Copies the extracted message for each sequence number in the global field.
    The minimum length for the first 3 sequences is greater than 9, 
    so the original size is increased by 4
  */
  if(ui8_SeqNr == '0')
  {
    tUInt8 ui8_SizetoCopy = ui8_Size;
    if(ui8_Size > 7 && ui8_Size < 15)
    {
      ui8_Size += LENGTH_PREFIX_SEQUENCENUMBER0TO2;
      tChar pui8_SeqMsg[ui8_Size];
      tChar c_Size[LENGTH_PREFIX_SEQUENCENUMBER0TO2];
      tUInt8 ui8_Pos0 = ui8_Size / 10;
      tUInt8 ui8_Pos1 =  ui8_Size - ui8_Pos0*10;
      c_Size[0] = c_ConvertIntToChar(ui8_Pos0);
      c_Size[1] = c_ConvertIntToChar(ui8_Pos1);
      c_Size[2] = '.';
      c_Size[3] = ui8_SeqNr;
      memcpy(pui8_SeqMsg, c_Size, sizeof(c_Size));
      memcpy(pui8_SeqMsg+sizeof(c_Size), charArray+aui8_IndexPosition[0], ui8_SizetoCopy);
      memcpy(m_aui8_Data, pui8_SeqMsg, sizeof(pui8_SeqMsg));
    }
    else
    {
      m_c_Logger.v_Print("WS: Seq0 length too big: ");
      Serial.println(ui8_SizetoCopy);
    }
    ui8_length = ui8_Size;
  }
  else if(ui8_SeqNr == '1')
  {
    tUInt8 ui8_SizetoCopy = ui8_Size;
    if(ui8_Size > 7 && ui8_Size < 13)
    {
      ui8_Size += LENGTH_PREFIX_SEQUENCENUMBER0TO2;
      tChar pui8_SeqMsg[ui8_Size];
      tChar c_Size[LENGTH_PREFIX_SEQUENCENUMBER0TO2];
      tUInt8 ui8_Pos0 = ui8_Size / 10;
      tUInt8 ui8_Pos1 =  ui8_Size - ui8_Pos0*10;
      c_Size[0] = c_ConvertIntToChar(ui8_Pos0);
      c_Size[1] = c_ConvertIntToChar(ui8_Pos1);
      c_Size[2] = '.';
      c_Size[3] = ui8_SeqNr;
      memcpy(pui8_SeqMsg, c_Size, sizeof(c_Size));
      memcpy(pui8_SeqMsg+sizeof(c_Size), charArray+aui8_IndexPosition[4], ui8_SizetoCopy);
      memcpy(m_aui8_Data, pui8_SeqMsg, sizeof(pui8_SeqMsg));
    }
    else
    {
      m_c_Logger.v_Print("WS: Seq1 length too big: ");
      Serial.println(ui8_SizetoCopy);
    }
    ui8_length = ui8_Size;
  }
  else if(ui8_SeqNr == '2')
  {
    tUInt8 ui8_SizetoCopy = ui8_Size;
    if(ui8_Size > 7 && ui8_Size < 17)
    {
      ui8_Size += LENGTH_PREFIX_SEQUENCENUMBER0TO2;
      tChar pui8_SeqMsg[ui8_Size];
      tChar c_Size[LENGTH_PREFIX_SEQUENCENUMBER0TO2];
      tUInt8 ui8_Pos0 = ui8_Size / 10;
      tUInt8 ui8_Pos1 =  ui8_Size - ui8_Pos0*10;
      c_Size[0] = c_ConvertIntToChar(ui8_Pos0);
      c_Size[1] = c_ConvertIntToChar(ui8_Pos1);
      c_Size[2] = '.';
      c_Size[3] = ui8_SeqNr;
      memcpy(pui8_SeqMsg, c_Size, sizeof(c_Size));
      memcpy(pui8_SeqMsg+sizeof(c_Size), charArray+aui8_IndexPosition[8], ui8_SizetoCopy);
      memcpy(m_aui8_Data, pui8_SeqMsg, sizeof(pui8_SeqMsg));
    }
    else
    {
      m_c_Logger.v_Print("WS: Seq2 length too big: ");
      Serial.println(ui8_SizetoCopy);
    }
    ui8_length = ui8_Size;
  }
  else if(ui8_SeqNr == '3')
  {
    tUInt8 ui8_SizetoCopy = ui8_Size;
    if(ui8_Size > 7 && ui8_Size < 13)
    {
      ui8_Size += LENGTH_PREFIX_SEQUENCENUMBER0TO2;
      tChar pui8_SeqMsg[ui8_Size];
      tChar c_Size[LENGTH_PREFIX_SEQUENCENUMBER0TO2];
      tUInt8 ui8_Pos0 = ui8_Size / 10;
      tUInt8 ui8_Pos1 =  ui8_Size - ui8_Pos0*10;
      c_Size[0] = c_ConvertIntToChar(ui8_Pos0);
      c_Size[1] = c_ConvertIntToChar(ui8_Pos1);
      c_Size[2] = '.';
      c_Size[3] = ui8_SeqNr;
      memcpy(pui8_SeqMsg, c_Size, sizeof(c_Size));
      memcpy(pui8_SeqMsg+sizeof(c_Size), charArray+aui8_IndexPosition[12], ui8_SizetoCopy);
      memcpy(m_aui8_Data, pui8_SeqMsg, sizeof(pui8_SeqMsg));
    }
    else if(ui8_Size == 6)
    {
      ui8_Size += LENGTH_PREFIX_SEQUENCENUMBER3;
      tChar pui8_SeqMsg[ui8_Size];
      tChar c_Size[LENGTH_PREFIX_SEQUENCENUMBER3];
      tUInt8 ui8_Pos0 = ui8_Size ;
      c_Size[0] = c_ConvertIntToChar(ui8_Pos0);
      c_Size[1] = '.';
      c_Size[2] = ui8_SeqNr;
      
      memcpy(pui8_SeqMsg, c_Size, sizeof(c_Size));
      memcpy(pui8_SeqMsg+sizeof(c_Size), charArray+aui8_IndexPosition[12], ui8_SizetoCopy);
      memcpy(m_aui8_Data, pui8_SeqMsg, sizeof(pui8_SeqMsg));
    }
    else
    {
      m_c_Logger.v_Print("WS: Seq3 length too big: ");
      Serial.println(ui8_SizetoCopy);
    }
    ui8_length = ui8_Size;
  }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  the function reads the message comming
from the server and save into 2 arrays. One for the ACK and the 
another one for the cyclic message
----------------------------------------------------------------
@parameter: String str_Ack, tUInt8 ui8_MessageLength, 
            tUInt8* pACKMsg, tUInt8* pCyclicMessage
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_MessageHandler(String str_Ack, tUInt8 ui8_MessageLength, tUInt8* pACKMsg, tUInt8* pCyclicMessage)
{
    tUInt8 aui8_DataToWrite[MSG_LENGTH_CYCLIC_MESSAGE + MSG_LENGTH_ACK_MESSAGE_SERVER] = {0}; 
    String str_Current;
    tUInt8 ui8_StrACKLength = str_Ack.length();
    tUInt8 ui8_LastPosition = 0;
    tUInt8 ui8_PositionToWrite = 0;

    for(unsigned int i = 0; i < ui8_StrACKLength; i++)
    {
        tUInt8 ui8_CurrentPosition = i;
        if(str_Ack.charAt(i) == '.')
        {
            str_Current = str_Ack.substring(ui8_LastPosition, ui8_CurrentPosition);
            ui8_LastPosition = ui8_CurrentPosition + 1;
            if(ui8_PositionToWrite < ui8_MessageLength)
            {
                aui8_DataToWrite[ui8_PositionToWrite++] = str_Current.toInt();
            }
            else
            {
                break;
            }
        }
    }
    aui8_DataToWrite[ui8_PositionToWrite] = str_Ack.substring(ui8_LastPosition).toInt();
    memcpy(pACKMsg, aui8_DataToWrite, MSG_LENGTH_ACK_MESSAGE_SERVER);
    memcpy(pCyclicMessage, aui8_DataToWrite + MSG_LENGTH_ACK_MESSAGE_SERVER, MSG_LENGTH_CYCLIC_MESSAGE);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  check the connection to the server and 
    when the connection is lost try 5 times to reconnect the client
    to the server
----------------------------------------------------------------
@parameter: --
@Returnvalue:   True -- The client is connected to the server
                False -- The client isn't connected to the server
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_ClientIsRunning( tBool& b_FlagtoReset)
{
    tBool b_IsRunning = true;
    if (!b_ServerConnected()) 
    {
        b_FlagtoReset = false;
        tUInt8 ui8_NumbConnAttempts = 0;
        tUInt32 ui32_PrintNumb = 0;
        while(!m_rc_WiFiClient.connect(host, port) && (ui8_NumbConnAttempts < MAX_SERVERCONNECTION_ATTEMPTS)) 
        {
            m_c_Logger.v_Print("WS: Connection to Server lost, try to connect...\n", ui32_PrintNumb);
            ui8_NumbConnAttempts++;
        }
        b_IsRunning = (ui8_NumbConnAttempts < MAX_SERVERCONNECTION_ATTEMPTS) ? true : false;
    }
    else 
    {
        b_IsRunning = true;
    }
    return b_IsRunning;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  Get if the pump activation is requested
----------------------------------------------------------------
@parameter: -
@Returnvalue:   0x00 -- Pump activation is not requested
                0x01 -- Pump activation is requested
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tUInt8 ui8_GetPumpActivationRequested()
{
    return m_ui8_PumpActivationRequested;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  reset the m_ui8_PumpActivationRequested
----------------------------------------------------------------
@parameter: -
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_PumpActivationRequestedReset()
{
    m_ui8_PumpActivationRequested = PUMP_CLOSE;
    m_ui8_PumpActivatioReqViaWebInterface = PUMP_CLOSE;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  check if the server is connected 
                        to the client
----------------------------------------------------------------
@parameter: -
@Returnvalue:   True -- Client is connected
                False -- Client is not connected
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_ServerConnected()
{
    return m_rc_WiFiClient.connected();
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  try to connect client to WiFi
----------------------------------------------------------------
@parameter: --
@Returnvalue:   True -- Connection to Wifi established
                False -- Connection to Wifi not established
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_StartWiFiConnection()
{
    tBool b_RetVal = false;
    tUInt32 ui32_ElapsedTimeStartConnection = 0;
    while((m_c_WiFiHandler.rc_GetESP8266WiFiClass().status() != WL_CONNECTED) && (ui32_ElapsedTimeStartConnection < TIME_OUT_CONNECTION_WIFI))
    {
        ui32_ElapsedTimeStartConnection = millis();
        delay(10);
    }
    m_c_Logger.v_Print(" ", true);
    if(ui32_ElapsedTimeStartConnection < TIME_OUT_CONNECTION_WIFI)
    {
        b_RetVal = true;
        m_c_Logger.v_Print("WS: Connected to WiFi\n");
    }
    else
    {
        m_c_Logger.v_Print("WS: Connection to WiFi failed\n");
    }
    return b_RetVal;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  Reset all Acknowledges
----------------------------------------------------------------
@parameter: --
@Returnvalue: --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_Reset()
{
    m_b_ACKForSeq0Received = false;
    m_b_ACKForSeq1Received = false;
    m_b_ACKForSeq2Received = false;
    m_b_ACKForSeq3Received = true;
}

private:
    tChar* m_str_ssid ;
    tChar* m_str_passwd ;
    const tChar* host = "192.168.1.200";
    const tUInt16 port = 50011;
    WiFiClient& m_rc_WiFiClient;
    IPAddress m_c_local_IP; 
    IPAddress m_c_gateway; 
    IPAddress m_c_subnet;
    tUInt8 m_ui8_PumpActivationRequested;
    tUInt8 m_ui8_PumpActivatioReqViaWebInterface;
    WiFiHandler m_c_WiFiHandler;
    tBool m_b_ACKForSeq0Received;
    tBool m_b_ACKForSeq1Received;
    tBool m_b_ACKForSeq2Received;
    tBool m_b_ACKForSeq3Received;
    SoftwareSerial& m_rc_SoftwareSerial_0;
    SoftwareSerial& m_rc_SoftwareSerial_1;
    tChar m_aui8_Data[MSG_LENGTH_CYCLIC_MESSAGE + MSG_LENGTH_ACK_MESSAGE_SERVER];
    #ifdef TIME_MEASUREMENT_SERIAL_PORT
    tUInt32 m_ui32_ElapsedTimeSendUARTMsgAndRcvACK;
    tUInt32 m_ui32_OldElapsedTimeSendUARTMsgAndRcvACK;
    #endif
    #ifdef TIME_MEASUREMENT
	tUInt32 m_ui32_ElapsedTimeSendAndREceiveMsg;
    tUInt32 m_ui32_OldElapsedTimeSendAndREceiveMsg;
	#endif

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
};


#endif //WIFI_SERVER_H
