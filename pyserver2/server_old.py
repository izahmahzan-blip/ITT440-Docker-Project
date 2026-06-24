nano pyserver2/test_db.py


nano pyserver2/test_db.pyimport socket
import pymysql

HOST = "0.0.0.0"
PORT = 5000

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((HOST, PORT))
server.listen()

print("Python Server Running on Port 5000")

while True:
    conn, addr = server.accept()

    try:
        db = pymysql.connect(
        host="db",
        user="root",
        password="root123",
        database="assignment_db",
        port=3306

        )

        cursor = db.cursor()

        cursor.execute("""
            SELECT points, datetime_stamp
            FROM score_table
            WHERE user='python_user'
        """)

        result = cursor.fetchone()

        message = f"Points: {result[0]} | Updated: {result[1]}"

        conn.send(message.encode())

        db.close()

    except Exception as e:
        conn.send(str(e).encode())

    conn.close()
