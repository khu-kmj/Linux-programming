#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destinaion\n", argv[0]);			
		exit(1);							
	}
		
	// symbolic link 설정		
	if (symlink(argv[1],argv[2])<0)  {	
		perror("link");			
		exit(1);			
	}
	return 0;
}
