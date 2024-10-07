from random import random
import socket
import threading
import time
ui32_StartTimeStamp = time.time()
import os
import sys
from datetime import datetime
if(sys.argv[1] != "-without" and sys.argv[2] !="mariaDB"):
    import mariadb

#comment the line below to start the server manually
#import nothing


HEADER = 64
SERVER ="192.168.1.200"
PORT1 = 50011
ADDR = (SERVER, PORT1)
FORMAT = 'utf-8'
DISCONNECT = "break"
CLIENT_MOIS_SENSOR0 = "192.168.1.201"
CLIENT_MOIS_SENSOR1="192.168.1.202"
CLIENT_MOIS_SENSOR2="192.168.1.203"
CLIENT_PUMP = "192.168.1.210"
CLIENT_DHT11 = "192.168.1.213"
CLIENT_DASHBOARD = "192.168.1.101"

CLIENT_ANEMOMETER = "192.168.1.211"

CLIENT_LCD = "192.168.1.215"

DHT11_DATA_MSG_ID = 0x80
PUMP_STATUS_MSG_ID = 0x81
SOIL_MOISTURE_MSG_ID = 0x82
WINDSPEED_MSG_ID = 0x86
LCD_MSG_ID = 0x87


server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(ADDR)

#--------------------------------------------------------------------------
# @author: Gerald Emvoutou
# @creation date: 09-04-2023
#--------------------------------------------------------------------------
# @function description: timestamp in format [YYYY-MM-DD hh:mm:ss]
#--------------------------------------------------------------------------
# @parameter: --
# @return value: timestamp
#--------------------------------------------------------------------------
def TimeStamp():
    LocalTime = time.localtime()
    str_CurrentTime : str = "["
    month = LocalTime.tm_mon
    if(month < 10):
        month = "0"+str(month)
    year = str(LocalTime.tm_year)
    day = LocalTime.tm_mday
    if(day < 10):
        day = "0"+str(day)
    str_CurrentTime += str(year)+"-"+str(month)+"-"+str(day)
    hour = LocalTime.tm_hour
    if (hour < 10):
        hour = "0"+str(hour)
    min = LocalTime.tm_min
    if (min < 10):
        min = "0"+str(min)
    sec = LocalTime.tm_sec
    if (sec < 10):
        sec = "0"+str(sec)
    str_CurrentTime += " "+str(hour)+":"+str(min) + ":"+str(sec) +"]"
    return str_CurrentTime

#--------------------------------------------------------------------------
# @author: Gerald Emvoutou
# @creation date: 09-04-2023
#--------------------------------------------------------------------------
# @function description: checks if the file is older than 14 days
#--------------------------------------------------------------------------
# @parameter: --
# @return value: true if it is older otherwise false
#--------------------------------------------------------------------------
def b_IsOlderThanTwoWeeks(path_file: str):

    b_Isolder: bool = False
    initTimeStamp : str = "190001010000"
    start_time = datetime.strptime(initTimeStamp, "%Y%m%d%H%M")
    current_Time  = datetime.strptime(GetCurrentDateAndTime(True), "%Y%m%d%H%M")
    if os.path.exists(path_file):
        for i in range(len(path_file)):
            if (path_file[i] == "_"):
                str_FileTimeStamp = path_file[i+1:i+13]
                start_time = datetime.strptime(str_FileTimeStamp, "%Y%m%d%H%M")
                break
    else:
        print("File doesn't exists!!!")

    if(str_FileTimeStamp[0:4] != "1900"):
        if(current_Time - start_time).days >= 7:
            b_Isolder = True
    else:
        print("Cannot extract timestamp!!!")

    return b_Isolder

#--------------------------------------------------------------------------
# @author: Gerald Emvoutou
# @creation date: 09-04-2023
#--------------------------------------------------------------------------
# @function description: start the connectionto the mariaDB server
#--------------------------------------------------------------------------
# @parameter: --
# @return value: a cursor from the server
#--------------------------------------------------------------------------
def StartMariaDBServer():
    if(sys.argv[1] != "-without" and sys.argv[2] !="mariaDB"):
        
        try :
            mydb = mariadb.connect(user='your_user', password='your_password', host="127.0.0.1", port=3306, database='clever_tree')
            mycursor = mydb.cursor()
            return mycursor, mydb
        except mariadb.Error as e:
            print(f"Error connecting to MariaDB Platform: {e}")
            sys.exit(1)


#--------------------------------------------------------------------------
# @author: Gerald Emvoutou
# @creation date: 09-04-2023
#--------------------------------------------------------------------------
# @function description: Get the current date and time
#--------------------------------------------------------------------------
# @parameter: date : bool = False
# @return value: return the date and the time
#--------------------------------------------------------------------------
def GetCurrentDateAndTime(date : bool = False):
    LocalTime = time.localtime()
    str_CurrentTime : str = ""

    if date == True:
        month = LocalTime.tm_mon
        if(month < 10):
            month = "0"+str(month)
        year = str(LocalTime.tm_year)
        day = LocalTime.tm_mday
        if(day < 10):
            day = "0"+str(day)
        str_CurrentTime = str(year)+str(month)+str(day)

    hour = LocalTime.tm_hour
    if (hour < 10):
        hour = "0"+str(hour)
    min = LocalTime.tm_min
    if (min < 10):
        min = "0"+str(min)
    str_CurrentTime += str(hour)+str(min)
    return str_CurrentTime

#--------------------------------------------------------------------------
# @author: Gerald Emvoutou
# @creation date: 09-04-2023
#--------------------------------------------------------------------------
# @function description: converts an array of int to a string 
#--------------------------------------------------------------------------
# @parameter: ArrayData : list
# @return value: Array as String
#--------------------------------------------------------------------------
def ConvertArrayToString(ArrayData : list):
    str_Msg = '.'.join([str(i) for i in ArrayData])
    return str_Msg

#--------------------------------------------------------------------------
# @author: Gerald Emvoutou
# @creation date: 09-04-2023
#--------------------------------------------------------------------------
# @function description: converts a string to array of int
#--------------------------------------------------------------------------
# @parameter: strMsg : str
# @return value: Array of int
#--------------------------------------------------------------------------
def ConvertStringtoArray(strMsg : str):
    DataContent = []
    currentIndex = 0
    oldIndex = 0
    for index in range(len(strMsg)):
        currentIndex = index
        if (strMsg[index] == '.'):
            DataContent.append(strMsg[oldIndex:currentIndex])
            oldIndex = currentIndex+1
    DataContent.append(strMsg[oldIndex:])
    return DataContent

#--------------------------------------------------------------------------
# @author: Gerald Emvoutou
# @creation date: 09-04-2023
#--------------------------------------------------------------------------
# @function description: compare two msg that was send from client
#--------------------------------------------------------------------------
# @parameter: Old_DataContent : list , New_DataContent : list
# @return value: Array of int
#--------------------------------------------------------------------------
def IncomingMsgIsChanged(Old_DataContent : list , New_DataContent : list):

    IsChanged : bool = False
    if len(Old_DataContent) != len(New_DataContent):
        IsChanged = True
    for i in range(len(Old_DataContent)):
        if Old_DataContent[i] != New_DataContent[i]:
            IsChanged = True
    return IsChanged

#--------------------------------------------------------------------------
# @author: Gerald Emvoutou
# @creation date: 09-04-2023
#--------------------------------------------------------------------------
# @function description: sends message to client
#--------------------------------------------------------------------------
# @parameter: conn : socket, New_DataContent : list, 
#             Last_DataContent : list, delayTimeInMs : int
# @return value: --
#--------------------------------------------------------------------------
def SendMsgToClient(conn : socket, DataContentToSend : list, delayTimeInMs : int = 0):

    delayTimeInSec = delayTimeInMs / 1000
    s_OutComingMsg = ConvertArrayToString(DataContentToSend)
    conn.send(s_OutComingMsg.encode(FORMAT))
    time.sleep(delayTimeInSec)

#--------------------------------------------------------------------------
# @author: Gerald Emvoutou
# @creation date: 09-04-2023
#--------------------------------------------------------------------------
# @function description: sends message to client
#--------------------------------------------------------------------------
# @parameter: conn : socket, New_DataContent : list, 
#             Last_DataContent : list, delayTimeInMs : int
# @return value: --
#--------------------------------------------------------------------------
def InitArrayWithElements(DataContent : list, maxElements : int, dataTyp ):
    for i in range(maxElements):
        if dataTyp == "boolean":
            DataContent.append(False)
        else:
            DataContent.append(0)

#--------------------------------------------------------------------------
# @author: Gerald Emvoutou
# @creation date: 09-04-2023
#--------------------------------------------------------------------------
# @function description: sends message to client
#--------------------------------------------------------------------------
# @parameter: conn : socket, New_DataContent : list, 
#             Last_DataContent : list, delayTimeInMs : int
# @return value: --
#--------------------------------------------------------------------------
def WriteATheArrayPosition(DataContent : list, position : int, value : int):
    DataContent[position] = value

#--------------------------------------------------------------------------
# @author: Jonathan Guiyoba
# @creation date: 09-04-2023
#--------------------------------------------------------------------------
# @function description: This function will be used in the start function 
#                       just after the line where we accept new connections
#--------------------------------------------------------------------------
# @parameter: conn : socket, New_DataContent : list, 
#             Last_DataContent : list, delayTimeInMs : int
# @return value: --
#--------------------------------------------------------------------------
def GetIPAdressClient(ClientAddr : socket, ListIPAdressClient : list):
        # Add customer address to table
        ListIPAdressClient.append(ClientAddr)
        # Return array with client IP addresses
        return ListIPAdressClient

#--------------------------------------------------------------------------
# @author: Gerald Emvoutou
# @creation date: 09-04-2023
#--------------------------------------------------------------------------
# @function description: 
#--------------------------------------------------------------------------
# @parameter: conn : socket, addr: socket, DataContent: list
# @return value: --
#--------------------------------------------------------------------------
def handle_client(conn : socket, addr: socket, DataContent: list, PumpactivationRequested: list, file_path: str):
    #ui8_OldPumpStatus = 0xff
    #old_ClientMoisSensor1 = [0xff, 0xff, 0xff]
    #old_ClientMoisSensor2 = [0xff, 0xff, 0xff]
    #old_ClientMoisSensor3 = [0xff, 0xff, 0xff]
    #old_ClientPump = [0xff, 0xff, 0xff]
    #old_DHT11 = [0xff, 0xff, 0xff,0xff, 0xff, 0xff]
    #old_WindSpeed = [0xff, 0xff]
    
    connected = True
    while connected:
        s_IncomingMsg = conn.recv(HEADER).decode(FORMAT)
        MsgInArray = ConvertStringtoArray(s_IncomingMsg)

        with open(file_path, "a") as fichier :
            Logs = TimeStamp() +" Message receive from client= "+str(addr[0])+" message length= "+ str(len(MsgInArray))+ "\n"
            fichier.write(Logs)
            print(Logs)
            fichier.close()
        
        if (CLIENT_MOIS_SENSOR0 == addr[0]):
            AckMsg = [0xC0, 0x0, SOIL_MOISTURE_MSG_ID, 0x0]
            if len(MsgInArray) == 3:
                PumpactivationRequested[0] = MsgInArray[1]
                DataContent[1] = MsgInArray[2]                      # ui8_SoilMoistureValue_01
                DataContent[0] = PumpActivationIsRequested(DataContent)
            else:
                AckMsg = [0xC0, 0x0, SOIL_MOISTURE_MSG_ID, 0x1]
            #if(IncomingMsgIsChanged(old_ClientMoisSensor1, MsgInArray) == True):
                #old_ClientMoisSensor1 = MsgInArray
            with open(file_path, "a") as fichier :
                Logs = TimeStamp() +" ACK send to client "+str(addr[0])+"\t Pump request= "+ str(DataContent[0]) +", Soil Moisture 00 value= "+str(DataContent[2]) + "\n"
                fichier.write(Logs)
                print(Logs)
                fichier.close()
            SendMsgToClient(conn, AckMsg)

        elif (CLIENT_MOIS_SENSOR1 == addr[0]):
            AckMsg = [0xC0, 0x0, SOIL_MOISTURE_MSG_ID, 0x0]
            if len(MsgInArray) == 3:
                PumpactivationRequested[1] = MsgInArray[1]
                DataContent[2] = MsgInArray[2]              # ui8_SoilMoistureValue_02
                DataContent[0] = PumpActivationIsRequested(DataContent)
            else:
                AckMsg = [0xC0, 0x0, SOIL_MOISTURE_MSG_ID, 0x1]
            #if(IncomingMsgIsChanged(old_ClientMoisSensor2, MsgInArray) == True):
                #old_ClientMoisSensor2 = MsgInArray
            with open(file_path, "a") as fichier :
                Logs = TimeStamp() +" ACK send to client "+str(addr[0])+"\t Pump request= "+ str(DataContent[0]) +", Soil Moisture 01 value= "+str(DataContent[2]) + "\n"
                fichier.write(Logs)
                print(Logs)
                fichier.close()
            SendMsgToClient(conn, AckMsg)

        elif (CLIENT_MOIS_SENSOR2 == addr[0]):
            AckMsg = [0xC0, 0x0, SOIL_MOISTURE_MSG_ID, 0x0]
            if len(MsgInArray) == 3:
                PumpactivationRequested[2] = MsgInArray[1]
                DataContent[3] = MsgInArray[2]                  # ui8_SoilMoistureValue_03
                DataContent[0] = PumpActivationIsRequested(DataContent)
            else:
                AckMsg = [0xC0, 0x0, SOIL_MOISTURE_MSG_ID, 0x1]
            #if(IncomingMsgIsChanged(old_ClientMoisSensor3, MsgInArray) == True):
                #old_ClientMoisSensor3 = MsgInArray
            with open(file_path, "a") as fichier :
                Logs = TimeStamp() +" ACK send to client "+str(addr[0])+"\t Pump request= "+ str(DataContent[0]) +", Soil Moisture 02 value= "+str(DataContent[2]) + "\n"
                fichier.write(Logs)
                print(Logs)
                fichier.close()
            SendMsgToClient(conn, AckMsg)


        elif(CLIENT_PUMP == addr[0]):
            #Get if the pump activation was requested
            AckMsg = [0xC0, 0x0, PUMP_STATUS_MSG_ID, 0x0]
            if len(MsgInArray) == 3:
                DataContent[4] = MsgInArray[2]                  # ui8_WaterLevel
                DataContent[5] = MsgInArray[1]                  # ui8_PumpStatus
            else:
                AckMsg = [0xC0, 0x0, PUMP_STATUS_MSG_ID, 0x1]
            # check if the status switch from On --> Off
            #if ui8_OldPumpStatus == 1 and DataContent[5] == 0:
                #DataContent[0] = 0
                #with open(file_path, "a") as fichier :
                    #Logs = TimeStamp() +"["+str(addr[0])+"] Pump switch from ON to OFF\n"
                    #fichier.write(Logs)
                    #print(Logs)
                   #fichier.close()
            #ui8_OldPumpStatus = DataContent[5] 

            AckMsg.append(0xc5)
            for i in range(len(DataContent)):
                AckMsg.append(DataContent[i])
            #if(IncomingMsgIsChanged(old_ClientPump, MsgInArray) == True):
                #old_ClientPump = MsgInArray
            with open(file_path, "a") as fichier :
                Logs = TimeStamp() +" ACK send to client "+str(addr[0])+"\t Tank level= "+ str(DataContent[4]) +", Pump State= "+str(DataContent[5]) + "\n"
                fichier.write(Logs)
                print(Logs)
                fichier.close()
            SendMsgToClient(conn, AckMsg)

            with open(file_path, "a") as fichier :
                Logs = TimeStamp() +"["+str(addr[0])+"] Message sends to Pump client: "
                for i in range(len(AckMsg)):
                    Logs += str(hex(int(AckMsg[i]))[2:]).upper() + " "
                Logs += "\n"
                fichier.write(Logs)
                print(Logs)
                fichier.close()

        elif(CLIENT_DHT11 == addr[0]):
            AckMsg = [0xC0, 0x0, DHT11_DATA_MSG_ID, 0x0]
            if len(MsgInArray) == 9:
                DataContent[7] = MsgInArray[1]          # copy temp
                DataContent[8] = MsgInArray[2]
                DataContent[12] = MsgInArray[3]
                DataContent[13] = MsgInArray[4]
                DataContent[14] = MsgInArray[5]
                DataContent[9] = MsgInArray[6]
                DataContent[10] = MsgInArray[7]
                DataContent[11] = MsgInArray[8]
                with open(file_path, "a") as fichier :
                    Logs = TimeStamp() +" ACK send to client "+str(addr[0])+"\t Temp= "+ str(DataContent[8]) +", Hum= "+str(DataContent[7]) + "\n"
                    fichier.write(Logs)
                    print(Logs)
                    fichier.close()
                
            else:
                AckMsg = [0xC0, 0x0, DHT11_DATA_MSG_ID, 0x1]
            #if(IncomingMsgIsChanged(old_DHT11, MsgInArray) == True):
               # old_DHT11 = MsgInArray
            SendMsgToClient(conn, AckMsg)
            
        elif(CLIENT_ANEMOMETER == addr[0]):
            AckMsg = [0xC0, 0x0, WINDSPEED_MSG_ID, 0x0]
            if len(MsgInArray) == 2:
                DataContent[6] = MsgInArray[1]
                #if(IncomingMsgIsChanged(old_WindSpeed, MsgInArray) == True):
                    #old_WindSpeed = MsgInArray
            else:
                AckMsg = [0xC0, 0x0, WINDSPEED_MSG_ID, 0x1]
            with open(file_path, "a") as fichier :
                Logs = TimeStamp() +" ACK send to client "+str(addr[0])+"\t Wind Speed= "+ str(DataContent[6])+"mps\n"
                fichier.write(Logs)
                print(Logs)
                fichier.close()
            SendMsgToClient(conn, AckMsg)

        elif(CLIENT_LCD == addr[0]):
            AckMsg = [0xC0, 0, LCD_MSG_ID, 0]
            AckMsg.append(0xC5)
            for i in range(len(DataContent)):
                AckMsg.append(DataContent[i])
            with open(file_path, "a") as fichier :
                Logs = TimeStamp() +" ACK send to client "+str(addr[0])+"\n"
                fichier.write(Logs)
                print(Logs)
                fichier.close()
            SendMsgToClient(conn, AckMsg)


        if s_IncomingMsg == DISCONNECT:
            connected = False
        
        if(sys.argv[1] != "-without" and sys.argv[2] !="mariaDB"):
            #Insert clevertree value into SQL Database
            sql = "INSERT module (Data_PumpStatus, Data_TankLevel, Data_SoilMosture_01, \
                 Data_SoilMosture_02, Data_SoilMosture_03, Data_Temperature, Data_Humidity,\
                 Data_WindSpeed, Data_InfraRed, Data_Ultraviolet, Data_Visible, Data_CarbonMonoxyd, Data_LPG, Data_Smoke)\
                      VALUES (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)"
            val = (int(DataContent[5]), int(DataContent[4]), int(DataContent[1]), int(DataContent[2]), int(DataContent[3]), float(DataContent[7]), float(DataContent[8]),
                    float(DataContent[6]), int(DataContent[9]), float(DataContent[10]), int(DataContent[11]), float(DataContent[12]), float(DataContent[13]), float(DataContent[14]))
            StartMariaDBServer()[0].execute(sql, val)

            # transaction check-in
            StartMariaDBServer()[1].commit()

#--------------------------------------------------------------------------
# @author: Jonathan Guiyoba
# @creation date: 09-04-2023
#--------------------------------------------------------------------------
# @function description: 
#--------------------------------------------------------------------------
# @parameter: conn : 
# @return value: --
#--------------------------------------------------------------------------       
def start(listIPAdressClient : list, LocalDataContent : list, PumpactivationRequested : list):


    #Delete files if they are older than 2 weeks
    if(sys.argv[1] != "-without" and sys.argv[2] !="mariaDB"):
        for i in os.listdir("/home/pi/shp/Raspberry/LogFiles") :
            path_name = "/home/pi/shp/Raspberry/LogFiles" + "/" + i
            if (True == b_IsOlderThanTwoWeeks(path_name)):
                os.remove(path_name)
    server.listen()
    try:
        CurrentDateAndTime = GetCurrentDateAndTime(True)
        file_path = "/home/pi/shp/Raspberry/LogFiles/serverLog_"+ CurrentDateAndTime +".txt"
        while True:
            conn, addr = server.accept()
            listIPAdressClient.append(conn)
            thread = threading.Thread(target=handle_client, args=(conn, addr, LocalDataContent, PumpactivationRequested,  file_path))
            thread.start()
            if(sys.argv[1] != "-without" and sys.argv[2] !="mariaDB"):
                with open(file_path, "a") as fichier :
                    Logs = TimeStamp() +":" + f"[ACTIVE CONNECTIONS] {threading.active_count() -1}\t ipaddress = " + str(addr[0]) + "\n"
                    fichier.write(Logs)
                    print(Logs)
                fichier.close()
    except KeyboardInterrupt as e:
        print(f"The server was closed, because of keyboard interrupt: {e}")
        server.close()
        pass
    server.close()

#--------------------------------------------------------------------------
# @author: Gerald Emvoutou
# @creation date: 09-04-2023
#--------------------------------------------------------------------------
# @function description: Get if the pump activation was requested
#--------------------------------------------------------------------------
# @parameter: soilMois01Request: bool, soilMois02Request: bool, soilMois03Request: bool
# @return value:    True -- Activation was requested
#                   False -- Activation wasn't requested
#--------------------------------------------------------------------------
def PumpActivationIsRequested(DataContent: list) :
    b_PumpActivatioRequest = '0'
    ui8_ActivationRequested = 0
    if((int(DataContent[1]) != 0) and (int(DataContent[1]) < 30)):
        ui8_ActivationRequested = ui8_ActivationRequested + 1
    if((int(DataContent[2]) != 0) and (int(DataContent[2]) < 30)):
        ui8_ActivationRequested = ui8_ActivationRequested + 1
    if((int(DataContent[3]) != 0) and (int(DataContent[3]) < 30)):
        ui8_ActivationRequested = ui8_ActivationRequested + 1
    if ui8_ActivationRequested != 0 :
        b_PumpActivatioRequest = '1'
    print("b_PumpActivatioRequest ")
    print(b_PumpActivatioRequest)
    return b_PumpActivatioRequest

#--------------------------------------------------------------------------
# @author: Gerald Emvoutou
# @creation date: 09-04-2023
#--------------------------------------------------------------------------
# @function description: main function
#--------------------------------------------------------------------------
# @parameter: --
# @return value:--
#--------------------------------------------------------------------------
def main():
    # Initialize array to store client IP addresses
    localDataContent = []
    InitArrayWithElements(localDataContent, 15, "Integer")
    PumpactivationRequested =  []
    InitArrayWithElements(PumpactivationRequested, 3, "boolean")
    listIPAdressClient = []
    start(listIPAdressClient, localDataContent, PumpactivationRequested)
#--------------------------------------------------------------------------
#--------------------------------------------------------------------------

if __name__ == "__main__":
    print("Start...")
    main()
