#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>

#define SIZE 4
#define PORT 6868
#define SA struct sockaddr

char board[10][10];
int counter = 0;
int score = 250;
char keep[] = "Continue";

void battleShip(int confd);
void printBoard();
int rowCoordinate();
int columnCoordinate();

int main()
{
    int sockfd, confd, len;
    struct sockaddr_in servaddr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
   
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket binding is failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    if ((listen(sockfd, 5)) != 0) {
        printf("Listening the server is failed...\n");
        exit(0);
    }
    else
        printf("Server is start listening..\n");
    len = sizeof(cli);
   
    // Accept the data packet from client and verification
    confd = accept(sockfd, (SA*)&cli, &len);
    if (confd < 0) {
        printf("Server could not accept the player...\n");
        exit(0);
    }
    else
        printf("Server accepted the player...\n");
    battleShip(confd);
    close(sockfd);
}
int randomCoordinate()
{
    srand(time(0));
    int randomNumber;
   
    randomNumber = rand() % 5;
    return randomNumber;
}
void printBoard()
{
    for(int i = 0 ; i < 10 ; i++)
    {
        for(int j = 0 ; j < 10 ; j++)
        {
            printf("%c ",board[i][j]);
        }
        printf("\n");
    }

}
void battleShip(int confd)
{
    char target[SIZE];
    int shipOne_row,shipOne_column,shipTwo_row,shipTwo_column,shipThree_row,shipThree_column;
    int targetRow, targetColumn, n;
    
    for(int i = 0 ; i < 10 ; i++ )
    {
        for(int j = 0 ; j < 10 ; j++)
        {
            board[i][j] = '0';
        }
    }
    for(int i = 0 ; i < 35 ; i++)
        printf("-");
    printf("\n");
    printf("Welcome the battleship game.\n");
    for(int i = 0 ; i < 35 ; i++)
        printf("-");
    printf("\n");
    printf("The score:%d\n",score);
    printBoard();
    printf("\n");
    
    srand(time(NULL));
    shipOne_row = rand() % 10;
    shipTwo_row = rand() % 10;
    shipThree_row = rand() % 10;
    shipOne_column = rand() % 10;
    shipTwo_column = rand() % 10;
    shipThree_column = rand() % 10;
   while(1)
    {
        if(shipOne_row == shipTwo_row && shipOne_column == shipTwo_column)
        {
            shipTwo_row = rand() % 10;
            shipTwo_column = rand() % 10;
            continue;
        }
        else if(shipOne_row == shipThree_row && shipOne_column == shipThree_column)
        {
            shipThree_row = rand() % 10;
            shipThree_column = rand() % 10;
            continue;
        }
        else if(shipTwo_row == shipThree_row && shipTwo_column == shipThree_column)
        {
            shipThree_row = rand() % 10;
            shipThree_column = rand() % 10;
            continue;
        }
        else 
        {
            for(int i = 0 ; i < 35 ; i++)
              printf("-");
            printf("\n");
            bzero(target, SIZE);
            read(confd, target, sizeof(target));
            targetRow = target[0] - '0';
            targetColumn = target[2] - '0';
            if((targetRow == shipOne_row && targetColumn == shipOne_column) || (targetRow == shipTwo_row && targetColumn == shipTwo_column) || (targetRow == shipThree_row && targetColumn == shipThree_column))
            {
                if(board[targetRow - 1][targetColumn - 1] == '/')
                {
                    for(int i = 0 ; i < 35 ; i++)
                       printf("-");
                    printf("\n");
                    printf("You have already selected\n");
                    printBoard();
                    printf("Score = %d",score);
                }
                else
                {
                    for(int i = 0 ; i < 35 ; i++)
                      printf("-");
                    printf("\n");
                    printf("Congratulations! you have sinked the ship!");
                    board[targetRow- 1][targetColumn -1] = '/';
                    printf("Score = %d",score);
                    for(int i = 0 ; i < 35 ; i++)
                       printf("-");
                    printf("\n");
                    printBoard();
                    counter += 1;
                }
            }
            else
            {
                if((targetColumn < 0 || targetRow < 0) || (targetColumn > 10 || targetRow > 10))
                {
                    for(int i = 0 ; i < 35 ; i++)
                      printf("-");
                    printf("\n");
                    printf("You have the crossed the limit of the field\n");
                }
                else if (board[targetRow - 1][targetColumn - 1] == 'X')
                {
                    for(int i = 0 ; i < 35 ; i++)
                      printf("-");
                    printf("\n");
                    printf("You have already selected that coordinate\n");
                    for(int i = 0 ; i < 35 ; i++)
                      printf("-");
                    printf("\n");
                    printBoard();
                }
                else
                {
                    for(int i = 0 ; i < 35 ; i++)
                      printf("-");
                    printf("\n");
                    printf("You have missed\n");
                    for(int i = 0 ; i < 35 ; i++)
                      printf("-");
                    printf("\n");
                    board[targetRow - 1][targetColumn - 1] == 'X';
                    score -= 10;
                    printf("Score:%d\n",score);
                    for(int i = 0 ; i < 35 ; i++)
                      printf("-");
                    printf("\n");
                    printBoard();
                }
                if (counter == 3)
                {
                    for(int i = 0 ; i < 35 ; i++)
                      printf("-");
                    printf("\n");
                    printf("All ships have been sinked\n");
                    printf("Congratulations! You won the game\n");
                }
                write(confd, target, sizeof(target));
                /*bzero(target, SIZE);
                n = 0;
                while((target[n++] = getchar()) != '\n')
                ;
                write(confd, target, sizeof(target));
                if (strncmp("exit", target, 4) == 0) {
                    printf("The game has been ended...\n");
                    break;
                }*/
            }
        }
    }
}