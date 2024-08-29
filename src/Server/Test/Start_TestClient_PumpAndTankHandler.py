import socket
import sys

HOST = socket.gethostbyname(socket.gethostname())  # The server's hostname or IP address
PORT = 50011  # The port used by the server

if (HOST == "192.168.1.210"):
    print("Start client " + str(HOST) + ":" + str(PORT))
    if( 2 <= len(sys.argv)):
        str_DataToSend : str = "129."+ str(sys.argv[0]) + "." + str(sys.argv[1])

        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client:
            client.connect(("192.168.1.200", PORT))
            client.sendall(str_DataToSend)
            print("message sent: " + str_DataToSend)
            str_DataReceived = client.recv(1024)
            print(f"ACK_Message: {str_DataReceived!r}")
    else:
        print("Arguments are missing. run command \"python Start_TestClient_PumpAndTankHandler.py 0 100\" for example 0 = pumpstatus; 100 = Tank level")

else:
    print("Set your computer's IP address as a static address: 192.168.1.210")