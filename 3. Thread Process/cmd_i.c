#include <stdio.h>
#include <unistd.h>
#define	MAX_CMD	256

// Iteration, 요청이 끝날 때 까지 기다렸다가 다른 요청을 처리한다.
void DoCmd(char *cmd){	
	printf("Doing %s", cmd);		
	sleep(1);			
	printf("Done\n");
}

int main(){	
	char cmd[MAX_CMD];
		
	while (1)  {	
		printf("CMD> ");		
		fgets(cmd, MAX_CMD, stdin);					
		if (cmd[0] == 'q')						
			break;
			
		DoCmd(cmd);	
	}
	return 0;
}
