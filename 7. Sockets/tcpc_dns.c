#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include "tcp.h"


int main(int argc, char *argv[])
{
	int sockfd, n;
	struct sockaddr_in servAddr;
	MsgType msg;
	struct hostent *hp;
	// ./tcpc_dns dns와 같은 형태로 입력 받는다. 아닌 경우 종료
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s IPaddress\n", argv[0]);
		exit(1);
	}
	// create tcp socket
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}
	// init zero
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	// port number
	servAddr.sin_port = htons(SERV_TCP_PORT);
	
	// ip address의 첫 번째 주소가 숫자다 : Dotte-Decimal
	if (isdigit(argv[1][0]))  {
		//그러므로 inet_addr 사용
		servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	}
	// host name인 경우
	else  {
		// gethostbyname
		if ((hp = gethostbyname(argv[1])) == NULL)  {
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
		// memory copy
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
	}
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
