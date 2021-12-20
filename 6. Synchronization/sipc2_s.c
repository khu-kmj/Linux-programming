#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "shm.h"
#include "semlib.h"
#define N 2
int main()
{
	int shmid, semid;
	char *ptr, *pData;
	int *pInt;
	// shred memory get
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	// shared memory attach
	if ((ptr = shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}
	
	pInt=(int *)ptr;

	// semaphore init
	if ((semid=semInit(SEM_KEY)) < 0){
		fprintf(stderr, "semInit faulure\n");
		exit(1);
	}
	// sipc2_s를 먼저 시작한다고 가정하고 여기서 초기값을 지정해준다
	//  semaphore 초기값 1로 지정
	if (semInitValue(semid, 1) < 0){
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	for(int i=0; i<N; i++){
		// wait :  초기값 1이기 때문에 바로 사용 가능
		if (semWait(semid) < 0 ){
			fprintf(stderr, "semWait faulure\n");
			exit(1);
		}
		// write
		if(i==0){
			pData = ptr + sizeof(int);
			sprintf(pData, "This is a request from %d.", getpid());
			printf("Sent a request.....\n");
		}
		// read
		else{
			printf("Received reply: %s\n", pData);
		}
		// signal() semaphore 반납 +1
		if (semPost(semid) < 0 ){
			fprintf(stderr, "semPost faliure\n");
			exit(1);
		}
		// sleep을 하는 이유
		// : 초기값이 1이라서 sleep을 하지 않으면 혼자고 쓰고 혼자 읽는 문제가 발생
		sleep(2);
	}
	
	//  문자열 주고 받는 것을 끝냈기 때문에 semaphore delete
	if(semDestroy(semid)<0){
		fprintf(stderr, "semDestroy faulure\n");
		exit(1);	
	}
	return 0;
}
