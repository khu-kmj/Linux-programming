#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "semlib.h"
#include "prodcons.h"

int main()
{
	BoundedBufferType *pBuf;
	int shmid, i, data;
	int emptySemid, fullSemid, mutexSemid;
	// shared memory get
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	// shared memory attach
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}
	/* semaphore init*/
	// empty : 데이터가 안들어가 있는지
	if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	// full : 데이터가 들어가 있는지
	if ((fullSemid = semInit(FULL_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	// mutex : critical section
	if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	/* init value set */
	// 초기에는 모두 비어있기 때문에 MAX_BUF
	if (semInitValue(emptySemid, MAX_BUF) < 0)  {
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	// 초기에는 모두 비어있기 때문에 0
	if (semInitValue(fullSemid, 0) < 0)  {
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	// critical section
	if (semInitValue(mutexSemid, 1) < 0)  {
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}

	srand(0x9999);
	for (i = 0 ; i < NLOOPS ; i++)  {
		// wait(full)
		if (semWait(fullSemid) < 0)  {
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		// wait(mutex)
		if (semWait(mutexSemid) < 0)  {
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		printf("Consumer: Consuming an item.....\n");
		data = pBuf->buf[pBuf->out].data;
		pBuf->out = (pBuf->out + 1) % MAX_BUF;
		pBuf->counter--;
		// signal(mutex)
		if (semPost(mutexSemid) < 0)  {
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		// signal (empty)
		if (semPost(emptySemid) < 0)  {
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		usleep((rand()%100)*10000);
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", pBuf->counter);
	return 0;
}
