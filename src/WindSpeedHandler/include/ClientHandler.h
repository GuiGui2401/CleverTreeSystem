/**
*@Copyright DTTA Maker Factory Tech. All Rights reserved.
*It is not allowed, whether to copy this document, or to pass it even in extracts to others, without the approval DTTA Maker Factory Tech.
*All copyrights for this document are the exclusive property of DTTA Maker Factory Tech.
*/ 
/*----------------------------------------------------------------------*/
#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H
/**
*@file: ClientHandler.h
*@Author: Guiyoba, Jonatan <jguiyo@gmail.com> 
*@date: 29/06/2023 13:03:14
 *//*----------------------------------------------------------------------*/
 
/*-----------------------------includes---------------------------------*/
/* C system headers */ 
/* C++ standard library headers */ 
/* External libraires headers */ 
/* Arduino libraries headers */ 
#include "ESP8266WiFi.h"
#include "additional_types.h"
 
/*-------------------------global defines-------------------------------*/
 
class ClientHandler
{ 

private:
	ESP8266WiFiClass&   m_rc_ESP8266WiFiClass;
    WiFiClient&         m_rc_WiFiClient;
    IPAddress           m_c_local_IP; 
    IPAddress           m_c_gateway; 
    IPAddress           m_c_subnet;
    const tChar*              m_str_ssid;
    const tChar*              m_str_passwd;


    const tChar* host = "192.168.1.200";
    const tUInt16 port = 50011;

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Guiyoba Jonathan
    @Create Date: 29/06/2023
    ---------------------------------------------------------------------
    @Function description: start the WiFi connection
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value: 	True -- connection to WiFi estbalished
					False -- connection to WiFi not estbalished
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tBool b_StartWiFiConnection( )
    {
		tBool b_ConnectedToWiFi = false;
        tUInt32 ui32_ElapsedTime = 0;
        // Start connection to WiFi
        while((m_rc_ESP8266WiFiClass.status() != WL_CONNECTED) && (ui32_ElapsedTime < TIME_OUT_CONNECTION_WIFI))
        {
            ui32_ElapsedTime = millis();
			delay(10);
        }
        if(ui32_ElapsedTime < TIME_OUT_CONNECTION_WIFI)
        {
            b_ConnectedToWiFi = true;
        }
        return b_ConnectedToWiFi;
	}
 
public :

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Guiyoba Jonathan
    @Create Date: 29/06/2023
    ---------------------------------------------------------------------
    @Function description: constructor
    ---------------------------------------------------------------------
    @Parameter: ESP8266WiFiClass& rc_ESP8266WiFiClass, WiFiClient& rc_WiFiClient
    @Return value: -
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	ClientHandler(ESP8266WiFiClass& rc_ESP8266WiFiClass, WiFiClient& rc_WiFiClient)
    : m_rc_ESP8266WiFiClass(rc_ESP8266WiFiClass)
    , m_rc_WiFiClient(rc_WiFiClient)
    , m_c_local_IP(192,168,1,211)
    , m_c_gateway(192,168,1,1)
    , m_c_subnet(255,255,255,0)
    , m_str_ssid("WiFi_CleverTree")
    , m_str_passwd("02655826")
	{

	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Guiyoba Jonathan
    @Create Date: 29/06/2023
    ---------------------------------------------------------------------
    @Function description: destructor
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value: -
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	~ClientHandler() 
	{

	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Guiyoba Jonathan
    @Create Date: 24/06/2023
    ---------------------------------------------------------------------
    @Function description: check if the server is connected 
                        to the client
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value: 	True -- Client is connected
                	False -- Client is not connected
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tBool b_IsClientRunning()
    {
		tBool b_IsRunning = false;
		if (!m_rc_WiFiClient.connected()) 
		{
			tUInt8 ui8_NumbConnAttempts = 0;
			while(!m_rc_WiFiClient.connect(host, port) && (ui8_NumbConnAttempts < MAX_SERVERCONNECTION_ATTEMPTS)) 
			{
				ui8_NumbConnAttempts++;
				delay(200);
			}
			b_IsRunning = (ui8_NumbConnAttempts < MAX_SERVERCONNECTION_ATTEMPTS) ? true : false;
		}
		else 
		{
			b_IsRunning = true;
		}
		return b_IsRunning;
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Guiyoba Jonathan
    @Create Date: 29/06/2023
    ---------------------------------------------------------------------
    @Function description: read the acknowledgment
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value: 	True -- Acknowledge received
					False -- Acknowledge not received
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tBool b_HandlerReceivedMessage()
    {
		tBool b_ACKReceived = false;
		tUInt8 aui8_RecvMsg[MSG_LENGTH_ACK_MESSAGE_SERVER];
		String str_RecvMsg = m_rc_WiFiClient.readStringUntil('\r');
		v_MessageHandler(str_RecvMsg, sizeof(aui8_RecvMsg), aui8_RecvMsg);
		tChar c_Debug[50];
		if((aui8_RecvMsg[0] == 0xC0) && (aui8_RecvMsg[2] == MSG_ID_WINDSPEED_MESSAGE))
		{
			if(aui8_RecvMsg[3] == 0)
			{
				sprintf(c_Debug, "CH: Rx[ACK] for ID:%.2X OK",aui8_RecvMsg[2]);
				b_ACKReceived = true;
			}
			else
			{
				sprintf(c_Debug, "CH: Rx[ACK] for ID:%.2X NOK ErrorCode=%d",aui8_RecvMsg[2], aui8_RecvMsg[3]);
			}
		}
		else
		{
			sprintf(c_Debug, "CH: No Handler for ID:%.2X",aui8_RecvMsg[0]);
		}
		Serial.println(c_Debug);
		return b_ACKReceived;
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Guiyoba Jonathan
    @Create Date: 29/06/2023
    ---------------------------------------------------------------------
    @Function description: send message to the server
    ---------------------------------------------------------------------
    @Parameter: const tUInt8* p_aui8_Data, tUInt8 ui8_DataLength
    @Return value:  True --  message was send successful to the server
					False -- message can't be send to server
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
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
			tChar c_Debug[100];
			m_rc_WiFiClient.print(str_Data);
			b_MessageWasSent = true;
			sprintf(c_Debug, "CH: [TX] ID=%.2X OK  WindSpeed=%d", 	
						p_aui8_Data[0], p_aui8_Data[1]);
			Serial.println(c_Debug);
		}
		return b_MessageWasSent;
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Guiyoba Jonathan
    @Create Date: 29/06/2023
    ---------------------------------------------------------------------
    @Function description: Handle the receive message 
    ---------------------------------------------------------------------
    @Parameter: String str_RecvMsg, tUInt8 ui8_MessageLength, tUInt8* pACKMsg
    @Return value: -
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tVoid v_MessageHandler(String str_RecvMsg, tUInt8 ui8_MessageLength, tUInt8* pACKMsg)
    {
		tUInt8 aui8_DataToWrite[MSG_LENGTH_ACK_MESSAGE_SERVER] = {0}; 
		String str_Current;
		tUInt8 ui8_StrACKLength = str_RecvMsg.length();
		tUInt8 ui8_LastPosition = 0;
		tUInt8 ui8_PositionToWrite = 0;

		for(unsigned int i = 0; i < ui8_StrACKLength; i++)
		{
			tUInt8 ui8_CurrentPosition = i;
			if(str_RecvMsg.charAt(i) == '.')
			{
				str_Current = str_RecvMsg.substring(ui8_LastPosition, ui8_CurrentPosition);
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
		aui8_DataToWrite[ui8_PositionToWrite] = str_RecvMsg.substring(ui8_LastPosition).toInt();
		memcpy(pACKMsg, aui8_DataToWrite, MSG_LENGTH_ACK_MESSAGE_SERVER);
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Guiyoba Jonathan
    @Create Date: 29/06/2023
    ---------------------------------------------------------------------
    @Function Description:  start the connection to the server
	----------------------------------------------------------------
	@parameter: --
	@Returnvalue:   True -- Connection to server established
					False -- Connection to server not established
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tBool b_StartServerConnection()
    {
		Serial.begin(BAUDRATE);
		tBool b_ServerIsRunning = false;
		tUInt8 ui8_Counter = 0;
		m_rc_ESP8266WiFiClass.mode(WIFI_STA);
		m_rc_ESP8266WiFiClass.begin(m_str_ssid, m_str_passwd);
		if (!m_rc_ESP8266WiFiClass.config(m_c_local_IP, m_c_gateway, m_c_subnet)) 
		{
			b_ServerIsRunning = false;
		} 

		// Waiting to connect to the WiFi
		tUInt8 ui8_ResetCounter = 0;
		while((ui8_ResetCounter < 5) && !b_StartWiFiConnection())
		{
			ui8_ResetCounter++;
			delay(10);
		}
		
		// Waiting to connect to the server 5S seconds
		while((!m_rc_WiFiClient.connect(host, port)) && (ui8_Counter < TIME_OUT_CONNECTION_SERVER)) 
		{
			ui8_Counter++;
			delay(500);
		}
		if(ui8_Counter < TIME_OUT_CONNECTION_SERVER)
		{
			b_ServerIsRunning = true;
		}
		else
		{

		}
		return b_ServerIsRunning;
	}

};
#endif //CLIENTHANDLER_H