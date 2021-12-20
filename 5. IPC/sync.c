#include <stdio.h>
#include <stdlib.h>
#include "synclib.h"
#define	NLOOPS	5

int main()
{
	int i;
	pid_t pid;
	
	TELL_WAIT();
	
	// parent, child process
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	// parent process
	else if (pid > 0)  {
		for (i = 0 ; i < NLOOPS ; i++)  {
			TELL_CHILD();
			printf("Parent: Tell to child\n");
			printf("Parent: Wait for child to tell\n");
			WAIT_CHILD();
		}
	}
	// child process
	else  {
		for (i = 0 ; i < NLOOPS ; i++)  {
			printf("Child: Wait for parent to tell\n");
			WAIT_PARENT();
			TELL_PARENT();
			printf("Child: Tell to parent\n");
		}
	}
	return 0;
}
