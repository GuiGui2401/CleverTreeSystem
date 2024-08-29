/**
*@Copyright DTTA Maker Factory Tech. All Rights reserved.
*It is not allowed, whether to copy this document, or to pass it even in extracts to others, without the approval DTTA Maker Factory Tech.
*All copyrights for this document are the exclusive property of DTTA Maker Factory Tech.
*/ 
/*----------------------------------------------------------------------*/
#ifndef SOILMOISTUREMEASUREMENT_H
#define SOILMOISTUREMEASUREMENT_H
/**
*@file: SoilMoistureMeasurement.h
*@author: Emvoutou, Cezy <cezy.emvoutou@dta-alliance.com> 
*@date: 04/07/2023 21:29:50
 *//*----------------------------------------------------------------------*/
 
/*-----------------------------includes---------------------------------*/
/* C system headers */ 
/* C++ standard library headers */ 
/* External libraires headers */ 
/* Arduino libraries headers */ 
#include "additional_types.h"
#include "WiFi_Server.h"
 
/*-------------------------global defines-------------------------------*/

class SoilMoistureMeasurement
{ 

private: 
    WiFi_Server& m_rc_WiFi_Server;
    tUInt8 m_ui8_SoilMoistureValue;
    tUInt32 m_ui32_SendCounter;
    tUInt32 m_ui32_ReceivedCounter;
    tBool m_b_ACkIsPending;
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author:
    @Create Date: 04/07/2023
    ---------------------------------------------------------------------
    @Function description: 
    ---------------------------------------------------------------------
    @Parameter: 
    @Return value: 
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tBool b_PumpActivationRequested( )
    {
        tBool b_IsRequested = false;
        if (m_ui8_SoilMoistureValue < 30)
        {
            b_IsRequested = true;
        }
        return b_IsRequested;
	}

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author:
    @Create Date: 04/07/2023
    ---------------------------------------------------------------------
    @Function description: 
    ---------------------------------------------------------------------
    @Parameter: 
    @Return value: 
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tUInt8 ui8_ReadSoilMoistureValue( )
    {
        tUInt16 ui16_SoilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
        m_ui8_SoilMoistureValue = map(ui16_SoilMoistureValue, 60, 600, 0, 100);
        return m_ui8_SoilMoistureValue;
	}

public :

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author:
    @Create Date: 04/07/2023
    ---------------------------------------------------------------------
    @Function description: 
    ---------------------------------------------------------------------
    @Parameter: 
    @Return value: 
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	SoilMoistureMeasurement(WiFi_Server& rc_WiFi_Server) 
	: m_rc_WiFi_Server(rc_WiFi_Server)
    , m_ui8_SoilMoistureValue(255)
    , m_ui32_SendCounter(0)
    , m_ui32_ReceivedCounter(0)
    , m_b_ACkIsPending(false)
	{

	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author:
    @Create Date: 04/07/2023
    ---------------------------------------------------------------------
    @Function description: 
    ---------------------------------------------------------------------
    @Parameter: 
    @Return value: 
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	~SoilMoistureMeasurement() 
	{

	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author:
    @Create Date: 04/07/2023
    ---------------------------------------------------------------------
    @Function description: 
    ---------------------------------------------------------------------
    @Parameter: 
    @Return value: 
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tBool b_OnEntry( )
    {
        tBool b_RetVal = m_rc_WiFi_Server.b_StartServer();
        return b_RetVal;
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author:
    @Create Date: 04/07/2023
    ---------------------------------------------------------------------
    @Function description: 
    ---------------------------------------------------------------------
    @Parameter: 
    @Return value: 
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tBool b_OnDuring( )
    {
        tBool b_RetVal = true;
        tUInt8 aui8_DataToSend[MSG_SOILMOISTURE_LENGTH] = {MSG_ID_SOIL_MOISTURE_MESSAGE, 0, 0};
        tUInt8 ui8_Position = 1;
        aui8_DataToSend[ui8_Position++] = static_cast<tUInt8>(b_PumpActivationRequested());
        aui8_DataToSend[ui8_Position] = ui8_ReadSoilMoistureValue();
        if(m_rc_WiFi_Server.b_ClientIsRunning())
        {
            if(m_b_ACkIsPending == false)
            {
                if (m_rc_WiFi_Server.b_SendMessage(&aui8_DataToSend[0], sizeof(aui8_DataToSend)))
                {
                    WiFi_Server::m_c_Logger.v_Print("SMM: SendCounter: ");
                    WiFi_Server::m_c_Logger.v_Print(m_ui32_SendCounter++, false);
                    WiFi_Server::m_c_Logger.v_Print(", ReceiveCounter: ", false);
                    WiFi_Server::m_c_Logger.v_Print(m_ui32_ReceivedCounter, false);
                    WiFi_Server::m_c_Logger.v_Print("\n");
                    m_b_ACkIsPending = true;
                    char c_Debug[128];
                    sprintf(c_Debug, "SMM: Mois Sensor Value: %d Percent, Pump activation Requested: %d", aui8_DataToSend[2], aui8_DataToSend[1]);
                    WiFi_Server::m_c_Logger.v_Print(c_Debug, true);
                }
                else
                {
                    WiFi_Server::m_c_Logger.v_Print("SMM: Message to server cannot send\n");
                }
            }
               
            if(m_b_ACkIsPending)
            {
                WiFi_Server::m_c_Logger.v_Print("SMM: ACK is pending\n");
                if (m_rc_WiFi_Server.b_HandlerReceivedMessage())
                {
                    WiFi_Server::m_c_Logger.v_Print("SMM: SendCounter: ");
                    WiFi_Server::m_c_Logger.v_Print(m_ui32_SendCounter, false);
                    WiFi_Server::m_c_Logger.v_Print(", ReceiveCounter: ", false);
                    WiFi_Server::m_c_Logger.v_Print(m_ui32_ReceivedCounter++, false);
                    WiFi_Server::m_c_Logger.v_Print("\n");
                    m_b_ACkIsPending = false;
                }
            }
            
        }
        else
        {
            WiFi_Server::m_c_Logger.v_Print("SMM: Client is not running\n");
        }

        return b_RetVal;
        
	}

};
#endif //SOILMOISTUREMEASUREMENT_H