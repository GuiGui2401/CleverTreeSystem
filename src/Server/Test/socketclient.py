import socket
import time


HOST = socket.gethostbyname(socket.gethostname())  # The server's hostname or IP address
PORT = 50011  # The port used by the server

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect(("192.168.1.200", PORT))
    while True:
        s.sendall(b"334.1.237")
        data = s.recv(1024)

        print(f"Received {data!r}")
        time.sleep(1)