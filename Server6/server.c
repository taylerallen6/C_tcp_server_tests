#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>


#define SA struct sockaddr 

int main(){

    char *ip = "127.0.0.1";
    int port = 5566;

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0){
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    // server_addr.sin_addr.s_addr = htonl(INADDR_ANY);     // use for any ip address
    server_addr.sin_addr.s_addr = inet_addr(ip);        // use for only localhost



    n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (n < 0){
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to the port number: %d\n", port);

    listen(server_sock, 5);
    printf("Listening...\n");


    // // Binding newly created socket to given IP and verification 
    // if ((bind(server_sock, (SA*)&server_addr, sizeof(server_addr))) != 0)
    // {
    //     perror("[-]Bind error");
    //     exit(1);
    // } 
    // else
    //     printf("[+]Bind to the port number: %d\n", port);
   
    // // Now server is ready to listen and verification 
    // if ((listen(server_sock, 5)) != 0)
    // { 
    //     printf("[-]Listen failed...\n"); 
    //     exit(0); 
    // } 
    // else
    //     printf("Server listening..\n"); 



    addr_size = sizeof(client_addr);

    for (;;)
    {        
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        printf("[+]Client connected.\n");

        for(;;)
        {
            bzero(buffer, 1024);
            // recv(client_sock, buffer, sizeof(buffer), 0);
            int response_code = read(client_sock, buffer, sizeof(buffer));
            printf("response_code: %d\n", response_code);
            if (response_code == 0)
            {
                printf("BREAKING\n\n");
                break;
            }
            
            printf("Client: %s\n", buffer);

            bzero(buffer, 1024);
            strcpy(buffer, "HI, THIS IS SERVER. HAVE A NICE DAY!!!");
            printf("Server: %s\n", buffer);
            // send(client_sock, buffer, strlen(buffer), 0);
            write(client_sock, buffer, sizeof(buffer));

        }

        close(client_sock);
        printf("[+]Client disconnected.\n\n");

    }

    close(server_sock);

    return 0;
}