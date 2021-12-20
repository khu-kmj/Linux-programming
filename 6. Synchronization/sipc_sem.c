#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "semlib2.h"


char Data[100];
sem_t Sem, Sem2;

void Thread_sipc1(pthread_t tid){
        // semaphore wait
        if(sem_wait(&Sem2)<0){
		perror("sem_wait");
		pthread_exit(NULL);
	}
	// semaphore wait
	if (sem_wait(&Sem) < 0)  {
                perror("sem_wait");
                pthread_exit(NULL);
        }
	// read
	printf("Received reply: %s.....\n",Data);
        // write
       	sprintf(Data, "this is a reply from %ld.",tid);
        printf("Replied.\n");
        // semaphore signal
        if(sem_post(&Sem)<0){
                perror("sem_post");
                pthread_exit(NULL);
        }
        pthread_exit(NULL);
}

void Thread_sipc2(pthread_t tid){  
  	// semaphore wait
	if (sem_wait(&Sem) < 0)  {        
  		perror("sem_wait");       
		pthread_exit(NULL);        
	}	
	sprintf(Data, "this is a reply from %ld.",tid);
	printf("Sent a request.....\n");
	// semaphore signal
	if(sem_post(&Sem)<0){	
		perror("sem_post");	
		pthread_exit(NULL);
	} 	
	if(sem_post(&Sem2)<0){
		perror("sem_post");
		pthread_exit(NULL);
	}
	sleep(1);
	printf("Received reply: %s\n", Data);
	pthread_exit(NULL);
}
int main(){
	pthread_t tid1, tid2;
	// Cond init 초기값 1
	if (sem_init(&Sem, 0 ,1)<0){
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&Sem2, 0, 0)<0){
		perror("sem_init");
		exit(1);
	}
	// create two pthread
	if(pthread_create(&tid1,NULL,(void*)Thread_sipc1, &tid1)<0){
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&tid2,NULL,(void*)Thread_sipc2, &tid2)<0){
		perror("pthread_create");
		exit(1);
	}

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

	if(sem_destroy(&Sem)<0){
		perror("sem_destroy");
	}
	if(sem_destroy(&Sem2)<0){
		perror("sem_destroy");
	}
	return 0;
}
