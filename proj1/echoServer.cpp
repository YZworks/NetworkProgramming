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
int main(int argc,char *argv[])
{
    if (argc != 3){
        printf("Length of command is wrong, expected length is 3 !\n");
        exit(1);
    }
    if (strcmp(commandString,argv[1]) != 0){
        printf("Wrong command, expected \"echos\" !\n");
        exit(1);
    }
    char str[100];
    int listen_fd, comm_fd;
    socklen_t chilen;
    pid_t pid;
    struct sockaddr_in chiaddr, servaddr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd== -1)
    {
        perror("listen");
        exit(1);
    }
 
    bzero( &servaddr, sizeof(servaddr));
 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[2]));
    printf("\nStart");
   // bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if(bind(listen_fd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1){
        perror("bind");
        exit(1);
    }
    printf("\nListening...");
    printf("\n");
    listen(listen_fd, 10);
 //***** multiprogress
    while(1)
    {
        chilen=sizeof(chiaddr);

        comm_fd=accept(listen_fd,(struct sockaddr*)&chiaddr,&chilen);
        printf("\nAccepted");
        printf("\n");
        if( comm_fd == -1)
        {    printf("accept client error: %s\n",strerror(errno));
             return -1;
        }
        else
        {
             printf("client connected\n");
        }        
         
        pid = fork(); //创建新进程的系统函数调用fork()
        if( pid == 0) // 子进程返回
        {
            close(listen_fd);
            printf("client from %s\n",inet_ntoa(chiaddr.sin_addr));
            while(1)
            {
         
                bzero( str, 100);
         
                read(comm_fd,str,100);
         
                printf("Echoing back - %s",str);
         
                write(comm_fd, str, strlen(str)+1);
         
            }
            exit(0);    
        }
        else if ( pid < 0) //fork调用失败返回-1 并设置errno
        {
               printf("fork error: %s\n",strerror(errno));
        }
        else  //父进程中返回
        {
            close(comm_fd);
        }
        
    }
//** end of multiprogress
}


// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <errno.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netdb.h>
// #include <arpa/inet.h>
// #include <sys/wait.h>
// #include <signal.h>
// int main()
// {
//     char str[100];
//     int listen_fd, comm_fd;
 
//     struct sockaddr_in servaddr;

//     listen_fd = socket(AF_INET, SOCK_STREAM, 0);
 
//     bzero( &servaddr, sizeof(servaddr));
 
//     servaddr.sin_family = AF_INET;
//     servaddr.sin_addr.s_addr = htons(INADDR_ANY);
//     servaddr.sin_port = htons(8080);
//     printf("\nStart");
//     bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
//     printf("\nListening...");
//     printf("\n");
//     listen(listen_fd, 10);
 
//     comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
//     printf("\nAccepted");
//     printf("\n");
//     while(1)
//     {
 
//         bzero( str, 100);
 
//         read(comm_fd,str,100);
 
//         printf("Echoing back - %s",str);
 
//         write(comm_fd, str, strlen(str)+1);
 
//     }
// }