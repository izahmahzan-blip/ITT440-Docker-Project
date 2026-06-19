import pymysql

try:
    db = pymysql.connect(
        host="127.0.0.1",
        user="root",
        password="rootpassword",
        database="assignment_db",
        port=3306
    )

    print("Connected to MySQL!")

    cursor = db.cursor()

    cursor.execute("SELECT * FROM score_table")

    rows = cursor.fetchall()

    for row in rows:
        print(row)

    db.close()

except Exception as e:
    print("Error:", e)
