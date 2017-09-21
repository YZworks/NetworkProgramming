# This is the makefile 
CC=gcc

all: client server 
	
client: 
	$(CC) Client.cpp -o Client

server: 
	$(CC) Server.cpp -o Server

	
clean:
	rm ./Server ./Client
	
runServer: all
	./Server echos 8090
	
runClient1:	
	./Client echo 127.0.0.1 8090

runClient2:	
	./Client echo 127.0.0.1 8090

runClient3:	
	./Client echo 127.0.0.1 8090
