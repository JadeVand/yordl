#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 1222
#define SA struct sockaddr


int main()
{
 int sockfd, connfd;
 struct sockaddr_in servaddr, cli;
char buff[MAX]="HelloWorld";
int newval_val = 194000000;
char buf[256];
char str1[]= "edit##freq##";
char str2[10];
 // socket create and verification
 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 if (sockfd == -1) {
     printf("socket creation failed...\n");
     exit(0);
 }
 else
     printf("Socket successfully created..\n");
 bzero(&servaddr, sizeof(servaddr));

 // assign IP, PORT
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr = inet_addr("173.11.11.!3");
 servaddr.sin_port = htons(PORT);

 // connect the client socket to server socket
 if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
     printf("connection with the server failed...\n");
     exit(0);
 }
 else
     printf("connected to the server..\n");



     snprintf(str2, 10, "%d", newval_val);

     snprintf(buf, sizeof(buf), "%s%s \n", str1,str2);
     printf("combined the string..\n");
     printf("msg to send : %s", buf);
 


             write(sockfd, buf, sizeof(buf));

     
             bzero(buff, sizeof(buff));

             read(sockfd, buff, sizeof(buff));

             
             printf("From Server : %s", buff);
     if ((strncmp(buff, "exit", 4)) == 0) {
         printf("Client Exit...\n");
     
     }
 
 close(sockfd);
}
