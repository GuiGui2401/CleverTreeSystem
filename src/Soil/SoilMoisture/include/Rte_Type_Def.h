#ifndef __RTE_TYPE_DEF_H__
#define __RTE_TYPE_DEF_H__

#include "additional_types.h"
#include <Arduino.h>


/***************************************************************
 * 
 *   RTE INTERFACE TO READ DATA FROM ANALOG PIN
 * 
 * *************************************************************/
Std_ReturnType  Rte_Call_Read_Analog_Pin_PT_ESP8266_Analog_Pin(tUInt8 portnumber, tUInt8& value)
{
    Std_ReturnType n_ErrorCode = E_OK;
    value = analogRead(portnumber);
    value = map(value, 0, 1023, 0, 100);
    return n_ErrorCode;
}

Std_ReturnType Rte_Call_Write_Digital_Pin_PT_ESP8266_Digital_Pin(tUInt8 portnumber, tUInt8 value)
{
    Std_ReturnType n_ErrorCode = E_OK;
    if(value > 1)
    {
        n_ErrorCode = E_NOT_OK;
    }
    else
    {
        digitalWrite(portnumber, value);
    }
    return n_ErrorCode;
}

Std_ReturnType Rte_Call_Read_Serial_Port_PT_ESP8266_Serial_Port(tUInt8* data, tUInt8 length)
{
    tUInt16 ui16_TimerInMs = 0;
    Std_ReturnType n_ErrorCode = E_OK;
    while((Serial.available() == 0) && (ui16_TimerInMs < TIMEOUT_WAIT_MESSAGE_SERIALPORT))
    {
        ui16_TimerInMs++;
    }
    if(ui16_TimerInMs < TIMEOUT_WAIT_MESSAGE_SERIALPORT )
    {
        tUInt8 ui8_NumberOfBytes = Serial.readBytes(data, length);
        if(ui8_NumberOfBytes < length)
        {
            n_ErrorCode = E_NOT_OK;
        }
    }

    return n_ErrorCode;
}

#endif  // __RTE_TYPE_DEF_H__
