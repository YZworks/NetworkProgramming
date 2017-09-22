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
#include "helper.cpp"

#define commandString "echo"
#define MAX_TEXT_LEN 100	//	the maximum length of text line
#define BUFF_SIZE 100	//	the size of buffer

int main(int argc,char **argv) {

	//	command check
    if(argc != 4) {
        printf("Length of command is wrong, expected length is 4 !\n");
        exit(1);
    } else if(strcmp(commandString, argv[1]) != 0) {
        printf("Wrong command, expected \"echo\" !\n");
        exit(1);
    }
	
    int sockfd, n;
    char send_buff[BUFF_SIZE];
    char recv_buff[BUFF_SIZE];
    struct sockaddr_in servaddr;
 
	//	socket() with TCP/IPv4
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
 
	//	servaddr config
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[3]));
    inet_pton(AF_INET, argv[2], &(servaddr.sin_addr));
 
	// connect()
    connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    
	while(fgets(send_buff, BUFF_SIZE, stdin)) {
					
		//	writen()
		//int sendByte = writen(sockfd, send_buff, strlen(send_buff) + 1);
		int sendByte = write(sockfd, send_buff, strlen(send_buff) + 1);

		if(sendByte == 0) {
			break;
		}
		//	readline()
		read(sockfd, recv_buff, BUFF_SIZE + 1);
		//readline(sockfd, recv_buff, BUFF_SIZE + 1);
		
		//	print echo
		printf("Echoing back from Server : \n");
		printf("******************************************************\n");
        printf("%s", recv_buff);
        printf("\n******************************************************\n");
        printf("\n");

        bzero(recv_buff, BUFF_SIZE + 1);
        bzero(send_buff, BUFF_SIZE + 1);     
	}
		
	printf("Client terminated!\n");
	close(sockfd);
	return 1;
}
