#include <SabertoothSimplified.h>
#include <stdint.h>

#define BAUDRATE    9600

const uint8_t ui8_Motor1 = 1;
SabertoothSimplified m_c_SabertoothSimplified;

void setup()
{
    SabertoothTXPinSerial.begin(BAUDRATE);
}


void loop()
{
    m_c_SabertoothSimplified.motor(ui8_Motor1, 127);
    delay(2000);
    m_c_SabertoothSimplified.motor(ui8_Motor1, 0);
    delay(2000);
}