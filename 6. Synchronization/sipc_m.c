#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

char Data[100];
pthread_mutex_t Mutex;
pthread_cond_t Cond;
// Condition을 구현하기 위해 flag 사용
int flag=0;

void Thread_sipc1(pthread_t tid){
	// mutex lock
	if (pthread_mutex_lock(&Mutex) < 0)  {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	// flag==0인 경우 cond wait
	while (flag == 0)  {
		if (pthread_cond_wait(&Cond, &Mutex) < 0)  {
			perror("pthread_cond_wait");
			pthread_exit(NULL);
		}
	}
	printf("Received request: %s.....\n", Data);      
	sprintf(Data, "this is a reply from %ld.",tid);
	printf("Replied.\n");
	flag=0;
	// cond signal
	if (pthread_cond_signal(&Cond) < 0)  {        
   		perror("pthread_cond_wait");	        
		pthread_exit(NULL);
        }
	// mutex unlock
        if(pthread_mutex_unlock(&Mutex)<0){        
  		perror("pthread_mutex_unlock");    
		pthread_exit(NULL);
	}	
	pthread_exit(NULL);
}
void Thread_sipc2(pthread_t tid){  
  	// mutex lock	
	if (pthread_mutex_lock(&Mutex) < 0)  {        
  		perror("pthread_mutex_lock");       
		pthread_exit(NULL);        
	}	
	
	sprintf(Data, "this is a reply from %ld.",tid);
	printf("Sent a request.....\n");
	flag=1;        
  	// cond signal 
	if (pthread_cond_signal(&Cond) < 0)  {        	
		perror("pthread_cond_wait");      
		pthread_exit(NULL);        
 	}        	
	// cond wait	
	if (pthread_cond_wait(&Cond, &Mutex) < 0){		
		perror("pthread_cond_wait");	
		pthread_exit(NULL);	
	}
	// mutex unlock
	if(pthread_mutex_unlock(&Mutex)<0){	
		perror("pthread_mutex_unlock");	
		pthread_exit(NULL);
	} 	
	printf("Received reply: %s\n", Data);
	pthread_exit(NULL);
}
int main(){
	pthread_t tid1, tid2;
	// Cond init
	if (pthread_cond_init(&Cond, NULL) < 0)  {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	// Mutex init
	if (pthread_mutex_init(&Mutex, NULL) < 0)  {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
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
	return 0;
}

