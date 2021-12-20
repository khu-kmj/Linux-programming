#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "shm.h"
#include "semlib.h"

int main(){
	int shmid,semid;
	char *ptr,*pData;
	int *pInt;
	// shared memory get
	if((shmid=shmget(SHM_KEY,SHM_SIZE,SHM_MODE))<0){
		perror("shmget");
		exit(1);
	}
	// shred memory attach
	if((ptr=shmat(shmid, 0, 0))==(void *)-1){
		perror("shmat");
		exit(1);
	}

	pInt=(int *)ptr;
	
	// semaphore init
	if((semid=semInit(SEM_KEY))<0){
		fprintf(stderr,"semInit failure\n");
		exit(1);
	}
	// sipc2에서 먼저 시작한다고 가정했다. (즉, child에서 semInitValue 사용)
	// Wait
	if(semWait(semid)<0){
		fprintf(stderr,"semWait failure\n");
		exit(1);
	}
	// Write data
	pData=ptr+sizeof(int);
	printf("Received request: %s.....",pData);
	sprintf(pData,"This is a reply from %d.\n",getpid());
	
	// Signal
	if(semPost(semid)<0){
		fprintf(stderr,"semPost failure\n");
		exit(1);
	}
	printf("Replied.\n");
	
	sleep(1);
	// shared memory delete
	if(shmctl(shmid,IPC_RMID,0)<0){
		perror("shmctl");
		exit(1);
	}
	return 0;
}

