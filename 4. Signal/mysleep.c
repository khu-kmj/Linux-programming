#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void SigAlarmHandler(int signo)
{
	/* nothing to do, just return to wake up the pause */
	return;
}

unsigned int mysleep(unsigned int nsecs)
{
	/* SIGALRM signal handler: SigAlarmHandler */
	if (signal(SIGALRM, SigAlarmHandler)==SIG_ERR)  {
		return nsecs;
	}
	// nsecs 이후로 signal을 보내달라
	alarm(nsecs);
	// signal이 발생될 때 까지 pause를 통해 기다린다.
	pause();

	// 이전의 alarm으로 부터 시간이 얼마나 지났는지 return (정상인 경우 0)
	return alarm(0);
}

int main()
{
	printf("Wait for 5 seconds...\n");
	mysleep(5);

	return 0;
}


