#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "fifo.h"


void SigIntHandler(int signo)
{
	// mkfifo로 만든 것을 삭제
	if (remove(SERV_FIFO) < 0)  {
		perror("remove");
		exit(1);
	}
	exit(0);
}


int main()
{
	int fd, cfd, n;
	MsgType	msg;
	
	if (signal(SIGINT, SigIntHandler) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}
	
	// make fifo
	if (mkfifo(SERV_FIFO, 0600) < 0)  {
		if (errno != EEXIST)  {
			perror("mkfifo");
			exit(1);
		}
	}
	// open
	if ((fd = open(SERV_FIFO, O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}

	while (1)  {
		// wait message
		if ((n = read(fd, (char *)&msg, sizeof(msg))) < 0)  {
			if (errno == EINTR)  {
				continue;
			}
			else  {
				perror("read");
				exit(1);
			}
		}
		printf("Received request: %s.....", msg.data);

		if ((cfd = open(msg.returnFifo, O_WRONLY)) < 0)  {
			perror("open");
			exit(1);
		}
		sprintf(msg.data, "This is a reply from %d.", getpid());
		write(cfd, (char *)&msg, sizeof(msg));
		close(cfd);
		printf("Replied.\n");
	}
	return 0;
}
