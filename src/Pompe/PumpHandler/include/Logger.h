#ifndef LOGGER_H
#define LOGGER_H

#include "additional_types.h"
#include <Arduino.h>

class Logger
{

public:
// Init 
    tVoid v_Init(tUInt32 ui32_BaudRate = BAUDRATE)
    {
        Serial.begin(ui32_BaudRate);
    }

    // print str 
    tVoid v_Print(String str_Message, tBool b_TimeStamp = true)
    {
        if(b_TimeStamp)
        {
            Serial.print(str_GetRuntime());
            Serial.print(": ");
            Serial.print(str_Message);
        }
        else
        {
            Serial.print(str_Message);
        }
    }

    // print unsigned integer
    tVoid v_Print(tUInt32 ui32_Msg, tBool b_TimeStamp = true)
    {
        if(b_TimeStamp)
        {
            Serial.print(str_GetRuntime());
            Serial.print(": ");
            Serial.print(ui32_Msg);
        }
        else
        {
            Serial.print(ui32_Msg);
        }
    }

    // print Hex form
    tVoid v_PrintHex(tUInt8 ui8_Hex, tBool b_Newline = false)
    {
        if(!b_Newline)
        {
            Serial.println();
            Serial.print("0x");
            Serial.print(ui8_Hex, HEX);
        }
        else
        {
            Serial.println();
            Serial.print("0x");
            Serial.println(ui8_Hex, HEX);
        }
    }

    // print string with max number of prints
    tVoid v_Print(String str_Message, tUInt32& ui32_PrintNumb, tBool b_TimeStamp = true)
    {
        if(ui32_PrintNumb == 0)
        {
            if(b_TimeStamp)
            {
                Serial.print(str_GetRuntime());
                Serial.print(": ");
                Serial.print(str_Message);
            }
            else
            {
                Serial.println(str_Message);
            }
        }
        ui32_PrintNumb++;
    }

    // print unsigned int with max number of prints
    tVoid v_Print(tUInt32 ui32_Msg, tUInt32& ui32_PrintNumb, tBool b_Newline = false)
    {
        if(ui32_PrintNumb == 0)
        {
            if(!b_Newline)
            {
                Serial.println();
                Serial.print(ui32_Msg);
            }
            else
            {
                Serial.println();
                Serial.println(ui32_Msg);
            }
        }
        ui32_PrintNumb++;
    }

    // Get the Runtime
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
};
#endif
