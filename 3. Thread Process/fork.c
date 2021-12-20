#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int Var = 6;
char Buf[] = "a write to stdout\n";

int main(){
	int var;	
	pid_t pid;
			
	var = 88;	

	// printf와 유사 동작
	write(STDOUT_FILENO, Buf, sizeof(Buf)-1);		
	printf("Before fork\n");
	
	// fork 함수를 호출하여 부모 프로세스와 자식 프로세스 생성	
	if ((pid = fork()) < 0)  {	
		perror("fork");				
		exit(1);			
	}
	// fork의 return value가 0이면 child	
	else if (pid == 0)  {	
		/* child */			
		Var++;			
		var++;		
	}
	// fork의 return value가 0보다 크면 parent	
	else  {	
		/* parent */	
		sleep(2);			
	}

	// child, parent에서 사용  printf문은 2번 실행된다.
	// child process를 종료 후 parent를 실행한다.
	// 그러므로 parent process에서는 var, Var가 증가되지 않은 상태로 출력
	printf("pid = %d, Var = %d, var = %d\n", getpid(), Var, var);
	return 0;
}
