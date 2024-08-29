
#include <ESPAsyncWebSrv.h>
#include <FS.h>
#include "additional_types.h"
#include <SoftwareSerial.h>

#define ssid "WiFi_CleverTree"
#define passwd "02655826"
#define RxPin0   D5
#define TxPin0   D6
#define RxPin1   D2
#define TxPin1   D3

/*---------------------------------------------------------
            STRUCT DECLARATION HERE
----------------------------------------------------------*/
struct DataServer
{
  String ui8_MsgID = "NaN";
  String ui8_PumPActivationRequested = "NaN";
  String ui8_SoilMoistureValue_01 = "NaN";
  String ui8_SoilMoistureValue_02 = "NaN";
  String ui8_SoilMoistureValue_03 = "NaN";
  String ui8_WaterLevel = "NaN";
  String ui8_PumpStatus = "NaN";
  String ui8_WindSpeed = "NaN";
  String ui8_Temperature = "NaN";
  String ui8_Humidity = "NaN";
  String ui8_IR = "NaN";
  String ui8_UV = "NaN";
  String ui8_Visible = "NaN";
  String ui8_CO = "NaN";
  String ui8_LPG = "NaN";
  String ui8_Smoke = "NaN";
};

/*---------------------------------------------------------
            GLOBAL VARIABLE DECLARATION HERE
----------------------------------------------------------*/
SoftwareSerial m_c_SoftwareSerial0(RxPin0, TxPin0);
SoftwareSerial m_c_SoftwareSerial1(RxPin1, TxPin1);
const tChar* PARAM_INPUT_1 = "state";
DataServer m_c_DataServer;
IPAddress local_IP(192, 168, 1, 239);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
tUInt32 m_ui32_ReceivedCounter = 0;
tUInt32 m_ui32_SendCounter = 0;
String str_MsgToSend = "";
AsyncWebServer server(80);          // serveur asynchrone 

/*---------------------------------------------------------
              FUNCTION DECLARATION HERE
----------------------------------------------------------*/
tVoid v_WriteSerial(String str_ButtonValue);
tVoid v_ReadSerialPort(SoftwareSerial& rc_SoftwareSerial);
tUInt8 ui8_CharToInt(tChar c_Character);
tBool b_MessageParsen(tChar* pui8_Msg, tUInt8 ui8_length);
tUInt8 ui8_ReadMsgLength(tChar* ui8_MsgData, tUInt8& ui8_SeqNr);
String str_GetRuntime();
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
  if (!WiFi.config(local_IP, gateway, subnet)) 
  {
    Serial.print(str_GetRuntime());
    Serial.println(" STA Failed to configure");
  }
  Serial.begin(115200);
  m_c_SoftwareSerial0.begin(9600);
  m_c_SoftwareSerial1.begin(9600);
  //******************************* vérification des fichiers dans la mémoire **********
  if(!SPIFFS.begin())
  {
    Serial.print(str_GetRuntime());
    Serial.println(" Erreur SPIFFS...");
    return;
  }

  //************** connexion au wifi **************
  WiFi.begin(ssid, passwd);
  Serial.print(str_GetRuntime());
  Serial.print(" try connection to WiFi...");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print(str_GetRuntime());
  Serial.print(" IP Address: "); 
  Serial.println(WiFi.localIP());
  //************************  chargement des fichiers ****************** 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    v_ReadSerialPort(m_c_SoftwareSerial0);
    v_ReadSerialPort(m_c_SoftwareSerial1);
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/jquery.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/jquery.js", "text/javascript");
  });

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) 
  {
    String str_InputMsg;
    String str_InputParam;
    // GET input1 value on <ESP_IP>/update?state=<str_InputMsg>
    if (request->hasParam(PARAM_INPUT_1)) 
    {
      str_InputMsg = request->getParam(PARAM_INPUT_1)->value();
      str_InputParam = PARAM_INPUT_1;
      v_WriteSerial(str_InputMsg);
    }
    else 
    {
      str_InputMsg = "No message sent";
      str_InputParam = "none";
    }
    Serial.print(str_GetRuntime());
    Serial.print(" WEB: ");
    Serial.println(str_InputMsg);
    request->send(200, "text/plain", "OK");
  });
  
  server.on("/lirPump", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", m_c_DataServer.ui8_PumpStatus);
  });

  server.on("/lirTank", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", m_c_DataServer.ui8_WaterLevel);
  });

  server.on("/lirSoil1", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", m_c_DataServer.ui8_SoilMoistureValue_01);
  });

  server.on("/lirSoil2", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", m_c_DataServer.ui8_SoilMoistureValue_02);
  });

  server.on("/lirSoil3", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", m_c_DataServer.ui8_SoilMoistureValue_03);
  });

  server.on("/lirTemp", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", m_c_DataServer.ui8_Temperature);
  });

  server.on("/lirHum", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", m_c_DataServer.ui8_Humidity);
  });

  server.on("/lirWind", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", m_c_DataServer.ui8_WindSpeed);
  });

  server.on("/lirIr", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", m_c_DataServer.ui8_IR);
  });
    
  server.on("/lirUv", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", m_c_DataServer.ui8_UV);
  });

  server.on("/lirLight", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", m_c_DataServer.ui8_Visible);
  });

  server.on("/lirCarbon", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", m_c_DataServer.ui8_CO);
  });

  server.on("/lirLpg", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", m_c_DataServer.ui8_LPG);
  });

  server.on("/lirSmoke", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", m_c_DataServer.ui8_Smoke);
  });
  server.begin();
  Serial.print(str_GetRuntime());
  Serial.println(" WEB: Server is running...");
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
void loop()
{
 
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Jonathan Guiyoba | Digital Transformation & Technology
                                Alliance
@Creation: 23.04.2023
----------------------------------------------------------------
@Function Description:  send a charachter string to serial port
----------------------------------------------------------------
@parameter: String str_ButtonValue
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tVoid v_WriteSerial(String str_ButtonValue)
{
    str_MsgToSend = str_ButtonValue;
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
  tChar aui8_RecvData[20] = {0};
  tUInt8 ui8_Length = 0;
  tUInt8 ui8_SeqNr = 0;
  if(rc_SoftwareSerial.available() > 0)
  {
    /*
    Read on the Serial port while the incomingsbytes are available 
    and store in an local array
    */
    /*do
    {
      for(int i = 0; i < sizeof(aui8_RecvData); ++i)
      {
        aui8_RecvData[i] = (tChar)rc_SoftwareSerial.read();
      }
    } while(rc_SoftwareSerial.read() == 0xFF);*/
    String str_RecvMsg = rc_SoftwareSerial.readStringUntil('\r');
    for(tUInt8 ui8_Position = 0; ui8_Position < str_RecvMsg.length(); ++ui8_Position)
    {
      aui8_RecvData[ui8_Position] = str_RecvMsg[ui8_Position];
    }

    Serial.println("");
    rc_SoftwareSerial.flush();

    // Reads the length and the sequence number
    ui8_Length = ui8_ReadMsgLength(aui8_RecvData, ui8_SeqNr);
    Serial.print(str_GetRuntime());
    Serial.print(" WEB: length= ");
    Serial.print(ui8_Length);
    Serial.print(" Seq Number= ");
    Serial.println(ui8_SeqNr);
    Serial.print(str_GetRuntime());
    Serial.print(" WEB: Message received= ");

    // Copy the incoming message in a new array 
    tChar ui8_CopyArray[ui8_Length];
    memcpy(ui8_CopyArray, aui8_RecvData, sizeof(ui8_CopyArray));

    // Print the incoming message
    for(tUInt8 ui8_Pos = 0; ui8_Pos < sizeof(ui8_CopyArray); ++ui8_Pos)
    {
      Serial.print((tChar)ui8_CopyArray[ui8_Pos]);
    }
    Serial.println("");

    // If the message is parsed successfully send a ACK otherwise print an error message
    if(b_MessageParsen(ui8_CopyArray, sizeof(ui8_CopyArray), ui8_SeqNr))
    {
      String str_SendMsg  = "1";
      if(str_MsgToSend.length() == 0)
      {
        str_MsgToSend = "00";
      }
      str_SendMsg += str_MsgToSend;
      str_SendMsg += "\r";
      
      
      rc_SoftwareSerial.print(str_SendMsg);
      Serial.print(str_GetRuntime());
      Serial.print(" WEB: Message sent ");
      Serial.println(str_SendMsg);
      rc_SoftwareSerial.flush();
    }
    else
    {
      Serial.print(str_GetRuntime());
      Serial.println(" WEB: Parsen not successfull");
    }
  }
  else
  {
    Serial.print(str_GetRuntime());
    Serial.print(" WEB: Serial port not available");
  }
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
@Creation: 23.04.2023
----------------------------------------------------------------
@Function Description:  convert a tChar to numeric value
----------------------------------------------------------------
@parameter: tChar c_Character
@Returnvalue: decimal value of charachter
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tUInt8 ui8_CharToInt(tChar c_Character)
{
  return c_Character - '0';
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 23.04.2023
----------------------------------------------------------------
@Function Description:  parse incoming message from server
----------------------------------------------------------------
@parameter: tChar* pui8_Msg, tUInt8 ui8_length
@Returnvalue: true if the message was parsed successfully
              false if the message was not parsed successfully
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tBool b_MessageParsen(tChar* pui8_Msg, tUInt8 ui8_length, tUInt8 ui8_SeqNr)
{
  tBool b_MsgParsenSuccessful = false;
  tUInt8 ui8_CurrentPosition = 0;
  tUInt8 aui8_IndexPos[6];
  String str_Temp = "";

  // store the position of delimiter
  for(unsigned int i = 0; i < ui8_length; i++)
  {
    str_Temp += pui8_Msg[i];
    if(pui8_Msg[i]== '.')
    {
      aui8_IndexPos[ui8_CurrentPosition++] = i;
    }
  }

  if(ui8_SeqNr == 0)
  {
    tUInt8 ui8_PayloadLength = (str_Temp.substring(0, aui8_IndexPos[1])).toInt();
    m_c_DataServer.ui8_PumPActivationRequested = str_Temp.substring(aui8_IndexPos[1]+1, aui8_IndexPos[2]);
    m_c_DataServer.ui8_SoilMoistureValue_01 = str_Temp.substring(aui8_IndexPos[2]+1, aui8_IndexPos[3]);
    m_c_DataServer.ui8_SoilMoistureValue_02 = str_Temp.substring(aui8_IndexPos[3]+1, aui8_IndexPos[4]);
    m_c_DataServer.ui8_SoilMoistureValue_03 = str_Temp.substring(aui8_IndexPos[4]+1, ui8_PayloadLength);
    b_MsgParsenSuccessful = true;
  }
  else if(ui8_SeqNr == 1)
  {
    tUInt8 ui8_PayloadLength = (str_Temp.substring(0, aui8_IndexPos[1])).toInt();
    m_c_DataServer.ui8_WaterLevel = str_Temp.substring(aui8_IndexPos[1]+1, aui8_IndexPos[2]);
    m_c_DataServer.ui8_PumpStatus = str_Temp.substring(aui8_IndexPos[2]+1, aui8_IndexPos[3]);
    m_c_DataServer.ui8_WindSpeed = str_Temp.substring(aui8_IndexPos[3]+1, aui8_IndexPos[4]);
    m_c_DataServer.ui8_Temperature = str_Temp.substring(aui8_IndexPos[4]+1, ui8_PayloadLength);
    b_MsgParsenSuccessful = true;
  }
  else if(ui8_SeqNr == 2)
  {
    tUInt8 ui8_PayloadLength = (str_Temp.substring(0, aui8_IndexPos[1])).toInt();
    m_c_DataServer.ui8_Humidity = str_Temp.substring(aui8_IndexPos[1]+1, aui8_IndexPos[2]);
    m_c_DataServer.ui8_IR = str_Temp.substring(aui8_IndexPos[2]+1, aui8_IndexPos[3]);
    m_c_DataServer.ui8_UV = str_Temp.substring(aui8_IndexPos[3]+1, aui8_IndexPos[4]);
    m_c_DataServer.ui8_Visible = str_Temp.substring(aui8_IndexPos[4]+1, ui8_PayloadLength);
    b_MsgParsenSuccessful = true;
  }
  else if(ui8_SeqNr == 3)
  {
    tUInt8 ui8_PayloadLength = (str_Temp.substring(0, aui8_IndexPos[1])).toInt();
    m_c_DataServer.ui8_CO = str_Temp.substring(aui8_IndexPos[1]+1, aui8_IndexPos[2]);
    m_c_DataServer.ui8_LPG = str_Temp.substring(aui8_IndexPos[2]+1, aui8_IndexPos[3]);
    m_c_DataServer.ui8_Smoke = str_Temp.substring(aui8_IndexPos[3]+1, ui8_PayloadLength);
    b_MsgParsenSuccessful = true;
  }
  else
  {
    Serial.print(str_GetRuntime());
    Serial.println(" WEB: no valid SeqNr");
  }

  tChar c_Debug[256];
  Serial.print(str_GetRuntime());
  sprintf(c_Debug, " WEB: PAR=%.2X SM01=%.2X SM02=%.2X SM03=%.2X WL=%.2X PS=%.2X WS=%.2X\n T=%.2X H=%.2X IR=%.2X UV=%.2X VS=%.2X CO=%.2X LPG=%.2X SMOKE=%.2X \n",
  m_c_DataServer.ui8_PumPActivationRequested.toInt(), m_c_DataServer.ui8_SoilMoistureValue_01.toInt(), m_c_DataServer.ui8_SoilMoistureValue_02.toInt(),m_c_DataServer.ui8_SoilMoistureValue_03.toInt()
  , m_c_DataServer.ui8_WaterLevel.toInt(), m_c_DataServer.ui8_PumpStatus.toInt(), m_c_DataServer.ui8_WindSpeed.toInt(), m_c_DataServer.ui8_Temperature.toInt()
  , m_c_DataServer.ui8_Humidity.toInt(), m_c_DataServer.ui8_IR.toInt(), m_c_DataServer.ui8_UV.toInt(), m_c_DataServer.ui8_Visible.toInt()
  , m_c_DataServer.ui8_CO.toInt(), m_c_DataServer.ui8_LPG.toInt(), m_c_DataServer.ui8_Smoke.toInt());
  Serial.print(c_Debug);
  return b_MsgParsenSuccessful;
}

// Get the Runtime
String str_GetRuntime()
{
    tChar str_RunTime[64];
    tUInt32 ui32_Second = millis() / 1000;
    tUInt32 ui32_RunHour = ui32_Second / 3600;
    tUInt32 ui32_SecsRemaining = ui32_Second % 3600;
    tUInt32 ui32_RunMinutes = ui32_SecsRemaining / 60;
    tUInt32 ui32_RunSeconds = ui32_SecsRemaining % 60;
    sprintf(str_RunTime, "%02d:%02d:%02d", ui32_RunHour, ui32_RunMinutes, ui32_RunSeconds);

    return str_RunTime;
}
