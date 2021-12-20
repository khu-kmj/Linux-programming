#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
static unsigned int AlarmSecs;


void SigAlarmHandler(int signo)
{
	/* UNIX 계열에 돌아가기 위해 있어야 한다.
	if (signal(SIGALRM, SigAlarmHandler)==SIG_ERR)  {
		perror("signal");
		exit(1);
	}
	*/
	alarm(AlarmSecs);

	/* Do something */
	printf(".");
	fflush(stdout);
	return;
}

int SetPeriodicAlarm(unsigned int nsecs)
{
	if (signal(SIGALRM, SigAlarmHandler)==SIG_ERR)  {
		return -1;
	}

	AlarmSecs = nsecs;
	// 1초 후 signal 발생
	alarm(nsecs);

	return 0;
}

int main()
{
	printf("Doing something every one seconds\n");
	
	//1초 마다 특정 일을 수행하는 함수
	SetPeriodicAlarm(1); 
	
	for ( ; ; )
		pause();
	return 0;
}
