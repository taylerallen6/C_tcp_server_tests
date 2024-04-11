#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>


int main(){

    char *ip = "127.0.0.1";
    // char* ip = "172.16.151.148";
    int port = 5566;

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0){
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    // // socket create and verification
    // sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // if (sockfd == -1) {
    //     printf("socket creation failed...\n");
    //     exit(0);
    // }
    // else
    //     printf("Socket successfully created..\n");

    bzero(&addr, sizeof(addr));

    // memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);

    // connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    // printf("Connected to the server.\n");

    // connect the client socket to server socket
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }

    // // connect the client socket to server socket
    // if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
    //     != 0) {
    //     printf("connection with the server failed...\n");
    //     exit(0);
    // }

    bzero(buffer, 1024);
    strcpy(buffer, "HELLO, THIS IS CLIENT.");
    printf("Client: %s\n", buffer);
    // send(sock, buffer, strlen(buffer), 0);
    write(sock, buffer, sizeof(buffer));

    bzero(buffer, 1024);
    // recv(sock, buffer, sizeof(buffer), 0);
    read(sock, buffer, sizeof(buffer));
    
    printf("Server: %s\n", buffer);

    sleep(1);


    bzero(buffer, 1024);
    strcpy(buffer, "HELLO, THIS IS CLIENT.");
    printf("Client: %s\n", buffer);
    // send(sock, buffer, strlen(buffer), 0);
    write(sock, buffer, sizeof(buffer));

    bzero(buffer, 1024);
    // recv(sock, buffer, sizeof(buffer), 0);
    read(sock, buffer, sizeof(buffer));
    
    printf("Server: %s\n", buffer);

    close(sock);
    printf("Disconnected from the server.\n");

    return 0;

}