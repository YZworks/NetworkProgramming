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

#define commandString "echos"
#define MAX_TEXT_LEN 100	//	the maximum length of text 
#define BUFF_SIZE 100	//	the size of buffer


void sigchld_handler(int s) {
    //  waitpid() migh overwriten errno, so we save and restore it:
    int saved_errno = errno;
    
    while(waitpid(-1 , NULL, WNOHANG) > 0);
    
    errno = saved_errno;
    
}

int main(int argc, char *argv[]) {
    
	//command check
	if (argc != 3){
        printf("Length of command is wrong, expected length is 3 !\n");
        exit(1);
    } else if (strcmp(commandString, argv[1]) != 0){
        printf("Wrong command, expected \"echos\" !\n");
        exit(1);
    }
	
    char data_buff[BUFF_SIZE];
    int sockfd, new_sockfd;
    socklen_t childlen;
    pid_t pid;	//	process ID
    struct sockaddr_in childaddr; // connector's address information
	struct sockaddr_in servaddr;// server address information
    struct sigaction sa;

	//	socket() with IPv4/TCP
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket Error");
		exit(1);
	}

    bzero(&servaddr, sizeof(servaddr));	//	zero the struct
 
    servaddr.sin_family = AF_INET;	// IPv4 byte order
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);	//	automatically fill with my IP
    servaddr.sin_port = htons(atoi(argv[2]));	// short, network byte order PORT number

	// bind()
    if(bind(sockfd,(struct sockaddr *) &servaddr, sizeof(servaddr)) == -1){
        perror("Bind Error");
        exit(1);
    }

	// listen() with maximum 10 connections
	if(listen(sockfd, 10) == -1) {
	    perror("Listen Error!");
	    exit(1);
	}
 
    //  handle the zombie processes  
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("Sigaction Error!\n");
        exit(1);
    }
    
    
	//	accept() with multi-process
    while(1) {
        childlen = sizeof(childaddr);

        if((new_sockfd = accept(sockfd, (struct sockaddr*) &childaddr, &childlen)) == -1 ) {
			printf("Accept client error: %s\n", strerror(errno));
			exit(1);
        } else {
            printf("Client connected\n");
        }        
         
        pid = fork(); 
        if(pid == 0) {	//	child process return

            close(sockfd);	// child doesn't need the listener
            printf("Client from %s\n", inet_ntoa(childaddr.sin_addr));

            while(1) {
                bzero(data_buff, BUFF_SIZE);	//	intial clear buffer

                //  readline()
                if(read(new_sockfd, data_buff, BUFF_SIZE) == 0) {
                };
                //readline(new_sockfd, data_buff, BUFF_SIZE);
                printf("Message from Client\n");
                printf("******************************************************\n");
                printf("%s", data_buff);
                printf("\n******************************************************\n");
                printf("Echo back...\n");
                write(new_sockfd, data_buff, strlen(data_buff) + 1);
            }
            exit(0);    
        } else if (pid < 0) {	//	fork error return 
        	printf("fork error: %s\n", strerror(errno));
        } else {	//	parent process return
            close(new_sockfd);	//	parent doesn't need this
        }
    }
}
