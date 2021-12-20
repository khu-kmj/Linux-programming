#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

void myusleep(int secs, int usecs)
{
	struct timeval tm;
	tm.tv_sec=secs;
	tm.tv_usec=usecs;
	// select를 모두 NULL => 발생 하지 않는다.
	// 즉, 지정한 시간동안 기다린다.
	if(select(0,(fd_set *)0,(fd_set *)0, (fd_set *)0, &tm)<0){
		perror("select");
		return;
	}

}

int main()
{
	printf("Sleep for 1.5 seconds\n");
	myusleep(1, 500000);
	return 0;
}
