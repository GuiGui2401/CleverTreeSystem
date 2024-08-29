#ifndef PUMPHANDLER_H
#define PUMPHANDLER_H

#include "Utilities.h"
#include "Rte_Type_Def.h"
#include "WiFi_Server.h"

class PumpHandler
{

public:

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 27.03.2023
----------------------------------------------------------------
@Function Description: constructor
----------------------------------------------------------------
@parameter: tUInt8 ui8_PumpPort, E_Relai_Configuration e_Relai_Configuration
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
PumpHandler(tUInt8 ui8_PumpPort, E_Relai_Configuration e_Relai_Configuration)
: m_ui8_PumpPort(ui8_PumpPort)
, m_e_Relai_Configuration(e_Relai_Configuration)
, m_ui8_PumpStatus(PUMP_CLOSED)
{

}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 27.03.2023
----------------------------------------------------------------
@Function Description: destructor
----------------------------------------------------------------
@parameter: -
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
~PumpHandler()
{

}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 27.03.2023
----------------------------------------------------------------
@Function Description: Init the pin for the pump and configure 
the relai pin mode
----------------------------------------------------------------
@parameter: --
@Returnvalue: --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_Init()
{
    pinMode(m_ui8_PumpPort, 0x01);
    Rte_Call_Write_Digital_Pin_PT_ESP8266_Digital_Pin(m_ui8_PumpPort, m_e_Relai_Configuration);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 27.03.2023
----------------------------------------------------------------
@Function Description: open the pump
----------------------------------------------------------------
@parameter: --
@Returnvalue: --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_OpenPump()
{
    if(E_OK == Rte_Call_Write_Digital_Pin_PT_ESP8266_Digital_Pin(m_ui8_PumpPort, 0x01))
    {
        m_ui8_PumpStatus = PUMP_OPENED;
        WiFi_Server::m_c_Logger.v_Print("PH: the pump is opened\n");
    }
    else
    {
        WiFi_Server::m_c_Logger.v_Print("PH: the pump can not be opened\n");
    }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 27.03.2023
----------------------------------------------------------------
@Function Description: close the pump
----------------------------------------------------------------
@parameter: --
@Returnvalue: --
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_ClosePump()
{
    if(E_OK == Rte_Call_Write_Digital_Pin_PT_ESP8266_Digital_Pin(m_ui8_PumpPort, 0x00))
    {
        m_ui8_PumpStatus = PUMP_CLOSED;
        WiFi_Server::m_c_Logger.v_Print("PH: The pump is closed\n");
    }
    else
    {
        WiFi_Server::m_c_Logger.v_Print("PH: The pump can not be closed\n");
    }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 27.03.2023
----------------------------------------------------------------
@Function Description: get the pump status
----------------------------------------------------------------
@parameter: --
@Returnvalue:   0x00 -- Pump is closed
                0x01 -- Pumo is opened
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tUInt8 ui8_GetPumpStatus() const
{
    return m_ui8_PumpStatus;
}

private:
    tUInt8 m_ui8_PumpPort;
    E_Relai_Configuration m_e_Relai_Configuration;
    tUInt8 m_ui8_PumpStatus;

};
#endif
