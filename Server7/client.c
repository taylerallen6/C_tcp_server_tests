#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>


int main()
{
    char *ip = "127.0.0.1";
    // char* ip = "172.16.151.148";
    int port = 5566;

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];

    // socket create and verification
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("[-]Socket error");
        exit(1);
    }
    else
        printf("[+]TCP server socket created.\n");

    memset(&addr, '\0', sizeof(addr));

    // assign IP, PORT
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);

    // connect the client socket to server socket
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
    {
        perror("[-]Connection with the server failed");
        exit(1);
    }
    else
        printf("[+]Connected to the server\n");



    bzero(buffer, 1024);
    strcpy(buffer, "HELLO, THIS IS CLIENT.");
    printf("Client: %s\n", buffer);
    write(sock, buffer, sizeof(buffer));

    bzero(buffer, 1024);
    read(sock, buffer, sizeof(buffer));
    
    printf("Server: %s\n", buffer);

    sleep(1);


    bzero(buffer, 1024);
    strcpy(buffer, "HELLO, THIS IS CLIENT.");
    printf("Client: %s\n", buffer);
    write(sock, buffer, sizeof(buffer));

    bzero(buffer, 1024);
    read(sock, buffer, sizeof(buffer));
    
    printf("Server: %s\n", buffer);

    close(sock);
    printf("Disconnected from the server.\n");

    return 0;

}