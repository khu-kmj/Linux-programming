#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{	
	if (argc != 2)  {	
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);			
		exit(1);							
	}		
	/* 755 permission */
	// directory를 rwxr-xr-x 옵션으로 생성	
	if (mkdir(argv[1], 0755)<0)  {			
		perror("mkdir");			
		exit(1);				
	}
	return 0;
}
