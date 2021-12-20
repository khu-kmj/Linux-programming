#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{	
	if (argc != 2)  {	
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);			
		exit(1);	
	}
	
	// directory 주소를 변경한다.
	// 근데 실제로는 변경되지 않는다 왜?
	// 현재 mycd 안의 chdir() 시스템 콜을 수행하게 되면 fork를 통해 똑같은 Process가 하나 생성되고
	// 이 프로세스 내에서 chdir가  수행되는 것이다. 그러므로 이 프로세스가 종료되면 없어진다. 그렇기 때문에 변경되지 않는다.	
	if (chdir(argv[1])<0)  {	
		perror("chdir");			
		exit(1);				
	}
	return 0;
}
