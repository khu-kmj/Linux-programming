#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int mysystem(char *cmd){
	pid_t pid;
	int status;
	// sysetm() 함수는 실행 쉘인 /bin/sh -c str을 호출하여 str에 지정된 명령어를 실행한다.
	char *vec[]={"bin/sh", "-c", cmd, NULL};

	//fork() => parent and child process
	if((pid=fork()) < 0) {
		perror("fork");
		exit(1);
	}
	//child
	else if (pid==0) {
		// path에 지정한 경로명의 파일을 실행한다 vec를 인자로 전달
		if (execv("/bin/sh",vec)<0){
			perror("execlp");
			exit(1);
	
		}
	}
	// child 프로세스를 기다린다
	if (waitpid(pid, &status, 0)<0){
		perror("waitpid");
		exit(1);
	}
	return status;
}

int main(){
	mysystem("ls -al");
	mysystem("date");
	mysystem("who");
	return 0;
}
