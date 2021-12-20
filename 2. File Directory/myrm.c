#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{	
	int i;	
	if (argc == 1)  {			
		fprintf(stderr, "Usage: %s filename\n", argv[0]);			
		exit(1);								
	}
	//입력한 항목들을 for문을 사용하여 remove한다.	
	for (i = 1 ; i < argc ; i++)  {		
		if (remove(argv[i])<0)  {				
			perror("remove");				
			exit(1);					
		}				
	}
	return 0;
}
