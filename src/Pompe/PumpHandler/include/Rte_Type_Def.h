#ifndef __RTE_TYPE_DEF_H__
#define __RTE_TYPE_DEF_H__

#include "additional_types.h"
#include <Arduino.h>
#include <HCSR04.h>
#include <SoftwareSerial.h>



/***************************************************************
 * 
 *   RTE INTERFACE TO READ DATA FROM ANALOG PIN
 * 
 * *************************************************************/

// Read analog pin
Std_ReturnType  Rte_Call_Read_Analog_Pin_PT_ESP8266_Analog_Pin(tUInt8 portnumber, tUInt8& value)
{
    Std_ReturnType n_ErrorCode = E_OK;
    value = analogRead(portnumber);
    value = map(value, 0, 1023, 0, 100);
    return n_ErrorCode;
}

// write on digital pin
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

// read water level with HCSR04 Sensor
Std_ReturnType Rte_Call_Read_Water_Level_PT_ESP8266_Water_Level(tUInt8& value, tUInt8 triggerPin = D0, tUInt8 echoPin = D1)
{
    UltraSonicDistanceSensor  c_HCSR04(triggerPin, echoPin);
    Std_ReturnType n_ErrorCode = E_OK;
    tUInt8 dist = static_cast<tUInt8>(c_HCSR04.measureDistanceCm()) ;
    delay(10);

    dist = (dist > MIN_TANK_LEVEL) ? MIN_TANK_LEVEL : dist;
    dist = (dist < MAX_TANK_LEVEL) ? MAX_TANK_LEVEL : dist;
    if(dist <= MIN_TANK_LEVEL && dist > TANK_LEVEL_3PERCENT)
    {
        value = map(dist, MIN_TANK_LEVEL, TANK_LEVEL_3PERCENT, 0, 3);
    }
    else if(dist <= TANK_LEVEL_3PERCENT && dist > TANK_LEVEL_7PERCENT)
    {
        value = map(dist, TANK_LEVEL_3PERCENT, TANK_LEVEL_7PERCENT, 4, 7);
    }
    else if(dist <= TANK_LEVEL_7PERCENT && dist > TANK_LEVEL_11PERCENT)
    {
        value = map(dist, TANK_LEVEL_7PERCENT, TANK_LEVEL_11PERCENT, 8, 11);
    }
    else if(dist <= TANK_LEVEL_11PERCENT && dist > TANK_LEVEL_14PERCENT)
    {
        value = map(dist, TANK_LEVEL_11PERCENT, TANK_LEVEL_14PERCENT, 12, 14);
    }
    else if(dist <= TANK_LEVEL_14PERCENT && dist > TANK_LEVEL_17PERCENT)
    {
        value = map(dist, TANK_LEVEL_14PERCENT, TANK_LEVEL_17PERCENT, 15, 17);
    }
    else if(dist <= TANK_LEVEL_17PERCENT && dist > TANK_LEVEL_21PERCENT)
    {
        value = map(dist, TANK_LEVEL_17PERCENT, TANK_LEVEL_21PERCENT, 18, 21);
    }
    else if(dist <= TANK_LEVEL_21PERCENT && dist > TANK_LEVEL_25PERCENT)
    {
        value = map(dist, TANK_LEVEL_21PERCENT, TANK_LEVEL_25PERCENT, 22, 25);
    }
    else if(dist <= TANK_LEVEL_25PERCENT && dist > TANK_LEVEL_50PERCENT)
    {
        value = map(dist, TANK_LEVEL_25PERCENT, TANK_LEVEL_50PERCENT, 26, 50);
    }
    else if(dist <= TANK_LEVEL_50PERCENT && dist > TANK_LEVEL_75PERCENT)
    {
        value = map(dist, TANK_LEVEL_50PERCENT, TANK_LEVEL_75PERCENT, 51, 75);
    }
    else if(dist <= TANK_LEVEL_75PERCENT && dist > MAX_TANK_LEVEL)
    {
        value = map(dist, TANK_LEVEL_75PERCENT, MAX_TANK_LEVEL, 76, 100);
    }
    return n_ErrorCode;
}

// Read message on softwareserial port
Std_ReturnType Rte_Call_Read_SoftwareSerial_Port_PT_ESP8266_SoftwareSerial_Port(tUInt8* data, tUInt8 length, SoftwareSerial& rc_SoftwareSerial)
{
    Std_ReturnType n_ErrorCode = E_OK;

    if(rc_SoftwareSerial.available() > 0)
    {
        String str_RecvMsg = rc_SoftwareSerial.readString();
        if(str_RecvMsg.length() == 0)
        {
            n_ErrorCode = E_NOT_OK;
        }
        else
        {
            //Serial.print("RTD: Msg received on SerialPort 3: ");
            
            for(unsigned int i = 0; i < str_RecvMsg.length(); ++i)
            {
                data[i] = (char)str_RecvMsg[i];
                //Serial.print(data[i]);
            }
            //Serial.println();
        }
    }

    return n_ErrorCode;
}

// Read time in milliseconds
tVoid Rte_Call_ESP8266_TimeService_Read_ms(tUInt32& ui32_Value)
{
  ui32_Value = millis();
}

String str_GetRuntime()
{
    tChar str_RunTime[64];
    tUInt32 ui32_CurrentTime = millis();
    tUInt32 ui32_Second = ui32_CurrentTime / 1000;
    tUInt32 ui32_milliSecond = ui32_CurrentTime - ui32_Second;
    tUInt32 ui32_RunHour = ui32_Second / 3600;
    tUInt32 ui32_SecsRemaining = ui32_Second % 3600;
    tUInt32 ui32_RunMinutes = ui32_SecsRemaining / 60;
    tUInt32 ui32_RunSeconds = ui32_SecsRemaining % 60;
    sprintf(str_RunTime, "%02d:%02d:%02d:%02d", ui32_RunHour, ui32_RunMinutes, ui32_RunSeconds, ui32_milliSecond);
    return str_RunTime;
}

// write on Serial port
Std_ReturnType Rte_Call_Write_Serial_Port_PT_ESP8266_Serial_Port(const tChar* data, tUInt8 length, SoftwareSerial& rc_SoftwareSerial)
{
    Std_ReturnType n_ErrorCode = E_NOT_OK;
    tChar c_Debug[126];
    if(NULL != data)
    {
        String str = "";
        String str_Runtime = str_GetRuntime();
        for(int i = 0; i < length; ++i)
        {
            str += data[i];
        }
        tUInt8 ui8_length = str.length();
        tChar c_Msg[ui8_length + 1];
        tChar c_MsgRuntime[str_Runtime.length() + 1];
        str.toCharArray(c_Msg, (ui8_length + 1));
        str += "\r";
        rc_SoftwareSerial.print(str);
        str_Runtime.toCharArray(c_MsgRuntime, (str_Runtime.length() + 1));
        sprintf(c_Debug, "%s  IntDbg: Data send to WebInterface: %s with size= %d\n", c_MsgRuntime, c_Msg, ui8_length);
        Serial.print(c_Debug);
        n_ErrorCode = E_OK;
    }
    return n_ErrorCode;
}



#endif  // __RTE_TYPE_DEF_H__
