import socket
import pymysql
import time

HOST = "0.0.0.0"
PORT = 5000

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server.bind((HOST, PORT))
server.listen()

print("Python Server Running on Port 5000...")

while True:
    try:
        conn, addr = server.accept()
        
        db = pymysql.connect(
            host="mysql_database",
            user="root",
            password="root123",
            database="assignment_db",
            port=3306
        )

        cursor = db.cursor()
        
        cursor.execute("UPDATE score_table SET points = points + 1, datetime_stamp = NOW() WHERE user = 'pyserver2'")
        db.commit()

        cursor.execute("SELECT points, datetime_stamp FROM score_table WHERE user = 'pyserver2'")
        result = cursor.fetchone()

        if result:
            message = f"Points: {result[0]} | Updated: {result[1]}"
        else:
            message = "User pyserver2 not found"

        conn.send(message.encode())
        db.close()
        conn.close()

    except Exception as e:
        print(f"Error encountered: {e}")
        time.sleep(2)
