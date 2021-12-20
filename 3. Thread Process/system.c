#include <stdio.h>
#include <stdlib.h>

int main(){	
	int status;
	
	// date : 날짜를 출력
	if ((status = system("date")) < 0)  {		
		perror("system");					
		exit(1);				
	}			
	printf("exit status = %d\n", status);
	
	// nosuchcommand not found
	if ((status = system("nosuchcommand")) < 0)  {	
		perror("system");						
		exit(1);				
	}			
	printf("exit status = %d\n", status);
					
	/* $ who; exit 44 */
	// who : 누가 로그인 했는지 알려줌	
	if (system("who; exit 44")<0)  {			
		perror("system");			
		exit(1);				
	}			
	printf("exit status = %d\n", status);
	
 	/* 연습용코드
  	printf("화면을 지웁니다.\n");	
       	if (system("clear")<0)  {
		perror("system");		
		exit(1);	
	}	       	
	printf("exit status = %d\n", status);*/
	return 0;
}
