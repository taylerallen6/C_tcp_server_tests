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
    int client_sock;
    struct sockaddr_in client_addr;
    socklen_t addr_size;
} Client_connection;

typedef struct
{
    int server_sock;
    struct sockaddr_in server_addr;
    Client_connection* client_connection;
} Database_server;


Client_connection* Client_connection_create()
{
    Client_connection* new_client_connection = malloc(sizeof(Client_connection));
    return new_client_connection;
}

Database_server* database_server_create()
{
    Database_server* new_db_server = malloc(sizeof(Database_server));
    new_db_server->client_connection = Client_connection_create();

    return new_db_server;
}

void database_server_free(Database_server* db_server)
{
    free(db_server->client_connection);
    free(db_server);
}

bool database_server_start(Database_server* db_server, int port)
{   
    // socket create and verification
    db_server->server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (db_server->server_sock == -1)
    {
        perror("[-]Socket error");
        exit(1);
    }
    else
        printf("[+]TCP server socket created.\n");

    memset(&(db_server->server_addr), '\0', sizeof(db_server->server_addr));

    // assign IP, PORT 
    db_server->server_addr.sin_family = AF_INET;
    db_server->server_addr.sin_port = port;
    db_server->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);     // use for any ip address
    // db_server->server_addr.sin_addr.s_addr = inet_addr(ip);        // use for only localhost

    // Binding newly created socket to given IP and verification 
    if ((bind(db_server->server_sock, (struct sockaddr*)&(db_server->server_addr), sizeof(db_server->server_addr))) != 0)
    {
        perror("[-]Bind error");
        exit(1);
    } 
    else
        printf("[+]Bind to the port number: %d\n", port);

    // Now server is ready to listen and verification 
    if ((listen(db_server->server_sock, 5)) != 0)
    { 
        printf("[-]Listen failed...\n"); 
        return false; 
    } 
    else
        printf("Server listening..\n"); 

    db_server->client_connection->addr_size = sizeof(db_server->client_connection->client_addr);

    return true;
}


bool database_server_close(Database_server* db_server)
{
    close(db_server->server_sock);
    printf("[+]Server closed.\n");

    return true;
}

bool database_server_client_connection_close(Client_connection* client_connection)
{
    close(client_connection->client_sock);
    printf("[+]Client closed.\n\n");

    return true;
}

bool database_server_process_message(Database_server* db_server)
{
    
}


int main()
{
    char *ip = "127.0.0.1";
    int port = 5566;
    char buffer[MAX_BUFFER_SIZE];


    Database_server* db_server = database_server_create();

    // socket create and verification
    db_server->server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (db_server->server_sock == -1)
    {
        perror("[-]Socket error");
        exit(1);
    }
    else
        printf("[+]TCP server socket created.\n");

    memset(&(db_server->server_addr), '\0', sizeof(db_server->server_addr));

    // assign IP, PORT 
    db_server->server_addr.sin_family = AF_INET;
    db_server->server_addr.sin_port = port;
    db_server->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);     // use for any ip address
    // db_server->server_addr.sin_addr.s_addr = inet_addr(ip);        // use for only localhost

    // Binding newly created socket to given IP and verification 
    if ((bind(db_server->server_sock, (struct sockaddr*)&(db_server->server_addr), sizeof(db_server->server_addr))) != 0)
    {
        perror("[-]Bind error");
        exit(1);
    } 
    else
        printf("[+]Bind to the port number: %d\n", port);

    // Now server is ready to listen and verification 
    if ((listen(db_server->server_sock, 5)) != 0)
    { 
        printf("[-]Listen failed...\n"); 
        return false; 
    } 
    else
        printf("Server listening..\n"); 

    db_server->client_connection->addr_size = sizeof(db_server->client_connection->client_addr);


    // continouusly wait for client to connect
    for (;;)
    {
        // Accept the data packet from client and verification 
        db_server->client_connection->client_sock = accept(db_server->server_sock, (struct sockaddr*)&(db_server->client_connection->client_addr), &(db_server->client_connection->addr_size));
        if (db_server->client_connection->client_sock < 0) { 
            perror("[-]Server accept failed...");
            exit(1);
        } 
        else
            printf("[+]Client connected.\n");

        // continuously wait for client message
        for(;;)
        {
            bzero(buffer, MAX_BUFFER_SIZE);
            // recv(client_sock, buffer, sizeof(buffer), 0);
            int response_code = read(db_server->client_connection->client_sock, buffer, sizeof(buffer));
            if (response_code == 0)
            {
                printf("[-]Client no longer connected.\n");
                break;
            }
            
            printf("Client: %s\n", buffer);

            bzero(buffer, MAX_BUFFER_SIZE);
            strcpy(buffer, "HI, THIS IS SERVER. HAVE A NICE DAY!!!");
            printf("Server: %s\n", buffer);
            // send(client_sock, buffer, strlen(buffer), 0);
            write(db_server->client_connection->client_sock, buffer, sizeof(buffer));

        }

        database_server_client_connection_close(db_server->client_connection);

    }

    database_server_close(db_server);

    return 0;
}