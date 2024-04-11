#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()


#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

typedef struct
{
  int sockfd;
  struct sockaddr_in servaddr;
} Database_connection;

Database_connection* database_connection_create()
{
    Database_connection* new_db_connection = malloc(sizeof(Database_connection));
    return new_db_connection;
}

void database_connection_free(Database_connection* db_connection)
{
    free(db_connection);
}


bool database_connection_connect(Database_connection* db_connection, char* ip_address, int port)
{
    // socket create and verification
    db_connection->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (db_connection->sockfd == -1) {
        printf("socket creation failed...\n");
        return false;
    }
    else
        printf("Socket successfully created..\n");

    bzero(&(db_connection->servaddr), sizeof(db_connection->servaddr));
 
    // assign IP, PORT
    db_connection->servaddr.sin_family = AF_INET;
    db_connection->servaddr.sin_addr.s_addr = inet_addr(ip_address);
    db_connection->servaddr.sin_port = htons(PORT);
 
    // connect the client socket to server socket
    if (connect(db_connection->sockfd, (SA*)&(db_connection->servaddr), sizeof(db_connection->servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        return false;
    }
    else
        printf("connected to the server..\n");

    return true;
}

bool database_connection_close(Database_connection* db_connection)
{
    close(db_connection->sockfd);
    printf("connection closed\n");

    return true;
}

void func_wrapper(Database_connection* db_connection)
{
    func(db_connection->sockfd);
}

char* database_connection_send_command(Database_connection* db_connection, char* query_string)
{
    char buff[MAX];
    // char* buff = malloc(sizeof(char) * MAX);
    
    bzero(buff, sizeof(buff));
    strcpy(buff, query_string);

    write(db_connection->sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));

    char* response = malloc(sizeof(char) * MAX);
    bzero(response, sizeof(response));

    read(db_connection->sockfd, response, sizeof(response));

    return response;
}
 
int main()
{

    Database_connection* db_connection = database_connection_create();
    database_connection_connect(db_connection, "127.0.0.1", 8080);
 
    // function for chat
    // func_wrapper(db_connection);

    char* response = database_connection_send_command(db_connection, "howdy");
    printf("response: %s\n", response);
    free(response);
 
    // close the socket
    database_connection_close(db_connection);
    database_connection_free(db_connection);
}