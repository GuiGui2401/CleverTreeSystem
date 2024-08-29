/**
*@Copyright DTTA Maker Factory Tech. All Rights reserved.
*It is not allowed, whether to copy this document, or to pass it even in extracts to others, without the approval DTTA Maker Factory Tech.
*All copyrights for this document are the exclusive property of DTTA Maker Factory Tech.
*/ 
/*----------------------------------------------------------------------*/
#ifndef DHT11MEASUREMENT_H
#define DHT11MEASUREMENT_H
/**
*@file: DHT11Measurement.h
*@Author: Emvoutou, Cezy <cezy.emvoutou@dta-alliance.com> 
*@date: 28/06/2023 22:40:57
 *//*----------------------------------------------------------------------*/
 
/*-----------------------------includes---------------------------------*/
/* C system headers */ 
/* C++ standard library headers */ 
/* External libraires headers */ 
/* Arduino libraries headers */ 
#include "ClientHandler.h"
#include "MQ2Measurement.h"
#include "SunlightSensorMeasurement.h"
#include "DHTesp.h"
 
/*-------------------------global defines-------------------------------*/

class DHT11Measurement
{ 

private: 
    ClientHandler&              m_rc_ClientHandler;
    DHTesp                      m_c_DHT11;
    tBool                       m_b_AckForMsgPending;
    MQ2Measurement&             m_rc_MQ2Measurement;
    SunlightSensorMeasurement&  m_rc_SunlightSensorMeasurement;
    tBool                       m_b_SunlightSensorInitDone;
    
public :

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 28/06/2023
    ---------------------------------------------------------------------
    @Function description: constructor
    ---------------------------------------------------------------------
    @Parameter: ClientHandler& rc_ClientHandler
    @Return value: -
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	DHT11Measurement(ClientHandler& rc_ClientHandler, MQ2Measurement& rc_MQ2Measurement, SunlightSensorMeasurement& rc_SunlightSensorMeasurement) 
	: m_rc_ClientHandler(rc_ClientHandler)
    , m_c_DHT11()
    , m_b_AckForMsgPending(false)
    , m_rc_MQ2Measurement(rc_MQ2Measurement)
    , m_rc_SunlightSensorMeasurement(rc_SunlightSensorMeasurement)
    , m_b_SunlightSensorInitDone(false)
	{

	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 28/06/2023
    ---------------------------------------------------------------------
    @Function description: destructor
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value: -
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	~DHT11Measurement() 
	{

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
	tBool b_OnEntry( )
    {
        ClientHandler::m_c_Logger.v_Init();
        tBool b_RetVal = m_rc_ClientHandler.b_StartServerConnection();
        m_c_DHT11.setup(DHTPin, DHTesp::DHT11);
        m_rc_MQ2Measurement.v_InitMQ2Sensor();
        m_b_SunlightSensorInitDone = m_rc_SunlightSensorMeasurement.b_InitSunlightSensor();

        return b_RetVal;
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
	tBool b_Onduring( )
    {
        tBool b_RetVal = true;
        tUInt8 aui8_DHT11DataToSend[DHT11_DATA_MESSAGE_LENGTH] = {MSG_ID_DHT11_DATA_MESSAGE, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
        tUInt8 ui8_Position = 1;
        aui8_DHT11DataToSend[ui8_Position++] = ui8_GetTemperature(); //random(20,40);  
        aui8_DHT11DataToSend[ui8_Position++] = ui8_GetHumidity(); //random(0,100); 
        aui8_DHT11DataToSend[ui8_Position++] =  m_rc_MQ2Measurement.ui8_ReadCarbonMonoxidValue();
        aui8_DHT11DataToSend[ui8_Position++] = m_rc_MQ2Measurement.ui8_ReadLPGValue();
        aui8_DHT11DataToSend[ui8_Position++] = m_rc_MQ2Measurement.ui8_ReadSmokeValue();
        if (m_b_SunlightSensorInitDone)
        {
            aui8_DHT11DataToSend[ui8_Position++] = random(0,255);  //m_rc_SunlightSensorMeasurement.ui8_ReadInfraRedValue();
            aui8_DHT11DataToSend[ui8_Position++] = random(0,255);  //m_rc_SunlightSensorMeasurement.ui8_ReadUltraVioletValue();
            aui8_DHT11DataToSend[ui8_Position] = random(0,255);  //m_rc_SunlightSensorMeasurement.ui8_ReadVisibleValue();
        }
        else
        {
            ClientHandler::m_c_Logger.v_Print(" DHT11: Sunlight sensor init failed\n");
        }
        
        if (m_rc_ClientHandler.b_IsClientRunning())
        {
            if(!m_b_AckForMsgPending)
            { 
                if (!m_rc_ClientHandler.b_SendMessage(aui8_DHT11DataToSend, sizeof(aui8_DHT11DataToSend)))
                {
                    ClientHandler::m_c_Logger.v_Print("DHT11: Message can not be send to server\n");
                }
                else
                {
                    m_b_AckForMsgPending = true;
                }
            }
            if(m_b_AckForMsgPending)
            {
                m_b_AckForMsgPending = m_rc_ClientHandler.b_HandlerReceivedMessage() ? false : true;
                ClientHandler::m_c_Logger.v_Print("DHT11: ACK for received message is ");
                Serial.println(m_b_AckForMsgPending);
            }
        }
        else
        {
            ClientHandler::m_c_Logger.v_Print("DHT11: Client is not running\n");
        }
        b_RetVal = m_b_AckForMsgPending;
        return b_RetVal;
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 28/06/2023
    ---------------------------------------------------------------------
    @Function description: read the temperature
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value: Temperature value
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tUInt8 ui8_GetTemperature( )
    {
        float temp = m_c_DHT11.getTemperature();
        ClientHandler::m_c_Logger.v_Print("Temp raw value: ");
        Serial.println(temp);
        return static_cast<tUInt8>(temp);
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 28/06/2023
    ---------------------------------------------------------------------
    @Function description: read humitidy
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value: Humidity value
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tUInt8 ui8_GetHumidity( )
    {
        float hum = m_c_DHT11.getHumidity();
        ClientHandler::m_c_Logger.v_Print("Hum raw value: ");
        Serial.println(hum);
        return static_cast<tUInt8>(hum);
	}

};
#endif //DHT11MEASUREMENT_H
