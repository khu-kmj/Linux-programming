#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
	struct stat statbuf;
	// bar 파일 정보를 stat 구조체에 복사한다.	
	if (stat("bar", &statbuf) < 0)  {				
		perror("stat");					
		exit(1);			
	}
	// bar파일의 기존 권한에서 그룹 실행 권한을 빼고 SetUid 권한을 추가한다.	
	if (chmod("bar", (statbuf.st_mode & ~S_IWGRP) | S_ISUID) < 0)  {			
		perror("chmod");							
		exit(1);					
	}
	//setuid 비트가 설정되어 있으면 사용자 접근권한의 실행 권한 자리에 실행 권한이 있으면 소문자 s, 실행권한이 없으면 대문자 S로 표기
			
	/* S_IRUSR, S_IWUSR, S_IRGRP, S_IROTH : ON */
	// chmod("foo", 0644) 	
	if (chmod("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)<0)  {						
		perror("chmod");			
		exit(1);				
	}
	return 0;
}
