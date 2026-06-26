import socket
import time

HOST = "pyserver2"
PORT = 5000

print("[CLIENT PYTHON] Client started, waiting to connect...")

while True:
    try:
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect((HOST, PORT))
        
        message = client.recv(1024).decode()
        print(f"[CLIENT PYTHON] Message received from Server: {message}")
        
        client.close()
    except Exception as e:
        print(f"[CLIENT PYTHON] Connection failed: {e}. Retrying in 5 seconds...")
        time.sleep(5)
        continue
        
    time.sleep(30)
