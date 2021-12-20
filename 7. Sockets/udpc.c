#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "udp.h"


int main(int argc, char *argv[])
{
	int sockfd, n, peerAddrLen;
	struct sockaddr_in servAddr, peerAddr;
	MsgType	msg;
	// create udp socket
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}
	// init zero
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	// address
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
	// port
	servAddr.sin_port = htons(SERV_UDP_PORT);

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	// send to server
	if (sendto(sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)  {
		perror("sendto");
		exit(1);
	}
	printf("Sent a request.....");
	peerAddrLen = sizeof(peerAddr);
	// receive from server
	if ((n = recvfrom(sockfd, (char *)&msg, sizeof(msg),
				0, (struct sockaddr *)&peerAddr, &peerAddrLen)) < 0)  {
		perror("recvfrom");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);
	// close socket
	close(sockfd);
	return 0;
}
