#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void CharAtaTime(char *str){	
	char *ptr;		
	int c, i;
		
	setbuf(stdout, NULL);		
	for (ptr = str ; c = *ptr++ ; )  {
		// 한 글자씩 실행하면서 할당된 Time quantum을 다 사용했기 때문에 다른 프로세스로 작업을 넘겨준다.
		// 그러다보니 parent와 child가 한 글자씩 출력이 된다.
		for(i = 0 ; i < 999999 ; i++)					
			;					
		putc(c, stdout);			
	}
}

int main(){	
	pid_t pid;
		
	if ((pid = fork()) < 0)  {		
		perror("fork");				
		exit(1);			
	}			
	else if (pid == 0)  {		
		CharAtaTime("output from child\n");			
	} 				
	else  {		
		CharAtaTime("output from parent\n");		
	}
	return 0;
}
