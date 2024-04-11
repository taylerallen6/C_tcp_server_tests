#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>


#define MAX_BUFFER_SIZE 1024

typedef struct
{
    int sock;
    struct sockaddr_in addr;
} Database_client_connection;

Database_client_connection* database_client_connection_create()
{
    Database_client_connection* new_db_connection = malloc(sizeof(Database_client_connection));
    return new_db_connection;
}

void database_client_connection_free(Database_client_connection* db_connection)
{
    free(db_connection);
}

bool database_client_connection_connect(Database_client_connection* db_connection, char* ip_address, int port)
{   
    // socket create and verification
    db_connection->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (db_connection->sock == -1)
    {
        perror("[-]Socket error");
        return false;
    }
    else
        printf("[+]TCP server socket created.\n");

    memset(&(db_connection->addr), '\0', sizeof(db_connection->addr));

    // assign IP, PORT
    db_connection->addr.sin_family = AF_INET;
    db_connection->addr.sin_port = port;
    db_connection->addr.sin_addr.s_addr = inet_addr(ip_address);

    // connect the client socket to server socket
    if (connect(db_connection->sock, (struct sockaddr*)&(db_connection->addr), sizeof(db_connection->addr)) != 0)
    {
        perror("[-]Connection with the server failed");
        return false;
    }
    else
        printf("[+]Connected to the server\n");

    return true;
}

bool database_client_connection_close(Database_client_connection* db_connection)
{
    close(db_connection->sock);
    printf("[+]Disconnected from the server.\n");

    return true;
}

char* database_client_connection_send_command(Database_client_connection* db_connection, char* query_string)
{
    char buffer[MAX_BUFFER_SIZE];

    bzero(buffer, MAX_BUFFER_SIZE);
    strcpy(buffer, query_string);
    write(db_connection->sock, buffer, sizeof(buffer));

    bzero(buffer, MAX_BUFFER_SIZE);
    read(db_connection->sock, buffer, sizeof(buffer));

    char* response = malloc(sizeof(char) * MAX_BUFFER_SIZE);
    strcpy(response, buffer);
    
    return response;
}


int main()
{
    char *ip = "127.0.0.1";
    // char* ip = "172.16.151.148";
    int port = 5566;


    Database_client_connection* db_connection = database_client_connection_create();
    database_client_connection_connect(db_connection, ip, port);
    char* response;

    response = database_client_connection_send_command(db_connection, "howdy");
    printf("response: %s\n", response);

    sleep(1);

    response = database_client_connection_send_command(db_connection, "howdy");
    printf("response: %s\n", response);

 
    // close the socket
    database_client_connection_close(db_connection);
    database_client_connection_free(db_connection);
    free(response);

    return 0;

}