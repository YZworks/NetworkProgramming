# This is the makefile 
CC=gcc

all: client server 
	
client: 
	$(CC) echoClient.cpp -o echoClient

server: 
	$(CC) echoServer.cpp -o echoServer

	
clean:
	rm ./echoServer ./echoClient
	
runServer: all
	./echoServer echos 8090
	
runClient1:	
	./echoClient echo 127.0.0.1 8090

runClient2:	
	./echoClient echo 127.0.0.1 8090

runClient3:	
	./echoClient echo 127.0.0.1 8090
