#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define	MAX_BUF	128

void main()
{
	int n, fd1[2], fd2[2];
	pid_t pid;
	char buf[MAX_BUF];
	// Make two pipes
	if (pipe(fd1) < 0 || pipe(fd2) < 0)   {
		perror("pipe");
		exit(1);
	}
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	// child
	else if (pid == 0)  {
		// fd1 write close
		close(fd1[1]);
		// fd2 read close
		close(fd2[0]);
		printf("Child : Wait for parent to send data\n");
		// wait message (read message to pipe1)
		if ((n = read(fd1[0], buf, MAX_BUF)) < 0)  {
			perror("read");
			exit(1);
		}
		printf("Child : Received data from parent: ");
		fflush(stdout);
		write(STDOUT_FILENO, buf, n);
		
		strcpy(buf, "I'm 25 years old.");
		printf("Child : Send data to parent\n");
		// send data (pipe2)
		write(fd2[1], buf, strlen(buf)+1);

	}
	// parent
	else  {
		// fd1 read close
		close(fd1[0]);
		// fd2 write close
		close(fd2[1]);
		strcpy(buf, "Hello, World!\n");
		printf("Parent: Send data to child\n");
		// send data (pipe1)
		write(fd1[1], buf, strlen(buf)+1);
		// wait message (read message to pipe2)
		if((n=read(fd2[0], buf, MAX_BUF)) < 0 ){
			perror("read");
			exit(1);
		}
		printf("Parent: Received data from child: ");
		fflush(stdout);
		write(STDOUT_FILENO, buf, n);
		printf("\n");
	}
	exit(0);
}
