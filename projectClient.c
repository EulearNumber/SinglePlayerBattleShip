#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <arpa/inet.h> 

#define SIZE 4
#define PORT 6868
#define SA struct sockaddr
int flag = 1;
void Fire(int sockfd)
{
    char target[SIZE];
    int index = 0;
    for (;;) {

        printf("Enter the row and column with a space between them.\n");
        printf("If you want to exit the game please type exit.\n");
        while((target[index++] = getchar()) != '\n')
        ;
        write(sockfd, target, sizeof(target));
       
        read(sockfd, target, sizeof(target));
            
   
        if(target[1] != ' ')
        {
            printf("Error make sure that there is a space between the row and column.\n");
            continue;
            
        }
        else if(strncmp("exit", target, 4) == 0)
        {
            flag = 0;
            break;
        }
        memset(target,'0', sizeof(target));
        index = 0;

    }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
   
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
   
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    Fire(sockfd);
    close(sockfd);
}