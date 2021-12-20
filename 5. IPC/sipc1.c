#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int shmid;
	char *ptr, *pData;
	int *pInt;
	
	// shared memory get
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	// shared memory attach
	if ((ptr = shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}
	
	// 0으로 초기화 되어있기 때문에 *pInt = 0인 상태
	pInt = (int *)ptr;
	while ((*pInt) == 0)
		;
	
	pData = ptr + sizeof(int);
	printf("Received request: %s.....", pData);
	sprintf(pData, "This is a reply from %d.", getpid());
	// sipc2로 인해 *pInt=1인데 0으로 변경 => sipc2 while 탈출
	*pInt = 0;
	printf("Replied.\n");
	// 잠시 시간을 주는 것
	sleep(1);
	// 제거
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}
	return 0;
}
