/*
@file additional_types.h
*
*Copyright &copy; MakerFactoryLab
*@author: Gerald Emvoutou
*@date: 25/03/2023 19:33:46
*/
#include <stdint.h>


typedef uint8_t tUInt8;
typedef uint16_t tUInt16;
typedef uint32_t tUInt32;
typedef void tVoid;
typedef bool tBool;
typedef char tChar;
typedef uint8_t Std_ReturnType;

#define ARDUINO_UNO
#define MSG_LENGHT_ACK_MESSAGE_SERVER           4
#define MSG_ID_SOIL_MOISTURE_MESSAGE            0x82
#define MSG_ID_PUMP_STATUS_MESSAGE              0x81
#define MSG_ID_SURROUND_DATA_MESSAGE            0x80
#define SOIL_MOISTURE_PIN                       A0
#define MSG_SOILMOISTURE_LENGTH                 3

#define PUMP_OPENED                             0x01
#define PUMP_CLOSED                             0x0
#define PUMP_OPEN                               0x01
#define PUMP_CLOSE                              0x0

#define BAUDRATE                                115200

#define E_OK                                    0x0U
#define E_NOT_OK                                0x01

#define MAX_TANK_LEVEL                          0xC
#define MIN_TANK_LEVEL                          0x2A

#define TIMER_PUMP                              300
#define TIMER_5_MIN                             6000
#define TIME_OUT_CONNECTION_WIFI                10000
#define TIME_OUT_CONNECTION_SERVER              10
#define TIMEOUT_WAIT_FOR_SERVER_MESSAGE         200
#define TIMEOUT_WAIT_MESSAGE_SERIALPORT         5000

#define MAX_SERVERCONNECTION_ATTEMPTS           5
#define MSG_LENGTH_ACK_MESSAGE_SERVER           4
//#define TIME_MEASUREMENT 

