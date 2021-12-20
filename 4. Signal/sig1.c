#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void SigIntHandler(int signo)
{
	printf("Received a SIGINT signal\n");
	printf("Terminate this process\n");

	exit(0);
}

int main()
{
	/* SIGINT signal handler: SigIntHandler */
	/* signal */
	// SIGINT :  프로세스 종료 신호(Ctrl + C)
	signal(SIGINT, SigIntHandler);
	printf("Press ^C to quit\n");

	for ( ; ; )
		pause();
	return 0;
}
