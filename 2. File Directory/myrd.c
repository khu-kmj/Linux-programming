#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{	
	if (argc != 2)  {	
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);			
		exit(1);							
	}
	
	//argv[1]에 입력된 내용의 directory를 삭제한다.	
	if (rmdir(argv[1])<0)  {		
		perror("rmdir");			
		exit(1);				
	}
	return 0;
}
