#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <mysql/mysql.h>

#define PORT 8080
#define DB_HOST "mysql_database"
#define DB_USER "root"
#define DB_PASS "root123"
#define DB_NAME "assignment_db"

void update_database_points() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) return;

    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0) == NULL) {
        mysql_close(conn);
        return;
    }

    char *query = "UPDATE score_table SET points = points + 1, datetime_stamp = NOW() WHERE user = 'cserver1'";
    if (mysql_query(conn, query)) {
        fprintf(stderr, "UPDATE failed: %s\n", mysql_error(conn));
    } else {
        printf("[DB UPDATE] Points for cserver1 successfully updated.\n");
    }
    mysql_close(conn);
}

void get_latest_points(char *response_buffer) {
    MYSQL *conn = mysql_init(NULL);
    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0) == NULL) {
        strcpy(response_buffer, "Error connecting to Database");
        return;
    }

    char *query = "SELECT points, datetime_stamp FROM score_table WHERE user = 'cserver1'";
    if (mysql_query(conn, query) == 0) {
        MYSQL_RES *result = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            sprintf(response_buffer, "Points: %s | Updated: %s", row[0], row[1]);
        } else {
            strcpy(response_buffer, "User cserver1 not found");
        }
        mysql_free_result(result);
    }
    mysql_close(conn);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("C Server running on port %d...\n", PORT);

    pid_t pid = fork();
    if (pid == 0) {
        while (1) {
            sleep(30);
            update_database_points();
        }
    } else {
        while (1) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) continue;
            char response[256];
            get_latest_points(response);
            send(new_socket, response, strlen(response), 0);
            printf("[SERVER] Data sent to client: %s\n", response);
            close(new_socket);
        }
    }
    close(server_fd);
    return 0;
}