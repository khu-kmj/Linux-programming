#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() 
{
	int fd[2];
	pid_t child;
	
	if(pipe(fd) == -1)  {
		perror("pipe");
		exit(1);
	}
 	
	child = fork();

	// error
	if(child == (pid_t)(-1))  {
        	perror("fork");
		exit(1);
	}
	// child
	// 출력
	else if(child == (pid_t)0) {
        	close(1);
		close(fd[0]);
        	
		// 1, fd[1] dup => dup2(fd[1], 1)
        	if(dup(fd[1]) == -1)  {
        		perror("dup");
			exit(1);
		}

        	if((execlp("ls", "ls", "-l", NULL)) == -1)  {
        		perror("execlp");
			exit(1);
		}
	} 
	// parent
	// 입력
	else {
        	close(0);
		close(fd[1]);
        	
		// 0, fd[0] dup => dup(fd[0])
        	if(dup2(fd[0], 0) == -1)  {
        		perror("dup");
			exit(1);
		}

        	if((execlp("more", "more", NULL)) == -1)  {
        		perror("execlp");
			exit(1);
		}
	}
	return 0;
}
