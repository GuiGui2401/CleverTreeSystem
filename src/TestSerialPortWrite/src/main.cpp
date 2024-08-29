#include <Arduino.h>
#include <stdint.h>
#include <SoftwareSerial.h>

#define RxPin   D7
#define TxPin   D8

SoftwareSerial m_c_SoftwareSerial(RxPin, TxPin);

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 18.06.2023
----------------------------------------------------------------
@Function Description:  function called once
----------------------------------------------------------------
@parameter: -
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void setup()
{
  m_c_SoftwareSerial.begin(9600);
  Serial.begin(115200);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 18.06.2023
----------------------------------------------------------------
@Function Description:  loop
----------------------------------------------------------------
@parameter: -
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
uint32_t counter = 0;
void loop()
{
  // Request a pump activation
  m_c_SoftwareSerial.print("01\r");
  Serial.println("Pump activation requested");
  delay(5000);
  if((counter % 4) == 0)
  {
    m_c_SoftwareSerial.print("1\r");
  }
  delay(1000);
  counter++;
  m_c_SoftwareSerial.flush();
}
