/*
@file additional_types.h
*
*Copyright &copy; MakerFactoryLab
*@author: Gerald Emvoutou
*@date: 25/03/2023 19:33:46
*/

#ifndef ADDITIONAL_TYPES_H
#define ADDITIONAL_TYPES_H
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

byte gauge_empty[8] =  {B11111, B00000, B00000, B00000, B00000, B00000, B00000, B11111};    // empty middle piece
byte gauge_fill_1[8] = {B11111, B10000, B10000, B10000, B10000, B10000, B10000, B11111};    // filled gauge - 1 column
byte gauge_fill_2[8] = {B11111, B11000, B11000, B11000, B11000, B11000, B11000, B11111};    // filled gauge - 2 columns
byte gauge_fill_3[8] = {B11111, B11100, B11100, B11100, B11100, B11100, B11100, B11111};    // filled gauge - 3 columns
byte gauge_fill_4[8] = {B11111, B11110, B11110, B11110, B11110, B11110, B11110, B11111};    // filled gauge - 4 columns
byte gauge_fill_5[8] = {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};    // filled gauge - 5 columns
byte gauge_left[8] =   {B11111, B10000, B10000, B10000, B10000, B10000, B10000, B11111};    // left part of gauge - empty
byte gauge_right[8] =  {B11111, B00001, B00001, B00001, B00001, B00001, B00001, B11111};    // right part of gauge - empty

byte gauge_mask_left[8] = {B01111, B11111, B11111, B11111, B11111, B11111, B11111, B01111};  // mask for rounded corners for leftmost character
byte gauge_mask_right[8] = {B11110, B11111, B11111, B11111, B11111, B11111, B11111, B11110}; // mask for rounded corners for rightmost character

byte warning_icon[8] = {B00100, B00100, B01110, B01010, B11011, B11111, B11011, B11111};     // warning icon - just because we still have one custom character left

byte gauge_left_dynamic[8];   // left part of gauge dynamic - will be set in the loop function
byte gauge_right_dynamic[8];  // right part of gauge dynamic - will be set in the loop function

tUInt8 cpu_gauge = 0;       // value for the CPU gauge
char buffer[10];         // helper buffer to store C-style strings (generated with sprintf function)
tUInt8 move_offset = 0;     // used to shift bits for the custom characters

const tUInt8 gauge_size_chars = 10;       // width of the gauge in number of characters
char gauge_string[gauge_size_chars+1]; // string that will include all the gauge character to be printed

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
#define MSG_ID_LCD_MESSAGE                      0x87

#define WAIT_500_MS                             500
#define WAIT_5000_MS                            5000
#define DISPLAY_TIME                            30000
#define MAX_DISPLAY_NUMBER                      2

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


#endif //ADDITIONAL_TYPES_H