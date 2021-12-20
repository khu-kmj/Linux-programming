#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcp.h"


int main(int argc, char *argv[])
{
	int sockfd, n;
	struct sockaddr_in servAddr;
	MsgType	msg;
	// create tcp socket
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}
	// init zero
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	// address
	// Dotte-Decimal을 바이너리로 변경
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
	// port
	servAddr.sin_port = htons(SERV_TCP_PORT);
	// connect with server
	if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("connect");
		exit(1);
	}
	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	// write message
	if (write(sockfd, (char *)&msg, sizeof(msg)) < 0)  {
		perror("write");
		exit(1);
	}
	printf("Sent a request.....");
	// read message
	if ((n = read(sockfd, (char *)&msg, sizeof(msg))) < 0)  {
		perror("read");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);
	// close socket
	close(sockfd);
	return 0;
}
