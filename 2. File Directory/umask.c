#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int creat(const char *file, mode_t mode);
int main()
{
	// any permission may be set
	umask(0);
	//creat("bar", 0666) rw-rw-rw 권한을 가진 파일 bar 생성	
	if (creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0)  {
		perror("creat");						
		exit(1);				
	}
			
	/* Set file mode creation mask: S_IRGRP, S_IWGRP, S_IROTH, S_IWOTH */		
	
	// bitwise결과 umask(0066)과 동일
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); 
	// bitwise결과 creat("foo", 0666)과 동일 그러므로 0666 - 0-66 = 0600 즉 0600  권한을 가진 파일 foo가 생성 된다.
	if (creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0)  {					
		perror("creat");
		exit(1);		
	}
	return 0;
}
