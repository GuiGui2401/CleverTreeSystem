/**
*@Copyright DTTA Maker Factory Tech. All Rights reserved.
*It is not allowed, whether to copy this document, or to pass it even in extracts to others, without the approval DTTA Maker Factory Tech.
*All copyrights for this document are the exclusive property of DTTA Maker Factory Tech.
*/ 
/*----------------------------------------------------------------------*/
#ifndef SUNLIGHTSENSORMEASUREMENT_H
#define SUNLIGHTSENSORMEASUREMENT_H
/**
*@file: SunlightSensorMeasurement.h
*@author: Emvoutou, Cezy <cezy.emvoutou@dta-alliance.com> 
*@date: 01/07/2023 12:51:35
 *//*----------------------------------------------------------------------*/
 
/*-----------------------------includes---------------------------------*/
/* C system headers */ 
/* C++ standard library headers */ 
/* External libraires headers */ 
/* Arduino libraries headers */ 
#include "additional_types.h"
#include "SI114X.h"
#include "Logger.h"
 
/*-------------------------global defines-------------------------------*/

class SunlightSensorMeasurement
{ 

private: 

    SI114X& m_rc_SunlightSensor;

public :

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 01/07/2023
    ---------------------------------------------------------------------
    @Function description: 
    ---------------------------------------------------------------------
    @Parameter: 
    @Return value: 
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	SunlightSensorMeasurement(SI114X& rc_SunlightSensor) 
	: m_rc_SunlightSensor(rc_SunlightSensor)
	{

	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 01/07/2023
    ---------------------------------------------------------------------
    @Function description: 
    ---------------------------------------------------------------------
    @Parameter: 
    @Return value: 
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	~SunlightSensorMeasurement() 
	{

	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 01/07/2023
    ---------------------------------------------------------------------
    @Function description: Get UV value
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value: 
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tUInt8 ui8_ReadUltraVioletValue()
    {
        tUInt16 ui16_UltravioletValue = m_rc_SunlightSensor.ReadUV();
        ClientHandler::m_c_Logger.v_Print("UV raw value: ");
        Serial.println(ui16_UltravioletValue);
        tUInt16 ui16_Factor = (ui16_UltravioletValue > UINT8_MAX) ? 1000 : 1;
        return static_cast<tUInt8>((ui16_UltravioletValue / ui16_Factor));
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 01/07/2023
    ---------------------------------------------------------------------
    @Function description: Get IR value
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value: 
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tUInt8 ui8_ReadInfraRedValue()
    {
        tUInt16 ui16_InfraRedValue = m_rc_SunlightSensor.ReadIR();
        ClientHandler::m_c_Logger.v_Print("IR raw value: ");
        Serial.println(ui16_InfraRedValue);
        tUInt16 ui16_Factor = (ui16_InfraRedValue > UINT8_MAX) ? 1000 : 1;
        return static_cast<tUInt8>((ui16_InfraRedValue / ui16_Factor));
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 01/07/2023
    ---------------------------------------------------------------------
    @Function description: Get Visible value
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value: 
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tUInt8 ui8_ReadVisibleValue()
    {
        tUInt16 ui16_VisibleValue = m_rc_SunlightSensor.ReadVisible();
        ClientHandler::m_c_Logger.v_Print("Visible raw value: ");
        Serial.println(ui16_VisibleValue);
        tUInt16 ui16_Factor = (ui16_VisibleValue > UINT8_MAX) ? 1000 : 1;
        return static_cast<tUInt8>((ui16_VisibleValue / ui16_Factor));
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 01/07/2023
    ---------------------------------------------------------------------
    @Function description: initialize the Sunlight sensor
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value:  True -- Sunlight sensor was initialized
                    False -- Sunlight sensor wasn't initialized
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tBool b_InitSunlightSensor( )
    {
        return m_rc_SunlightSensor.Begin();
	}

};


#endif //SUNLIGHTSENSORMEASUREMENT_H