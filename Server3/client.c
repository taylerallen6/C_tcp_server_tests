// #include <sys/socket.h>
// #include <sys/types.h>
// #include <signal.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <strings.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <stdarg.h>
// #include <errno.h>
// #include <fcntl.h>
// #include <sys/time.h>
// #include <sys/ioctl.h>
// #include <netdb.h>

// #define SERVER_PORT 8080

// #define MAXLINE 4096
// #define SA struct sockaddr


// void err_n_die(const char* fmt, ...)
// {
//     int errno_save;
//     va_list ap;

//     errno_save = errno;

//     /* print out the fmt+arg to standard out */
//     va_start(ap, fmt);
//     vfprintf(stdout, fmt, ap);
//     fprintf(stdout, "\n");
//     fflush(stdout);

//     /* print out error message if errno was set */
//     if (errno_save != 0)
//     {
//         fprintf(stdout, "(errno = %d) : %s\n", errno_save, strerror(errno_save));
//         fprintf(stdout, "\n");
//         fflush(stdout);
//     }
//     va_end(ap);

//     /* this is the .. and die part. terminate with an error */
//     exit(1);
// }

// int main(int argc, char** argv)
// {
//     int sockfd;
//     int n;
//     int sendbytes;
//     struct sockaddr_in servaddr;
//     char sendline[MAXLINE];
//     char recvline[MAXLINE];

//     if (argc != 2)
//         err_n_die("usage: %s <server address>", argv[0]);

//     if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) > 0)
//         err_n_die("Error while creating the socket!");

//     bzero(&servaddr, sizeof(servaddr));
//     servaddr.sin_family = AF_INET;
//     servaddr.sin_port = htons(SERVER_PORT);

//     if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
//         err_n_die("inet_pton error for %s ", argv[1]);

//     if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) < 0)
//         err_n_die("connect failed!");

//     /* wconnected, prepare the message. */
//     sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");
//     sendbytes = strlen(sendline);

//     /* send the request -- making sure you send it all */
//     /* this code is a bit fragile, since it bails if only some of the bytes are sent. */
//     /* normally, you would want to retry, unless the return value was -1. */
//     if (write(sockfd, sendline, sendbytes) != sendbytes)
//         err_n_die("write error");

//     memset(recvline, 0, MAXLINE);
//     /* now read the server's response. */
//     while((n = read(sockfd, recvline, MAXLINE-1)) > 0)
//     {
//         printf("%s", recvline);
//         memset(recvline, 0, MAXLINE);
//     }
//     if (n < 0)
//         err_n_die("read error");
    
//     /* end successfully */
//     exit(0);
    
    
// }


#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdarg.h> /* for variadic function */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h> /* basic socket definitions */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

// standard HTTP port
#define SERVER_PORT 8989

#define MAXLINE 4096
#define SA struct sockaddr

void err_n_die(const char *fmt, ...);

int main(int argc, char **argv) {
  int sockfd, n;
  int sendbytes;
  struct sockaddr_in servaddr;
  char sendline[MAXLINE];
  char recvline[MAXLINE];

  if (argc != 2)
    err_n_die("usage: %s <server address>", argv[0]);

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_n_die("Error while creating the socket!");

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERVER_PORT);

  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    err_n_die("inet_pton error for %s ", argv[1]);

  if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
    err_n_die("connect failed!");

  // We're connected. Prepare the message.
  sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");
  sendbytes = strlen(sendline);

  // Send the message -- making sure you send it all
  // This code is a bit fragile, since it bails if only some of the bytes are
  // sent. normally, you would want to retry, unless the return value was -1.
  if (write(sockfd, sendline, sendbytes) != sendbytes)
    err_n_die("write error");

  // Now read the server's response.
  while ((n = read(sockfd, recvline, MAXLINE - 1)) > 0) {
    recvline[n] = 0;
    printf("%s", recvline);
  }
  if (n < 0)
    err_n_die("read error");

  exit(0); // end successfully!
}

void err_n_die(const char *fmt, ...) {
  int errno_save;
  va_list ap;

  // all system calls can set errno, so we need to save it now
  errno_save = errno;

  // print out the fmt+args to standard out
  va_start(ap, fmt);
  vfprintf(stdout, fmt, ap);
  fprintf(stdout, "\n");
  fflush(stdout);

  // print out error message is errno was set.
  if (errno_save != 0) {
    fprintf(stdout, "(errno = %d) : %s\n", errno_save, strerror(errno_save));
    fprintf(stdout, "\n");
    fflush(stdout);
  }
  va_end(ap);

  // this is the ..and_die part. Terminate with an error.
  exit(1);
}