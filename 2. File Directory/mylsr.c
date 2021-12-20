#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define	MAX_PATH 256

void JustDoIt(char *path){	
	DIR *dp;		
	struct dirent *dep;			
	struct stat statbuf;			
	char fullpath[MAX_PATH];

	// myls와 동일
	if ((dp = opendir(path)) == NULL)  {						
		perror("opendir");					
		exit(0);				
	}
			
	printf("\n%s:\n", path);	
	while (dep = readdir(dp))  {
		// d_name이 "." or ".."인 경우 contunue를 통해 print하지 않겠다.		
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)				
			continue;										
		printf("%s\n", dep->d_name);				
	}

					
	// 디렉토리 정보 읽기 위치를 처음으로 이동	
	rewinddir(dp);

	// while문 시작
	while (dep = readdir(dp))  {		
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)				
			continue;

		// fullpath에 현재디렉토리경로(path)/d_name  값을 할당한다 ex) lsp05/d_name		
		strcpy(fullpath, path);
		strcat(fullpath, "/");				
		strcat(fullpath, dep->d_name);

		// 지정한 파일 정보를 담고 있는 stat 구조체를 얻는다.
		if (lstat(fullpath, &statbuf)<0)  {		
			perror("lstat");				
			exit(1);					
		}

		// 현재 fullpath가 directory가 맞다면 다시 JustDoIt 함수를 호출하여 정보를 읽어와라		
		if (S_ISDIR(statbuf.st_mode))  {		
			JustDoIt(fullpath);						
		}					
	}	
	// while문 종료
	closedir(dp);
}

int main(){	
	JustDoIt(".");
	return 0;
}
