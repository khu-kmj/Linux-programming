#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pwd.h>


void MyAlarmHandler(int signo)
{
	struct passwd *rootptr;
	signal(SIGALRM, MyAlarmHandler);
	alarm(1);
	printf("in signal handler\n");

	/* cjs가 return되기 전에 getpwnam를 호출했다
	 * 그러므로 구조체가 root로 바뀐다. static struct passwd root */
	if ((rootptr = getpwnam("root")) == NULL)  {
		perror("getpwnam");
		exit(1);
	}

	return;
}

int main()
{
	struct passwd *ptr;

	signal(SIGALRM, MyAlarmHandler);
	alarm(1);

	for ( ; ; )  {
		/* static struct passwd cjs  */
		if ((ptr = getpwnam("lsp05")) == NULL)  {
			perror("getpwnam");
			exit(1);
		}
		// ptr->pw_name = root 즉 충돌이 발생한다.
		if (strcmp(ptr->pw_name, "lsp05") != 0)  {
			printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
			exit(0);
		}
	}
	return 0;
}
