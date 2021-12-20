#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s old new\n", argv[0]);			
		exit(1);							
	}
	//현재 파일명을 argv[2]에 입력된 내용으로 rename	
	if (rename(argv[1],argv[2])<0)  {		
		perror("rename");			
		exit(1);				
	}
	return 0;
}
