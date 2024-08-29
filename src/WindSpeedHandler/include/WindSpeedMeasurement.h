#ifndef WINDSPEEDMEASUREMENT_H
#define WINDSPEEDMEASUREMENT_H

#include "ClientHandler.h"

class WindSpeedMeasurement {

    private:
    ClientHandler& m_rc_ClientHandler;
    static tUInt16 ui16_InterruptCounter;
    static const tUInt16 ui16_RecordTime;
    tBool m_b_AcknowledgePending;


    public:
    WindSpeedMeasurement(ClientHandler& rc_ClientHandler)
    : m_rc_ClientHandler(rc_ClientHandler)
    , m_b_AcknowledgePending(false)
    {

    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author:    Guiyoba Jonathan | Digital Transformation & Technology
                                    Alliance
    @Creation: 29.03.2023
    ----------------------------------------------------------------
    @Function Description:  The function will be call in main setup
    ----------------------------------------------------------------
    @parameter: --
    @Returnvalue: True -- all Init are done
                False -- Init failed
    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    tBool b_OnEntry()
    {
        return m_rc_ClientHandler.b_StartServerConnection();
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author:    Guiyoba Jonathan | Digital Transformation & Technology
                                    Alliance
    @Creation: 29.03.2023
    ----------------------------------------------------------------
    @Function Description:  The function will be call cyclically in loop
    ----------------------------------------------------------------
    @parameter: --
    @Returnvalue: return always true
    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    tBool b_OnDuring()
    {
        tBool b_RetVal = false;
        tUInt8 aui8_DataToSend[WINDSPEED_MESSAGE_LENGTH] = {MSG_ID_WINDSPEED_MESSAGE, 0x0};
        aui8_DataToSend[1] = ui8_GetWindSpeedInMpS();
        
        // send data to the Server and read the ACK
     if(m_rc_ClientHandler.b_IsClientRunning())
        {
            if( !m_b_AcknowledgePending)
            {
                if(m_rc_ClientHandler.b_SendMessage(&aui8_DataToSend[0], sizeof(aui8_DataToSend)))
                {
                    m_b_AcknowledgePending =true;
                }
            }
            if(m_b_AcknowledgePending)
            {
                if(m_rc_ClientHandler.b_HandlerReceivedMessage())
                {
                    b_RetVal = true;
                    m_b_AcknowledgePending = false;
                }
                else
                {
                    Serial.println("WS: No ACK received");
                }
            }
            else
            {
                m_b_AcknowledgePending = m_rc_ClientHandler.b_HandlerReceivedMessage();
                Serial.println("WS: Message can not be send");
            }
        }
        return b_RetVal;
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author:    Guiyoba Jonathan | Digital Transformation & Technology
                                    Alliance
    @Creation: 29.03.2023
    ----------------------------------------------------------------
    @Function Description:  The function will calculate the current wind speed
    ----------------------------------------------------------------
    @parameter: --
    @Returnvalue: return wind speed
    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    tUInt8 ui8_GetWindSpeedInMpS()
    {
        ui16_InterruptCounter = 0;  
        attachInterrupt(digitalPinToInterrupt(ANENOMETERPIN), v_CounterUp, RISING);  
        delay(TIMER_WAIT_3S);
        detachInterrupt(digitalPinToInterrupt(ANENOMETERPIN)); 
        float f_WindSpeedInKmpH = ui16_InterruptCounter / ui16_RecordTime * 2.4;   
        float f_WindSpeedInMpS = f_WindSpeedInKmpH * 0.27;
        Serial.print("counter: ");
        Serial.println(ui16_InterruptCounter);
        Serial.println(f_WindSpeedInMpS);
        return static_cast<tUInt8>(f_WindSpeedInMpS);              
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    @Author:    Guiyoba Jonathan | Digital Transformation & Technology
                                    Alliance
    @Creation: 29.03.2023
    ----------------------------------------------------------------
    @Function Description:  Increase the counter by one
    ----------------------------------------------------------------
    @parameter: --
    @Returnvalue: 
    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    IRAM_ATTR static tVoid v_CounterUp()
    {
        ui16_InterruptCounter++;
    }
};

tUInt16 WindSpeedMeasurement::ui16_InterruptCounter = 0;
const tUInt16 WindSpeedMeasurement::ui16_RecordTime = 3;

#endif