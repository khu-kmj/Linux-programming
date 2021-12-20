#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	int pid;
	// fork함수를 호출 => 부모 프레세스, 새롭게 생성되는 자식 프로세스
	if ((pid=fork())<0)  {	
		perror("fork");			
		exit(1);			
	}			
	else if (pid == 0)  {	
		/* child */					
		printf("I am %d. My parent is %d.\n", getpid(), getppid());				
	}				
	else  {
		/* parent */		
		printf("I am %d. My child is %d.\n", getpid(), pid);			
	}
	return 0;
}
