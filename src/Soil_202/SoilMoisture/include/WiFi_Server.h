#ifndef WIFI_SERVER_H
#define WIFI_SERVER_H
#include "Logger.h"
#include "ESP8266WiFi.h"

#define TEST


class WiFiHandler
{
public: 
    WiFiHandler(ESP8266WiFiClass& rc_ESP8266WiFiClass)
    :m_rc_ESP8266WiFiClass(rc_ESP8266WiFiClass)
    {

    }

    ESP8266WiFiClass rc_GetESP8266WiFiClass()
    {
        return m_rc_ESP8266WiFiClass;
    }

private:
    ESP8266WiFiClass& m_rc_ESP8266WiFiClass;
};


class WiFi_Server
{
public:
static Logger m_c_Logger;
WiFi_Server(WiFiClient& rc_WiFiClient, uint8 ui8_FourthOctet, tChar* passwd, tChar* ssid, ESP8266WiFiClass& rc_ESP8266WiFiClass)
: m_str_ssid(ssid)
, m_str_passwd(passwd)
, m_rc_WiFiClient(rc_WiFiClient)
, m_b_IsConnected(false)
, m_c_local_IP(192,168,1,ui8_FourthOctet)
, m_c_gateway(192,168,1,1)
, m_c_subnet(255,255,255,0)
, m_c_WiFiHandler(rc_ESP8266WiFiClass)
#ifdef TIME_MEASUREMENT
, m_ui32_ElapsedTimeSendAndRcvACK(0)
, m_ui32_OldElapsedTimeSendAndRcvACK(0)
#endif
{
    
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
    m_c_Logger.v_Init();

    m_c_WiFiHandler.rc_GetESP8266WiFiClass().mode(WIFI_STA);
    m_c_WiFiHandler.rc_GetESP8266WiFiClass().begin(m_str_ssid, m_str_passwd);
    if (!m_c_WiFiHandler.rc_GetESP8266WiFiClass().config(m_c_local_IP, m_c_gateway, m_c_subnet)) 
    {
        m_c_Logger.v_Print("WS: STA Failed to configured\n", true);
        b_ServerIsRunning = false;
    } 

    // Waiting to connect to the WiFi
    m_c_Logger.v_Print("WS: Try to connect to WiFi\n", true);
    tUInt8 ui8_ResetCounter = 0;
    while((ui8_ResetCounter < 5) && !b_StartWiFiConnection())
    {
        ui8_ResetCounter++;
        m_c_Logger.v_Print("WS: tried to start ");
        m_c_Logger.v_Print(String(ui8_ResetCounter), false);
        m_c_Logger.v_Print(" times\n", false);
    }
    WiFi_Server::m_c_Logger.v_Print("WS: IP-Address:");
    Serial.println(m_c_local_IP);
    // Waiting to connect to the server 5S seconds
    while((!m_rc_WiFiClient.connect(host, port)) && (ui8_Counter < TIME_OUT_CONNECTION_SERVER)) 
    {
        ui8_Counter++;
        m_c_Logger.v_Print("WS: try to connect to the server\n", true);
    }
    if(ui8_Counter < TIME_OUT_CONNECTION_SERVER)
    {
        b_ServerIsRunning = true;
        m_b_IsConnected = true;
        m_c_Logger.v_Print("WS: Connected to server\n", true);
    }
    else
    {
        m_c_Logger.v_Print("WS: Timeout\n", true);
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
        char c_Debug[64];
        m_rc_WiFiClient.print(str_Data);
        b_MessageWasSent = true;
        #ifdef TIME_MEASUREMENT
        m_ui32_OldElapsedTimeSendAndRcvACK = millis();
        #endif
        sprintf(c_Debug, "[TX] ID=%.2X OK, PumpActivationReq=%d, Mois Value=%d\n", p_aui8_Data[0], p_aui8_Data[1], p_aui8_Data[2]);
        m_c_Logger.v_Print(c_Debug, true);
    }
    return b_MessageWasSent;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  ACK Handler 
----------------------------------------------------------------
@parameter: --
@Returnvalue: --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_HandlerReceivedMessage()
{   
    tBool b_ACKReceived = false;
    tUInt8 aui8_Data[MSG_LENGHT_ACK_MESSAGE_SERVER] = {0};
    String str_ACK ;
    tUInt8 ui8_TimerWaitForMessage = 0;
  
    while((TIMEOUT_WAIT_FOR_SERVER_MESSAGE > ui8_TimerWaitForMessage)&& (m_rc_WiFiClient.available() == 0))
    {
        ui8_TimerWaitForMessage++;
        delay(10);
    }
    str_ACK = m_rc_WiFiClient.readStringUntil('\r');
    v_ReadAcknowledge(str_ACK, MSG_LENGHT_ACK_MESSAGE_SERVER, &aui8_Data[0]);
    if((aui8_Data[0] == 0xC0) && (aui8_Data[2] == MSG_ID_SOIL_MOISTURE_MESSAGE))
    {
        char c_Debug[64];
        if(aui8_Data[3] == 0)
        {
            sprintf(c_Debug, "WS: Rx[ACK] for ID:%.2X OK\n",aui8_Data[2]);
            b_ACKReceived = true;
            #ifdef TIME_MEASUREMENT
            m_ui32_ElapsedTimeSendAndRcvACK = ui32_GetElapsedTime(m_ui32_OldElapsedTimeSendAndRcvACK);
            m_c_Logger.v_Print("WS: Elapsed time send message and receive acknowldge from server ");
            m_c_Logger.v_Print(m_ui32_ElapsedTimeSendAndRcvACK, false);
            m_c_Logger.v_Print("ms\n", false);
            #endif
        }
        else
        {
            sprintf(c_Debug, "WS: Rx[ACK] for ID:%.2X NOK ErrorCode=%d\n",aui8_Data[2], aui8_Data[3]);
        }
        m_c_Logger.v_Print(c_Debug, true);
    }
    else if(aui8_Data[0] == 0xC5)
    {
        WiFi_Server::m_c_Logger.v_Print("WS: ACK received for 0xC5\n");
    }
    else if(str_ACK.length()== 0)
    {
        WiFi_Server::m_c_Logger.v_Print("WS: Incoming message length is zero\n");
    }
    else
    {
        m_c_Logger.v_Print("WS: no ACK for this ID: ");
        m_c_Logger.v_PrintHex(aui8_Data[0]);
        m_c_Logger.v_Print("\n", false);
    }

    return b_ACKReceived;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  read acknowldege 
----------------------------------------------------------------
@parameter: --
@Returnvalue: a pointer to array
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_ReadAcknowledge(String str_Ack, tUInt8 ui8_MessageLength, tUInt8* pDest)
{
    tUInt8 aui8_DataToWrite[MSG_LENGTH_ACK_MESSAGE_SERVER] = {0}; 
    String str_Current;

    tUInt8 ui8_LastPosition = 0;
    
    tUInt8 ui8_PositionToWrite = 0;
    for(unsigned int i = 0; i < str_Ack.length(); i++)
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
    str_Current = str_Ack.substring(ui8_LastPosition);
    aui8_DataToWrite[ui8_PositionToWrite++] = str_Current.toInt();
    
    memcpy(pDest, aui8_DataToWrite, ui8_MessageLength);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  
----------------------------------------------------------------
@parameter: --
@Returnvalue: 
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_ClientIsRunning()
{
    tBool b_IsRunning = true;
    if (!b_ServerConnected()) 
    {
        tUInt8 ui8_NumbConnAttempts = 0;
        while(!m_rc_WiFiClient.connect(host, port) && (ui8_NumbConnAttempts < MAX_SERVERCONNECTION_ATTEMPTS)) 
        {
            m_c_Logger.v_Print("WS: Connection lost, try to connect...\n", true);
            ui8_NumbConnAttempts++;
            delay(200);
        }
        b_IsRunning = (ui8_NumbConnAttempts < MAX_SERVERCONNECTION_ATTEMPTS) ? true : false;
    }
    else {
        b_IsRunning = true;
    }
    return b_IsRunning;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  
----------------------------------------------------------------
@parameter: --
@Returnvalue: 
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_ServerConnected()
{
    return m_rc_WiFiClient.connected();
}



private:
    

    tChar* m_str_ssid ;
    tChar* m_str_passwd ;
    const tChar* host = "192.168.1.200";
    const tUInt16 port = 50011;
    WiFiClient& m_rc_WiFiClient;
    tBool m_b_IsConnected;
    IPAddress m_c_local_IP; 
    IPAddress m_c_gateway; 
    IPAddress m_c_subnet;
    WiFiHandler m_c_WiFiHandler;
    #ifdef TIME_MEASUREMENT
    tUInt32 m_ui32_ElapsedTimeSendAndRcvACK;
    tUInt32 m_ui32_OldElapsedTimeSendAndRcvACK;
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

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author:    Gerald Emvoutou | Digital Transformation & Technology
                                    Alliance
    @Creation: 26.03.2023
    ----------------------------------------------------------------
    @Function Description:  
    ----------------------------------------------------------------
    @parameter: --
    @Returnvalue: 
    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    tBool b_StartWiFiConnection()
    {
        tBool b_RetVal = false;
        tUInt32 ui32_ElapsedTimeStartConnection = 0;
        while((m_c_WiFiHandler.rc_GetESP8266WiFiClass().status() != WL_CONNECTED) && (ui32_ElapsedTimeStartConnection < TIME_OUT_CONNECTION_WIFI))
        {
            ui32_ElapsedTimeStartConnection = millis();
            delay(50);
        }
        m_c_Logger.v_Print(".",false);
        if(ui32_ElapsedTimeStartConnection < TIME_OUT_CONNECTION_WIFI)
        {
            b_RetVal = true;
            m_c_Logger.v_Print("WS: Connected to WiFi\n", true);
        }
        return b_RetVal;
    }
};


#endif //WIFI_SERVER_H
