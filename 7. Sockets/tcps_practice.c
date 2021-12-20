#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "tcp.h"

int Sockfd;

// SIGINT signal handler
void CloseServer()
{
	// close socket
	close(Sockfd);
	printf("\nTCP Server exit.....\n");
	exit(0);
}


int main(int argc, char *argv[])
{
	int newSockfd, cliAddrLen, n;
	int pid;
	
	struct sockaddr_in cliAddr, servAddr;
	MsgType	msg;
	// signal
	signal(SIGINT, CloseServer);
	// create tcp socket
	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}
	// init zero
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	// ip address
	// INADDR_ANY : 어디로 와도 다 처리하겠다는 의미
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// port number
	// htons 네으쿼크 byte로 바꿈(short)
	servAddr.sin_port = htons(SERV_TCP_PORT);
	// bind 
	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5);

	printf("TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		// accept : newSockfd는 connection 이후의 새로운 소켓 디스크립터를 받음
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
		// fork process
		if(newSockfd<0){
			perror("accpt");
			exit(1);
		}	
			// read message	
		if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
                       	perror("read");
                       	exit(1);
               	}
               	printf("Received request: %s.....", msg.data);
			    	
		msg.type = MSG_REPLY;
                sprintf(msg.data, "This is a reply from %d.", getpid());
                // write message
		if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
                       	perror("write");
                       	exit(1);
                }
                printf("Replied.\n");
                usleep(10000);
                close(newSockfd);	
	}
	return 0;
}
