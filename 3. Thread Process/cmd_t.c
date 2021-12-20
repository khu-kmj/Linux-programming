#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define	MAX_CMD		256

void DoCmd(char *cmd){	
	printf("Doing %s", cmd);		
	sleep(1);			
	printf("Done\n");		
	pthread_exit(NULL);
}

int main(){
	char cmd[MAX_CMD];	
	pthread_t tid;
		
	while (1)  {		
		printf("CMD> ");		
		fgets(cmd, MAX_CMD, stdin);					
		if (cmd[0] == 'q')						
			break;
		// thread 생성	
		if (pthread_create(&tid, NULL, (void *)DoCmd, (void *)tid)<0)  {	
			perror("pthread_create");					
			exit(1);						
		}
// #if 1인 경우 iteration 방식과 똑같이 동작
#if 0
		pthread_join(tid, NULL);
#endif	
	}
	return 0;
}
