#include <stdio.h>

// envp : 환경변수
int main(int argc, char *argv[], char *envp[]){	
	int i;		
	char **p;
	// environ : 전역 변수를 사용하여 환경변수를 읽어온다.
	extern char **environ;
		
	printf("List command-line arguments\n");		
	for (i = 0 ; i < argc ; i++)  {						
		printf("%s\n", argv[i]);				
	}					
	printf("\n");		
	printf("List environment variables from environ variable\n");
#if 1								
	for (i = 0 ; environ[i] != NULL ; i++)  {		
		printf("%s\n", environ[i]);							
	}
#else					
	// pointer로 표현
	for (p = environ ; *p != NULL ; p++)  {		
		printf("%s\n", *p);					
	}
#endif	
	printf("\n");		
	printf("List environment variables from envp variable\n");
#if 1		
	// main의 3번째 파라미터를 사용하여 환경변수를 읽어온다.
	for (i = 0 ; envp[i] != NULL ; i++)  {		
		printf("%s\n", envp[i]);					
	}
#else			
	// pointer로 표현	
	for (p = envp ; *p != NULL ; p++)  {	
		printf("%s\n", *p);					
	}
#endif	
	return 0;
}
