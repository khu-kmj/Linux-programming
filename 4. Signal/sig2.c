#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
void SigUsrHandler(int signo)
{
	if (signo == SIGUSR1)  {
		printf("Received a SIGUSR1 signal\n");
	}
	else if (signo == SIGUSR2)  {
		printf("Received a SIGUSR2 signal\n");
	}
	// 나머지 signo일 때 종료
	else  {
		printf("Received unknown signal\n");
		printf("Terminate this process\n");
		exit(0);
	}
}

int main()
{
	// SIGURS1 handler
	if (signal(SIGUSR1, SigUsrHandler) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}
	// SIGUSR2 handler
	if (signal(SIGUSR2, SigUsrHandler) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}
	for ( ; ; )
		pause();
}
