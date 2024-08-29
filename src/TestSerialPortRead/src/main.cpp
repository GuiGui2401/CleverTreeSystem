#include <Arduino.h>
#include <stdint.h>
#include <SoftwareSerial.h>

typedef uint8_t tUInt8;
typedef uint32_t tUInt32;
typedef char tChar;
typedef void tVoid;

#define TIMEOUT_SERIALPORT  30000

#define RxPin0   D5
#define TxPin0   D6
#define RxPin1   D2
#define TxPin1   D3

SoftwareSerial m_c_SoftwareSerial0(RxPin0, TxPin0);
SoftwareSerial m_c_SoftwareSerial1(RxPin1, TxPin1);

/*-------------------------------------------------
        FUNCTION DECLARATION HERE
--------------------------------------------------*/
tVoid v_ReadSerialPort(SoftwareSerial& rc_SoftwareSerial);
tUInt8 ui8_CharToInt(tChar c_Character);
tUInt8 ui8_ReadMsgLength(tUInt8* ui8_MsgData, tUInt8& ui8_SeqNr);

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  function called once
----------------------------------------------------------------
@parameter: -
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void setup()
{
  Serial.begin(9600);
  m_c_SoftwareSerial0.begin(9600);
  m_c_SoftwareSerial1.begin(9600);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  loop
----------------------------------------------------------------
@parameter: -
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void loop()
{
  v_ReadSerialPort(m_c_SoftwareSerial0);
  delay(100);
  v_ReadSerialPort(m_c_SoftwareSerial1);
  delay(100);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  function concerts a character to his 
                        decimal value
----------------------------------------------------------------
@parameter: tChar c_Character
@Returnvalue: a decimal value of charachter
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tUInt8 ui8_CharToInt(tChar c_Character)
{
    return c_Character - '0';
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  This function calculates the length 
                        of the message sent over the serial
                        interface.
----------------------------------------------------------------
@parameter: tUInt8* ui8_MsgData, tUInt8& ui8_SeqNr
@Returnvalue: the length of the incoming message
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tUInt8 ui8_ReadMsgLength(tChar* ui8_MsgData, tUInt8& ui8_SeqNr)
{
  tUInt8 ui8_Length = 0;
  if(ui8_MsgData[1] == '.' )
    {
      ui8_Length = ui8_CharToInt(ui8_MsgData[0]);
      ui8_SeqNr = ui8_CharToInt(ui8_MsgData[2]);
    }
    else if(ui8_MsgData[2] == '.')
    {
      ui8_Length = (ui8_CharToInt(ui8_MsgData[0]) * 10) + ui8_CharToInt(ui8_MsgData[1]);
      ui8_SeqNr = ui8_CharToInt(ui8_MsgData[3]);
    }
    return ui8_Length;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 26.03.2023
----------------------------------------------------------------
@Function Description:  Reads the incoming message, that was sent
                      via the serial interface and store the values 
                      in an local array. 
----------------------------------------------------------------
@parameter: SoftwareSerial& rc_SoftwareSerial
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_ReadSerialPort(SoftwareSerial& rc_SoftwareSerial)
{
  char aui8_RecvData[20] = {0};
  tUInt8 ui8_Length = 0;
  tUInt8 ui8_SeqNr = 0;
  if(rc_SoftwareSerial.available() > 0)
  {
    /*
    Read on the Serial port while the incomingsbytes are available 
    and store in an local array
    */
    do
    {
      for(int i = 0; i < sizeof(aui8_RecvData); ++i)
      {
        aui8_RecvData[i] = (char)rc_SoftwareSerial.read();
      }
    } while(rc_SoftwareSerial.read() == 0xFF);

    Serial.println("");
    rc_SoftwareSerial.flush();

    // Reads the length and the sequence number
    ui8_Length = ui8_ReadMsgLength(aui8_RecvData, ui8_SeqNr);
    Serial.print("TSPR: length= ");
    Serial.println(ui8_Length);
    Serial.print("TSPR: Message received= ");

    // Copy the incoming message in a new array 
    char ui8_CopyArray[ui8_Length];
    memcpy(ui8_CopyArray, aui8_RecvData, sizeof(ui8_CopyArray));

    // Print the incoming message
    for(int i = 0; i < sizeof(ui8_CopyArray); ++i)
    {
      Serial.print(ui8_CopyArray[i]);
    }
    Serial.println("");

    // send a ACK to the sender
    rc_SoftwareSerial.print('1');
    rc_SoftwareSerial.flush();
  }
}
