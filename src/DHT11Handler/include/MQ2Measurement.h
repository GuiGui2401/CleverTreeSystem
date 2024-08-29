/**
*@Copyright DTTA Maker Factory Tech. All Rights reserved.
*It is not allowed, whether to copy this document, or to pass it even in extracts to others, without the approval DTTA Maker Factory Tech.
*All copyrights for this document are the exclusive property of DTTA Maker Factory Tech.
*/ 
/*----------------------------------------------------------------------*/
#ifndef MQ2MEASUREMENT_H
#define MQ2MEASUREMENT_H
/**
*@file: MQ2Measurement.h
*@author: Emvoutou, Cezy <cezy.emvoutou@dta-alliance.com> 
*@date: 30/06/2023 17:52:24
 *//*----------------------------------------------------------------------*/
 
/*-----------------------------includes---------------------------------*/
/* C system headers */ 
/* C++ standard library headers */ 
/* External libraires headers */ 
/* Arduino libraries headers */ 
#include "additional_types.h"
#include <MQ2.h>
#include "Logger.h"
 
/*-------------------------global defines-------------------------------*/

class MQ2Measurement
{ 

private: 
    MQ2& m_rc_MQ2Sensor;

public :

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 30/06/2023
    ---------------------------------------------------------------------
    @Function description: constructor
    ---------------------------------------------------------------------
    @Parameter: MQ2& rc_MQ2Sensor
    @Return value: -
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	MQ2Measurement(MQ2& rc_MQ2Sensor) 
	: m_rc_MQ2Sensor(rc_MQ2Sensor)
	{

	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 30/06/2023
    ---------------------------------------------------------------------
    @Function description: destructor
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value: -
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	~MQ2Measurement() 
	{

	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 30/06/2023
    ---------------------------------------------------------------------
    @Function description: Get CO value from MQ2 sensor
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value: CO value
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tUInt8 ui8_ReadCarbonMonoxidValue( )
    {
        float f_CO = m_rc_MQ2Sensor.readCO();
        ClientHandler::m_c_Logger.v_Print("CO Raw value: ");
        Serial.println(f_CO);
        return static_cast<tUInt8>(f_CO);
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 30/06/2023
    ---------------------------------------------------------------------
    @Function description: Get LPG value from MQ2 sensor
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value: LPG value
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tUInt8 ui8_ReadLPGValue()
    {
        float f_LPG = m_rc_MQ2Sensor.readLPG();
        ClientHandler::m_c_Logger.v_Print("LPG Raw value: ");
        Serial.println(f_LPG);
        return static_cast<tUInt8>(f_LPG);
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 30/06/2023
    ---------------------------------------------------------------------
    @Function description: Get Smoke value from MQ2 sensor
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value: Smoke value
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tUInt8 ui8_ReadSmokeValue()
    {
        float f_Smoke = m_rc_MQ2Sensor.readSmoke();
        ClientHandler::m_c_Logger.v_Print("Smoke Raw value: ");
        Serial.println(f_Smoke);
        return static_cast<tUInt8>(f_Smoke);
	}

	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author: Gerald Emvoutou
    @Create Date: 30/06/2023
    ---------------------------------------------------------------------
    @Function description: initialize the MQ2 sensor
    ---------------------------------------------------------------------
    @Parameter: -
    @Return value: -
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	tVoid v_InitMQ2Sensor( )
    {
        m_rc_MQ2Sensor.begin();
	}

};
#endif //MQ2MEASUREMENT_H