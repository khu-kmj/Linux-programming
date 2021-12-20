#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "chat.h"

#define	MAX_BUF	256

int Sockfd;
char buf[MAX_BUF];
void Receive()
{
	int n;	
	//while
	while(1){
		//buf에 데이터를 받아온다.
		if ((n = recv(Sockfd, buf, MAX_BUF, 0)) < 0)  {	      
      			perror("recv");		        	
			pthread_exit(NULL);				       
		}	
		//server terminated 상태 처리
		if (n == 0)  {		  	
			fprintf(stderr, "Server terminated.....\n");			
			close(Sockfd);					            	
			pthread_exit(NULL);	
		}
		// buf에 받아온 데이터 출력
		printf("%s", buf);
	}
	pthread_exit(NULL);
}
void Send()
{
	int n; 
 	//while	
	while(1){
		// 보낼 데이터 입력
		fgets(buf, MAX_BUF, stdin);
		// send buf data		
		if ((n = send(Sockfd, buf, strlen(buf)+1, 0)) < 0)  {				   
	  		perror("send");					             
			pthread_exit(NULL);				
		}
	}
	pthread_exit(NULL);
}

void CloseClient(int signo)
{       	
	// close socket
	close(Sockfd);
	printf("\nChat client terminated.....\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	pthread_t r,s;
	struct sockaddr_in servAddr;
	struct hostent *hp;
	// Server IP를 입력하지 않았을 때 예외처리
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s ServerIPaddress\n", argv[0]);
		exit(1);
	}
	// create socket
	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}
	// init zero
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	// port
	servAddr.sin_port = htons(SERV_TCP_PORT);
	// Dotte-Decimal
	if (isdigit(argv[1][0]))  {
		servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	}
	// host name
	else  {
		if ((hp = gethostbyname(argv[1])) == NULL)  {
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
	}
	// connect with server
	if (connect(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("connect");
		exit(1);
	}

	printf("Enter ID: ");
        	fflush(stdout);	  
  	fgets(buf, MAX_BUF, stdin);		       
       	*strchr(buf, '\n') = '\0';		        
	// sernd my id to server
	if (send(Sockfd, buf, strlen(buf)+1, 0) < 0)  {				
		perror("send");						
		exit(1);			
	}		
	printf("Press ^C to exit\n");	
	// create send thread
	if (pthread_create(&s, NULL, (void *)Send, NULL) < 0){
		perror("pthread_create");
		exit(1);
	}
	// create receive thread
	if (pthread_create(&r, NULL, (void *)Receive, NULL) < 0){
		perror("pthread_create");
		exit(1);
	}
	// thread join
	if (pthread_join(s, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(r, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}
	// signal ^C
	signal(SIGINT, CloseClient);
	return 0;
}
