#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

pid_t waitpid(pid_t pid, int *statloc , int options);

int main(){	
	pid_t pid;		
	char *argv[] = { "mycp", "exec.c", "exec2.c", NULL };
		
	// execl : list		
	if ((pid = fork()) < 0)  {	
		perror("fork");				
		exit(1);						 	
	}				 	
	else if (pid == 0)  {	
		// mycp exec.c exec2.c	
		// child는 exec을 통해 새로운 프로그램을 실행		
		/*if (execl("/home/lsp05/study5","mycp","exec.c","exec1.c",NULL)<0)  {				 
			perror("execl");				 
			exit(1);
		}*/
		if (execl("/bin/ls","ls", "-al", NULL)<0){
			perror("execl");
			exit(1);
		}		
	}			
	// 위 처리가 끝날 때까지 기다림
	if (waitpid(pid, NULL, 0) < 0)  {			 												
		perror("waitpid");
		exit(1);			
	}
							
	// execv : vector				 	
	if ((pid = fork()) < 0)  {		
		perror("fork");				 
		exit(1);			
	}			
	else if (pid == 0)  {	
		// mycp exec.c exec2.c			
		/*if (execv("/home/lsp05/study5",argv)<0)  {				 
			perror("execv");				 
			exit(1);				 	
		}*/
		char *argv2[] = {"ls", "-l", NULL };
		if (execv("/bin/ls", argv2)<0){
			perror("execv");
			exit(1);
		}		
	}	
	if (waitpid(pid, NULL, 0) < 0)  {	
		perror("waitpid");				 	
		exit(1);				 
	}			
	
	// execlp : env를 path에서 찾아서 실행
	if ((pid = fork()) < 0)  {		
		perror("fork");				
		exit(1);			
	}			
	else if (pid == 0)  {	
		// execute 'env' from PATH			
		if (execlp("env", "env", NULL)<0)  {				 	
			perror("execlp");				 
			exit(1);				 	
		}				
	}		
	if (waitpid(pid, NULL, 0) < 0)  {	
		perror("waitpid");				 	
		exit(1);				 
	}
	return 0;	
}
