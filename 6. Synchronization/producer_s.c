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
	/*semaphore init*/
	// empty : 데이터가 안들어가있는지
	if ((emptySemid = semInit(EMPTY_SEM_KEY) ) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	// full : 데이터가 들어가있는지
	if ((fullSemid = semInit(FULL_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	// mutex : critical section
	if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	srand(0x8888);
	for (i = 0 ; i < NLOOPS ; i++)  {
		// wait(empty)
		if (semWait(emptySemid) < 0)  {
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		// wait (mutex)
		if (semWait(mutexSemid) < 0)  {
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000;
		pBuf->buf[pBuf->in].data = data;
		pBuf->in = (pBuf->in + 1) % MAX_BUF;
		pBuf->counter++;
		// signal(mutex)
		if (semPost(mutexSemid) < 0)  {
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		// signal(full)
		if (semPost(fullSemid) < 0)  {
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		usleep(data);
	}
	printf("Producer: Produced %d items.....\n", i);
	sleep(2);
	printf("Producer: %d items in buffer.....\n", pBuf->counter);
	
	/* semaphore destroy */
	if (semDestroy(emptySemid) < 0)  {
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(mutexSemid) < 0)  {
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(fullSemid) < 0)  {
		fprintf(stderr, "semDestroy failure\n");
	}
	// shared momoery remove
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}
	return 0;
}
