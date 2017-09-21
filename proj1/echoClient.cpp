#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#define commandString "echo"
int main(int argc,char **argv)
{
	//	command check
    if(argc != 4) {
        printf("Length of command is wrong, expected length is 4 !\n");
        exit(1);
    } else if(strcmp(commandString, argv[1]) != 0) {
        printf("Wrong command, expected \"echo\" !\n");
        exit(1);
    }
	
    int sockfd, n;
    char sendline[100];
    char recvline[100];
    struct sockaddr_in servaddr;
 
	//	socket() with TCP/IPv4
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
 
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[3]));
 	
    inet_pton(AF_INET, argv[2], &(servaddr.sin_addr));
    printf("\nReady for sending...");
 
	// connect()
    connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    printf("\nEnter the message to send\n");
    
	while(1) {
        bzero(sendline, 100);
        bzero(recvline, 100);
        printf("\nClient: ");
        fgets(sendline, 100, stdin); /*stdin = 0 , for standard input */
        
        write(sockfd, sendline, strlen(sendline)+1);
        read(sockfd, recvline, 100);
        printf("Serverecho:%s", recvline);
        printf("\n");
    }
}
