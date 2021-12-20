#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define	MAX_CMD	256

void DoCmd(char *cmd){	
	printf("Doing %s", cmd);		
	sleep(1);			
	printf("Done\n");
}

int main(){	
	char cmd[MAX_CMD];	
	int pid;
	
	while (1)  {		
		printf("CMD> ");		
		fgets(cmd, MAX_CMD, stdin);					
		if (cmd[0] == 'q')						
			break;
		// process 생성		
		if ((pid=fork())<0)  {		
			perror("fork");				
			exit(1);				
		}
		// child process		
		else if (pid == 0)  {		
			DoCmd(cmd);				
			exit(0);				
		}
// 컴파일이 안된다. 그러므로 바로 새로운 커멘드를 받을 수 있다.
// #if 1 인 경우에는 child가 끝날 때 까지 기다린다. 즉 lteration 방식과 똑같이 동작
#if 0
		else  {
			wait(NULL);		
		}
#endif				
	}
	return 0;
}
