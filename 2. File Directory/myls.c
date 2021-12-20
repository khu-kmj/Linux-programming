#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

int main()
{	
	DIR *dp;
	struct dirent *dep;		
	
	
	// 현재 디렉토리 열기
	if ((dp = opendir(".")) == NULL)  {			
		perror("opendir");					
		exit(0);				
	}	

	// while문 시작 : dp에 의해 지목된 디렉토리 안의 엔트리에  대한 포인터를 반환한다.
	// 하나의 엔트리씩 while문을 통해 읽고, 더 이상 엔트리가 없으면 NULL을 리턴하여 while문이 종료 된다.
	while (dep=readdir(dp))  {
		if(strcmp(dep->d_name,".")==0 ||strcmp( dep->d_name, "..")==0) continue;	
		printf("%s\n", dep->d_name);				
	}
	//while문 종료	
	closedir(dp);
	return 0;
}
