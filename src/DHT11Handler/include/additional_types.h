/*
@file additional_types.h
*
*Copyright &copy; MakerFactoryLab
*@author: Gerald Emvoutou
*@date: 25/03/2023 19:33:46
*/
#include <stdint.h>
#include <Arduino.h>

//#define TIME_MEASUREMENT 

typedef uint8_t tUInt8;
typedef uint16_t tUInt16;
typedef uint32_t tUInt32;
typedef void tVoid;
typedef bool tBool;
typedef char tChar;
typedef uint8_t Std_ReturnType;

#define DHTPin                                  D5
#define DHTTYPE                                 DHT11
#define MQ2Pin                                  A0

#define ARDUINO_UNO
#define MSG_LENGTH_ACK_MESSAGE_SERVER           4
#define ACK_MESSAGE_LENGTH                      2
#define DHT11_DATA_MESSAGE_LENGTH               9
#define MSG_LENGTH_CYCLIC_MESSAGE               16
#define MSG_ID_SOIL_MOISTURE_MESSAGE            0x82
#define MSG_ID_PUMP_STATUS_MESSAGE              0x81
#define MSG_ID_DHT11_DATA_MESSAGE               0x80
#define MSG_ID_WINDSPEED_MESSAGE                0x86
#define MSG_ID_MQ2_MESSAGE                      0x83
#define MSG_ID_SUNLIGHT_MESSAGE                 0x84

#define PUMP_OPENED                             0x01
#define PUMP_CLOSED                             0x0
#define PUMP_OPEN                               0x01
#define PUMP_CLOSE                              0x0

#define BAUDRATE                                115200
#define BAUDRATE_TXRX                           9600

#define E_OK                                    0x0U
#define E_NOT_OK                                0x01

#define MAX_TANK_LEVEL                          0x4
#define MIN_TANK_LEVEL                          0x29
#define TANK_LEVEL_25PERCENT                    0x1D
#define TANK_LEVEL_3PERCENT                     0x28
#define TANK_LEVEL_7PERCENT                     0x25
#define TANK_LEVEL_11PERCENT                    0x24
#define TANK_LEVEL_14PERCENT                    0x23
#define TANK_LEVEL_17PERCENT                    0x21
#define TANK_LEVEL_21PERCENT                    0x1F
#define TANK_LEVEL_50PERCENT                    0x17
#define TANK_LEVEL_75PERCENT                    0xA

#define TIMER_PUMP                              80000
#define TIMER_5_MIN                             300000
#define TIME_OUT_CONNECTION_WIFI                60000
#define MAX_ATTEMPTS_CONNECTION_SERVER          10
#define MAX_ATTEMPTS_CONNECTION_WIFI            5
#define TIMER_500MS                             500
#define TIMEOUT_WAIT_FOR_SERVER_MESSAGE         2000
#define TIMEOUT_WAIT_MESSAGE_SERIALPORT         5000

#define TIMEOUT_ACKNOWLEDGE                     200

#define MAX_NUMB_DOT                            15
#define MAX_SERVERCONNECTION_ATTEMPTS           5
#define MIN_TANKLEVEL_FOR_TANK_ACTIVATION       5
#define LENGTH_PREFIX_SEQUENCENUMBER0TO2        4
#define LENGTH_PREFIX_SEQUENCENUMBER3           3

#define CLVT_DECL_OVERRIDE                      override


