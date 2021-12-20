#include <stdio.h>
#include <stdlib.h>

void myexit1(){	
	printf("first exit handler\n");
}

void myexit2(){	
	printf("second exit handler\n");
}

int main(){
	/* Add myexit2 to exit functions */
	// 프로그램이 종료되고 나면 exit handler의 경우 등록 순서의 역순으로(스택 형태) 실행	
	if (atexit(myexit2)!=0)  {			
		perror("atexit");			
		exit(1);				
	}
		
	if (atexit(myexit1) != 0)  {	
		perror("atexit");				
		exit(1);				
	}
			
	if (atexit(myexit1) != 0)  {	
		perror("atexit");				
		exit(1);				
	}			
	printf("main is done\n");
	return 0;
}
