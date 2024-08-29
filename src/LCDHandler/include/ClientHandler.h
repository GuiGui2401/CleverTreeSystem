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
*@Author: Emvoutou, Cezy <cezy.emvoutou@dta-alliance.com> 
*@date: 24/06/2023 09:11:25
 *//*----------------------------------------------------------------------*/
 
/*-----------------------------includes---------------------------------*/
/* C system headers */ 
/* C++ standard library headers */ 
/* External libraires headers */ 
/* Arduino libraries headers */ 
#include "ESP8266WiFi.h"
#include "additional_types.h"
#include <LiquidCrystal_I2C.h>
#include "Logger.h"
 
/*-------------------------global defines-------------------------------*/
 
class ClientHandler
{ 

struct S_Measurements
{
	tUInt8 ui8_PumpActivationRequested;
	tUInt8 ui8_SoilMoisture_01;
	tUInt8 ui8_SoilMoisture_02;
	tUInt8 ui8_SoilMoisture_03;
	tUInt8 ui8_WaterLevel;
	tUInt8 ui8_PumpStatus;
	tUInt8 ui8_WindSpeed;
	tUInt8 ui8_Temperatur;
	tUInt8 ui8_Humidity;
	tUInt8 ui8_InfraRed;
	tUInt8 ui8_UltraViolet;
	tUInt8 ui8_Visible;
	tUInt8 ui8_MonoxidCarbon;
	tUInt8 ui8_LPG;
	tUInt8 ui8_Smoke;
};


private:
	ESP8266WiFiClass&   m_rc_ESP8266WiFiClass;
    WiFiClient&         m_rc_WiFiClient;
    IPAddress           m_c_local_IP; 
    IPAddress           m_c_gateway; 
    IPAddress           m_c_subnet;
    const tChar*        m_str_ssid;
    const tChar*        m_str_passwd;
	S_Measurements 		g_S_Measurements;
	LiquidCrystal_I2C	m_c_LiquidCrystal_I2C;
	#ifdef TIME_MEASUREMENT
	tUInt32 m_ui32_ElapsedTimeSendAndREceiveMsg;
	tUInt32 m_ui32_OldElapsedTimeSendAndREceiveMsg;
	#endif


    const tChar* host = "192.168.1.200";
    const tUInt16 port = 50011;

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 24/06/2023
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

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 24/06/2023
    ---------------------------------------------------------------------
    @Function description: Handle the receive message 
    ---------------------------------------------------------------------
    @Parameter: String str_RecvMsg, tUInt8 ui8_MessageLength, tUInt8* pACKMsg
    @Return value: -
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tVoid v_MessageHandler(String str_RecvMsg, tUInt8 ui8_MessageLength, tUInt8* pACKMsg)
    {
		tUInt8 aui8_DataToWrite[MSG_LENGTH_CYCLIC_MESSAGE + MSG_LENGTH_ACK_MESSAGE_SERVER] = {0}; 
		String str_Current;
		tUInt8 ui8_StrACKLength = str_RecvMsg.length();
		tUInt8 ui8_LastPosition = 0;
		tUInt8 ui8_PositionToWrite = 0;

		for(tUInt8 i = 0; i < ui8_StrACKLength; i++)
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
		memcpy(pACKMsg, aui8_DataToWrite + MSG_LENGTH_ACK_MESSAGE_SERVER, MSG_LENGTH_CYCLIC_MESSAGE);
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
 
public :
	static Logger m_c_Logger;
	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 24/06/2023
    ---------------------------------------------------------------------
    @Function description: constructor
    ---------------------------------------------------------------------
    @Parameter: ESP8266WiFiClass& rc_ESP8266WiFiClass, WiFiClient& rc_WiFiClient
    @Return value: -
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	ClientHandler(ESP8266WiFiClass& rc_ESP8266WiFiClass, WiFiClient& rc_WiFiClient)
    : m_rc_ESP8266WiFiClass(rc_ESP8266WiFiClass)
    , m_rc_WiFiClient(rc_WiFiClient)
    , m_c_local_IP(192,168,1,215)
    , m_c_gateway(192,168,1,1)
    , m_c_subnet(255,255,255,0)
    , m_str_ssid("WiFi_CleverTree")
    , m_str_passwd("02655826")
	, m_c_LiquidCrystal_I2C(0x27, 20, 4)
	#ifdef TIME_MEASUREMENT
	, m_ui32_ElapsedTimeSendAndREceiveMsg(0)
	, m_ui32_OldElapsedTimeSendAndREceiveMsg(0)
	#endif
	{

	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 24/06/2023
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
    @Author: Gerald Emvoutou
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
    @Author: Gerald Emvoutou
    @Create Date: 24/06/2023
    ---------------------------------------------------------------------
    @Function description: read the acknowledgment
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value: 	True -- Acknowledge received
					False -- Acknowledge not received
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tVoid v_HandlerReceivedMessage()
    {
		tUInt8 aui8_RecvMsg[MSG_LENGTH_CYCLIC_MESSAGE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		tUInt8 ui8_TimerWaitForMessage = 0;
		while((TIMEOUT_WAIT_FOR_SERVER_MESSAGE > ui8_TimerWaitForMessage)&& (m_rc_WiFiClient.available() == 0))
		{
			ui8_TimerWaitForMessage++;
			delay(10);
		}
		if(ui8_TimerWaitForMessage <= TIMEOUT_WAIT_FOR_SERVER_MESSAGE)
		{
			String str_RecvMsg = m_rc_WiFiClient.readStringUntil('\r');
			#ifdef TIME_MEASUREMENT
			m_ui32_ElapsedTimeSendAndREceiveMsg = ui32_GetElapsedTime(m_ui32_OldElapsedTimeSendAndREceiveMsg);
			m_c_Logger.v_Print(" CH Elapsed Time Send Msg and receive acknowldge: ");
			m_c_Logger.v_Print(m_ui32_ElapsedTimeSendAndREceiveMsg, false);
			Serial.println("ms");
			#endif
			m_c_Logger.v_Print(" CH: Received Msg from server: ");
			Serial.println(str_RecvMsg);
			if(str_RecvMsg.length() > 0)
			{
				v_MessageHandler(str_RecvMsg, sizeof(aui8_RecvMsg), aui8_RecvMsg);
			}
			else
			{
				m_c_Logger.v_Print(" CH: Message length small\n");
			}
			
		}
		else
		{
			m_c_Logger.v_Print(" CH: Serial port not available\n");
		}
		
		
		tUInt8 ui8_Position = 1;
		g_S_Measurements.ui8_PumpActivationRequested = aui8_RecvMsg[ui8_Position++];
		g_S_Measurements.ui8_SoilMoisture_01 = aui8_RecvMsg[ui8_Position++];
		g_S_Measurements.ui8_SoilMoisture_02 = aui8_RecvMsg[ui8_Position++];
		g_S_Measurements.ui8_SoilMoisture_03 = aui8_RecvMsg[ui8_Position++];
		g_S_Measurements.ui8_WaterLevel = aui8_RecvMsg[ui8_Position++];
		g_S_Measurements.ui8_PumpStatus = aui8_RecvMsg[ui8_Position++];
		g_S_Measurements.ui8_WindSpeed = aui8_RecvMsg[ui8_Position++];
		g_S_Measurements.ui8_Temperatur = aui8_RecvMsg[ui8_Position++];
		g_S_Measurements.ui8_Humidity = aui8_RecvMsg[ui8_Position++];
		g_S_Measurements.ui8_InfraRed = aui8_RecvMsg[ui8_Position++];
		g_S_Measurements.ui8_UltraViolet = aui8_RecvMsg[ui8_Position++];
		g_S_Measurements.ui8_Visible = aui8_RecvMsg[ui8_Position++];
		g_S_Measurements.ui8_MonoxidCarbon = aui8_RecvMsg[ui8_Position++];
		g_S_Measurements.ui8_LPG = aui8_RecvMsg[ui8_Position++];
		g_S_Measurements.ui8_Smoke = aui8_RecvMsg[ui8_Position];	
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 24/06/2023
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
			m_rc_WiFiClient.print(str_Data);
			b_MessageWasSent = true;
			#ifdef TIME_MEASUREMENT
			m_ui32_OldElapsedTimeSendAndREceiveMsg = millis();
			#endif
		}
		return b_MessageWasSent;
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 24/06/2023
    ---------------------------------------------------------------------
    @Function Description:  start the connection to the server
	----------------------------------------------------------------
	@parameter: --
	@Returnvalue:   True -- Connection to server established
					False -- Connection to server not established
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tBool b_StartServerConnection()
    {
		tBool b_ServerIsRunning = true;
		tUInt8 ui8_StartServerCounter = 0;
		m_rc_ESP8266WiFiClass.mode(WIFI_STA);
		m_rc_ESP8266WiFiClass.begin(m_str_ssid, m_str_passwd);
		if (!m_rc_ESP8266WiFiClass.config(m_c_local_IP, m_c_gateway, m_c_subnet)) 
		{
			b_ServerIsRunning = false;
			m_c_Logger.v_Print(" CH: IP Adress cannot be configured\n");
		} 
		m_c_Logger.v_Print(" CH: IPAddress: ");
		Serial.println(m_c_local_IP);
		// Waiting to connect to the WiFi
		tUInt8 ui8_ResetCounter = 0;
		while((ui8_ResetCounter < MAX_ATTEMPTS_CONNECTION_WIFI) && !b_StartWiFiConnection())
		{
			ui8_ResetCounter++;
			delay(10);
		}
		if(ui8_ResetCounter >= MAX_ATTEMPTS_CONNECTION_WIFI)
		{
			b_ServerIsRunning = false;
			m_c_Logger.v_Print(" CH: Connection to Wifi not established\n");
		}
		
		// Waiting to connect to the server 5S seconds
		while((!m_rc_WiFiClient.connect(host, port)) && (ui8_StartServerCounter < MAX_ATTEMPTS_CONNECTION_SERVER)) 
		{
			ui8_StartServerCounter++;
			delay(TIMER_500MS);
		}
		if(ui8_StartServerCounter >= MAX_ATTEMPTS_CONNECTION_SERVER)
		{
			b_ServerIsRunning = false;
			m_c_Logger.v_Print("CH: Connection to server not established\n");
		}

		return b_ServerIsRunning;
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 24/06/2023
    ---------------------------------------------------------------------
    @Function Description: print message part 1 on the display
	----------------------------------------------------------------
	@parameter: -
	@Returnvalue: -   
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tVoid v_LiquidCrystalDisplayPrint_01()
	{
		String str_Temp = "TEMPERATURE: ";
		tUInt8 ui8_TempLength = str_Temp.length();			// length = 14
		m_c_LiquidCrystal_I2C.clear();
		m_c_LiquidCrystal_I2C.setCursor(0, 0);
		m_c_LiquidCrystal_I2C.print(str_Temp);
		m_c_LiquidCrystal_I2C.setCursor(ui8_TempLength, 0);
		m_c_LiquidCrystal_I2C.print(g_S_Measurements.ui8_Temperatur);
		m_c_LiquidCrystal_I2C.setCursor((String(g_S_Measurements.ui8_Temperatur).length() + ui8_TempLength), 0);
		m_c_LiquidCrystal_I2C.print((char)223);

		String str_TankLevel = "RESERVOIR: ";
		tUInt8 ui8_TankLevelLength = str_TankLevel.length();			// length = 12
		m_c_LiquidCrystal_I2C.setCursor(0, 1);
		m_c_LiquidCrystal_I2C.print(str_TankLevel);
		m_c_LiquidCrystal_I2C.setCursor(ui8_TankLevelLength, 1);
		m_c_LiquidCrystal_I2C.print(g_S_Measurements.ui8_WaterLevel);
		m_c_LiquidCrystal_I2C.setCursor((String(g_S_Measurements.ui8_WaterLevel).length() + ui8_TankLevelLength), 1);
		m_c_LiquidCrystal_I2C.print("%");
		v_LiquidCrystalDisplayPrintGauge(g_S_Measurements.ui8_WaterLevel, 0, 2);

		String str_WindSpeed = "VENT: ";
		tUInt8 ui8_WindSpeed = str_WindSpeed.length();      	// length = 7
		m_c_LiquidCrystal_I2C.setCursor(0, 3);
		m_c_LiquidCrystal_I2C.print(str_TankLevel);
		m_c_LiquidCrystal_I2C.setCursor(ui8_WindSpeed, 3);
		m_c_LiquidCrystal_I2C.print(g_S_Measurements.ui8_WindSpeed);
		m_c_LiquidCrystal_I2C.setCursor((String(g_S_Measurements.ui8_WindSpeed).length() + ui8_WindSpeed), 3);
		m_c_LiquidCrystal_I2C.print("M/S");
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 24/06/2023
    ---------------------------------------------------------------------
    @Function Description: print message part 2 on the display
	----------------------------------------------------------------
	@parameter: -
	
	@Returnvalue: -   
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tVoid v_LiquidCrystalDisplayPrint_02()
	{
		m_c_LiquidCrystal_I2C.clear();
		String str_SoilMois01 = "SOL 01:";
		tUInt8 ui8_SoilMois01 = str_SoilMois01.length();			// length = 8
		m_c_LiquidCrystal_I2C.setCursor(0, 0);
		m_c_LiquidCrystal_I2C.print(str_SoilMois01);
		v_LiquidCrystalDisplayPrintGauge(g_S_Measurements.ui8_SoilMoisture_01, (ui8_SoilMois01+1), 0);

		String str_SoilMois02 = "SOL 02:";
		tUInt8 ui8_SoilMois02 = str_SoilMois02.length();			// length = 8
		m_c_LiquidCrystal_I2C.setCursor(0, 1);
		m_c_LiquidCrystal_I2C.print(str_SoilMois02);
		v_LiquidCrystalDisplayPrintGauge(g_S_Measurements.ui8_SoilMoisture_02, (ui8_SoilMois02+1), 1);

		String str_SoilMois03 = "SOL 03:";
		tUInt8 ui8_SoilMois03 = str_SoilMois03.length();			// length = 8
		m_c_LiquidCrystal_I2C.setCursor(0, 2);
		m_c_LiquidCrystal_I2C.print(str_SoilMois03);
		v_LiquidCrystalDisplayPrintGauge(g_S_Measurements.ui8_SoilMoisture_03, (ui8_SoilMois03+1), 2);

		String str_PumpState = "ETAT POMPE: ";
		tUInt8 ui8_PumpState = str_PumpState.length();			// length = 13
		m_c_LiquidCrystal_I2C.setCursor(0, 3);
		m_c_LiquidCrystal_I2C.print(str_PumpState);
		m_c_LiquidCrystal_I2C.setCursor(ui8_PumpState, 3);
		(g_S_Measurements.ui8_PumpStatus == 1) ? m_c_LiquidCrystal_I2C.print("ALLUME") : m_c_LiquidCrystal_I2C.print("ETEINT");
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 24/06/2023
    ---------------------------------------------------------------------
    @Function Description: 
	----------------------------------------------------------------
	@parameter: --
	@Returnvalue:   
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tVoid v_LiquidCrystalDisplayInit()
	{
		m_c_LiquidCrystal_I2C.init();
		m_c_LiquidCrystal_I2C.backlight();
		m_c_LiquidCrystal_I2C.createChar(7, gauge_empty);   
		m_c_LiquidCrystal_I2C.createChar(1, gauge_fill_1);  
		m_c_LiquidCrystal_I2C.createChar(2, gauge_fill_2);  
		m_c_LiquidCrystal_I2C.createChar(3, gauge_fill_3);  
		m_c_LiquidCrystal_I2C.createChar(4, gauge_fill_4);
		m_c_LiquidCrystal_I2C.setCursor(6, 0);
		m_c_LiquidCrystal_I2C.print("BIENVENUE");
		m_c_LiquidCrystal_I2C.setCursor(7, 1);
		m_c_LiquidCrystal_I2C.print("WELCOME");
		m_c_LiquidCrystal_I2C.setCursor(9, 2);
		m_c_LiquidCrystal_I2C.print("TO");
		m_c_LiquidCrystal_I2C.setCursor(2, 3);
		m_c_LiquidCrystal_I2C.print("MAKER FACTORY LAB");
		delay(WAIT_5000_MS);
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 24/06/2023
    ---------------------------------------------------------------------
    @Function Description: 
	----------------------------------------------------------------
	@parameter: tUInt8 ui8_Value, tUInt8 ui8_Column, tUInt8 ui8_Line
	@Returnvalue: -
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tVoid v_LiquidCrystalDisplayPrintGauge(tUInt8 ui8_Value, tUInt8 ui8_Column, tUInt8 ui8_Line)
	{
		float units_per_pixel = (gauge_size_chars*5.0)/100.0;        //  every character is 5px wide, we want to count from 0-100
		tUInt8 value_in_pixels = round(cpu_gauge * units_per_pixel);    // cpu_gauge value converted to pixel width
		tUInt8 tip_position = 0;      // 0= not set, 1=tip in first char, 2=tip in middle, 3=tip in last char

		if (value_in_pixels < 5) 
		{
			tip_position = 1;// tip is inside the first character
		}                            
		else if (value_in_pixels > gauge_size_chars*5.0-5) 
		{
			tip_position = 3;		// tip is inside the last character
		}  
		else 
		{
			tip_position = 2;	// tip is somewhere in the middle
		}                                                

		move_offset = 4 - ((value_in_pixels-1) % 5);      // value for offseting the pixels for the smooth filling

		// dynamically create left part of the gauge
		for (tUInt8 i=0; i<8; i++) 
		{   
			if (tip_position == 1) 
			{
				gauge_left_dynamic[i] = (gauge_fill_5[i] << move_offset) | gauge_left[i];	 // tip on the first character
			} 
			else 
			{
				gauge_left_dynamic[i] = gauge_fill_5[i];			// tip not on the first character
			}                                                     
			gauge_left_dynamic[i] = gauge_left_dynamic[i] & gauge_mask_left[i];                                 // apply mask for rounded corners
		}

		// dynamically create right part of the gauge
		for (tUInt8 i=0; i<8; i++) 
		{   
			if (tip_position == 3) 
			{
				gauge_right_dynamic[i] = (gauge_fill_5[i] << move_offset) | gauge_right[i];				// tip on the last character
			}  
			else 
			{
				gauge_right_dynamic[i] = gauge_right[i];						// tip not on the last character
			}                                                       
			gauge_right_dynamic[i] = gauge_right_dynamic[i] & gauge_mask_right[i];                                // apply mask for rounded corners
		}  

		m_c_LiquidCrystal_I2C.createChar(5, gauge_left_dynamic);     // create custom character for the left part of the gauge
		m_c_LiquidCrystal_I2C.createChar(6, gauge_right_dynamic);    // create custom character for the right part of the gauge

		// set all the characters for the gauge
		for (tUInt8 i=0; i<gauge_size_chars; i++) 
		{  
			if (i==0) 
			{
				gauge_string[i] = byte(5);					 // first character = custom left piece
			}                       
			else if (i==gauge_size_chars-1) 
			{
				gauge_string[i] = byte(6);					// last character = custom right piece
			}  
			// character in the middle, could be empty, tip or fill
			else {                                                        
				if (value_in_pixels <= i*5) 
				{
					gauge_string[i] = byte(7);					// empty character
				}   
				else if (value_in_pixels > i*5 && value_in_pixels < (i+1)*5) 
				{
					gauge_string[i] = byte(5-move_offset);			// tip
				} 
				else 
				{
					gauge_string[i] = byte(255);	// filled character
				}                        
			}
		}
		m_c_LiquidCrystal_I2C.setCursor(ui8_Column, ui8_Line);              // move the cursor to the next line
		m_c_LiquidCrystal_I2C.print(gauge_string);         					// display the gauge

		// increase the CPU value, set between 0-100
		cpu_gauge = ui8_Value;
		if (cpu_gauge > 100) 
		{
			cpu_gauge = 0;
		}
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 28/06/2023
    ---------------------------------------------------------------------
    @Function description: call once
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value:  True when the connection to WiFi and Server is established
                    False if the connection can not be established
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tBool b_OnEntry(tBool force = false)
	{
		if(!force)
		{
			m_c_Logger.v_Init();
			v_LiquidCrystalDisplayInit();
		}
		return b_StartServerConnection();
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 28/06/2023
    ---------------------------------------------------------------------
    @Function description: call cyclically
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value:  True if the Message was successful sent to the server
                    False if the message can not be send
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tBool b_OnDuring()
	{
		tBool b_RetVal = false;
		tUInt8 aui8_DataTosend[2] = {MSG_ID_LCD_MESSAGE, 0xFF};
		if(b_SendMessage(aui8_DataTosend, sizeof(aui8_DataTosend)))
		{
			v_HandlerReceivedMessage();
			b_RetVal = true;
		}
		return b_RetVal;
	}

};
#endif //CLIENTHANDLER_H