#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdlib.h>
#include "synclib.h"
#define	NLOOPS 10
#define	SIZE sizeof(long)

#define MALLOC_SIZE 100000
#define SHM_SIZE 100000
#define SHM_MODE (SHM_R | SHM_W)

int update(long *ptr)
{
	// shared memory 1씩 증가
	return ((*ptr)++);
}

int main()
{
	int shmid, i, counter;
	pid_t	pid;
	char *shmptr;
	
	// shared memory get
	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) <0){
		perror("shmget");
		exit(1);
	}
	// shared memory attach
	if ((shmptr = shmat(shmid, 0, 0)) == (void *) -1){
		perror("shmat");
		exit(1);
	}

	TELL_WAIT();
	// fork
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	// parent
	else if (pid > 0)  {
		for (i = 0 ; i < NLOOPS ; i += 2)  {
			if ((counter = update((long *) shmptr)) != i)  {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter);
			TELL_CHILD();
			WAIT_CHILD();
		}
	}
	// child
	else  {
		for (i = 1 ; i < NLOOPS ; i += 2)  {
			WAIT_PARENT();
			if ((counter = update((long *) shmptr)) != i)  {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			TELL_PARENT();
		}
	}
	/* IPC_PRIVATE이므로 삭제 코드 생략 가능   */
	if (shmctl(shmid, IPC_RMID, 0 ) < 0) {
		perror("shmctl");
		exit(1);
	}
	return 0;
}
