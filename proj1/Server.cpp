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

int main(int argc,char *argv[]) {
    
	//command check
	if (argc != 3){
        printf("Length of command is wrong, expected length is 3 !\n");
        exit(1);
    } else if (strcmp(commandString, argv[1]) != 0){
        printf("Wrong command, expected \"echos\" !\n");
        exit(1);
    }
	
    char str[100];
    int listen_fd, comm_fd;	// listen on socket listen_fd, and new connection on socket comm_fd 
    socklen_t chilen;
    pid_t pid;	//	process ID
    struct sockaddr_in chiaddr; // connector's address information
	struct sockaddr_in servaddr;// server address information

	//	socket() with IPv4/TCP
	if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket Error");
		exit(1);
	}

    bzero(&servaddr, sizeof(servaddr));	//	zero the struct
 
    servaddr.sin_family = AF_INET;	// IPv4 byte order
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);	//	automatically fill with my IP
    servaddr.sin_port = htons(atoi(argv[2]));	// short, network byte order PORT number
    printf("\nStart");

	// bind()
    if(bind(listen_fd,(struct sockaddr *) &servaddr, sizeof(servaddr)) == -1){
        perror("Bind Error");
        exit(1);
    }

    printf("\nListening...");
    printf("\n");
    
	// listen() with maximum 10 connections
	listen(listen_fd, 10);
 
	//	accept() with multi-process
    while(1) {
        chilen=sizeof(chiaddr);

        if((comm_fd = accept(listen_fd, (struct sockaddr*) &chiaddr, &chilen)) == -1 ) {
			printf("accept client error: %s\n",strerror(errno));
			return -1;
        } else {
            printf("client connected\n");
        }        
         
        pid = fork(); 
        if(pid == 0) {	//	child process return
            close(listen_fd);	// child doesn't need the listener
            printf("client from %s\n",inet_ntoa(chiaddr.sin_addr));
            while(1) {
                bzero(str, 100);	//	intial clear buffer
                read(comm_fd,str,100);
                printf("Echoing back - %s",str);
                write(comm_fd, str, strlen(str)+1);
            }
            exit(0);    
        } else if (pid < 0) {	//	fork error return 
        	printf("fork error: %s\n",strerror(errno));
        } else {	//	parent process return
            close(comm_fd);	//	parent doesn't need this
        }
    }
}
