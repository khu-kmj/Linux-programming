#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

// SIGCHLD handler
void SigchHandler(int signo)
{
	printf("SigHandler\n");
}

int main()
{
	pid_t pid;
	// sighandler 등록
	signal(SIGCHLD, SigchHandler);
	// child process
	if((pid=fork()) <0){
		perror("fork");
		exit(1);
	}
	// child
	else if(pid==0){
		printf("Child process : wait 2seconds \n");
		sleep(2);
		exit(0);
	}
	// parent
	else{
		printf("Parent process\n");
		/* Parent process : gugudan 실행
		for(int i=5; i<=9; i++){
			printf("------%d단------\n",i);
			for(int j=1; j<=9; j++)
				printf("%d x %d = %d\n", i,j,i*j);
		}*/
		while(1);
	}
	return 0;
}
