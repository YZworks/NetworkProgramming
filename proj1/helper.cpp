#include <stdio.h>

#define MAX_TEXT_LEN 100
//	test code
void helper() {
	printf("The helepr file is included!");
}

int writen(int sockfd, char * buffer, size_t size) {	
	int count = 0;
	while(count < size) {
		int writenByte = write(sockfd, buffer, MAX_TEXT_LEN);
		
		//	write error detection
		if(writenByte == -1) {
			if(errno == EINTR) {
				printf("There is an Interrupt System Call!\n");
				continue;
			}
			perror("Write Error!\n");
			exit(1);
		}

		count += writenByte;
		buffer += writenByte;
	}

	printf("*****************\n");
	printf("Send %d Bytes\n", (int)size);
	return count;
}

int readline(int sockfd, char * buffer, size_t size) {
	
	while(1) {
		int readByte = read(sockfd, buffer, MAX_TEXT_LEN);

		//	read  error detection
		if(readByte == -1) {
			if(errno == EINTR) {
				printf("There is an Interrupt System Call!\n");
				continue;
			}
			perror("Read Error!\n");
			exit(1);
		} else if (readByte == 0) {
			break;
		}
			
		buffer += readByte;
	}	
	printf("Receive %lu Bytes from client\n", strlen(buffer) + 1);
	return 0;
}
