#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "chat.h"

#define	DEBUG

#define	MAX_CLIENT 5
#define	MAX_ID 32
#define	MAX_BUF	256

typedef	struct  {
	int sockfd;
	int inUse;
	char uid[MAX_ID];
}ClientType;

int Sockfd;
pthread_mutex_t	Mutex;
ClientType Client[MAX_CLIENT];

int GetID()
{
	int i;
	
	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		// 사용하고 있지 않다면 사용하는 상태로 변경 후 i를 return
		if (! Client[i].inUse)  {
			Client[i].inUse = 1;
			return i;
		}
	}
}

void SendToOtherClients(int id, char *buf)
{
	int i;
	char msg[MAX_BUF+MAX_ID];
	
	sprintf(msg, "%s> %s", Client[id].uid, buf);
#ifdef	DEBUG
	printf("%s", msg);
	fflush(stdout);
#endif
	// lock
	pthread_mutex_lock(&Mutex);
	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		// 현재 접속한 사용자(자신을 제외한)에게 모두 send message
		if (Client[i].inUse && (i != id))  {
			if (send(Client[i].sockfd, msg, strlen(msg)+1, 0) < 0)  {
				perror("send");
				exit(1);
			}
		}
	}
	//unlock
	pthread_mutex_unlock(&Mutex);
}
	

void ProcessClient(int id)
{
	int newSockfd, cliAddrLen;
	struct sockaddr_in cliAddr;
	cliAddrLen = sizeof(cliAddr);
	// accept (newSockfd는 connection 이후 새로운 소켓 디스크립터)
	newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
	if (newSockfd < 0)  {
		perror("accept");
		exit(1);
	}
	// Client[id]의 sockfd에 newSockfd 저장
	Client[id].sockfd = newSockfd;
	char buf[MAX_BUF];
	int n;
	// receive from client
	if ((n = recv(Client[id].sockfd, Client[id].uid, MAX_ID, 0)) < 0)  {
		perror("recv");
		exit(1);
	}
	printf("Client %d log-in(ID: %s).....\n", id, Client[id].uid);	
}
void removeClient(int id)
{
	
	printf("Client %d log-out(ID: %s).....\n", id, Client[id].uid);		
	// lock
	pthread_mutex_lock(&Mutex);			
	// 입력받은 파라미터 id의 sockfd를 종료
	close(Client[id].sockfd);		
	Client[id].inUse = 0;			
	//unlock
	pthread_mutex_unlock(&Mutex);		
	Client[id].sockfd=0;
	SendToOtherClients(id, "log-out.....\n");	
}

void CloseServer(int signo)
{
	int i;
	// close Sockfd
	close(Sockfd);
	// 사용하고 있는 Client sockfd 종료
	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		if (Client[i].inUse)  {
			close(Client[i].sockfd);
		}
	}
	// destroy mutex
	if (pthread_mutex_destroy(&Mutex) < 0)  {
		perror("pthread_mutex_destroy");
		exit(1);
	}
	printf("\nChat server terminated.....\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	int n, id=0, one = 1;
	char buf[MAX_BUF];
	struct sockaddr_in servAddr;
	int count;
	fd_set fdvar;
	signal(SIGINT, CloseServer);
	// init mutex
	if (pthread_mutex_init(&Mutex, NULL) < 0)  {
		perror("pthread_mutex_init");
		exit(1);
	}
	// create tcp socket
	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}
	// 이미 사용된 주소를 재사용하도록 socket option 설정 
	if (setsockopt(Sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0)  {
		perror("setsockopt");
		exit(1);
	}
	// init zero
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	// 어디로 와도 다 처리하겠다는 의미
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT);
	// bind
	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}
	listen(Sockfd, 5);

	printf("Chat server started.....\n");
	// while : Chat server start
	while (1)  {
		// all zero 
		FD_ZERO(&fdvar);
		// Sockfd를 fdvar 구조체에 저장
		FD_SET(Sockfd, &fdvar);
		for(int i=0; i<=id; i++){
			// Client들의 sockfd를 fdvar 구조체에 저장
			FD_SET(Client[i].sockfd, &fdvar);
		}
		// select(read)
		if ((count = select(10, &fdvar, (fd_set *)NULL, (fd_set *)NULL,
			(struct timeval *)NULL)) < 0)  {
			perror("select");
			exit(1);
		}
		while (count--)  {
			// Sockfd case
			if(FD_ISSET(Sockfd, &fdvar)){
				// GetID를 사용하여 Use설정
				id=GetID();
				// Client로 부터 id receive
				ProcessClient(id); 
			}
			for(int i=0; i<=id; i++){
				// Client Sockfd case
				if(FD_ISSET(Client[i].sockfd, &fdvar)){
					// receive message	
					if ((n = recv(Client[i].sockfd, buf, MAX_BUF, 0)) < 0)  {
						perror("recv");
						exit(1);
					}
					// Client가 종료되었을 때 처리
					if(n==0){
						removeClient(i);
						continue;
					}
					// Send to Other Clients
					SendToOtherClients(i, buf);
				}
			}
		}
	}
	return 0;
}
