#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#define MAX 1000

// 현재 시간은 ls-l에서 표기된 것 처럼 바꿔주는 함수
void change_time(char str[MAX], time_t t){
	char a[MAX];
	struct tm *time; 
	time=localtime(&t); // time_t의 형태를 struct tm 형태로 변환한다. 
	strftime(a,sizeof(a), "%b %d %H:%M",time); //strftime => 원하는 형식대로 출력할 수 있는 함수
	strcpy(str,a);
}

int main()
{
	DIR *dp;
	struct dirent *dep;
	struct passwd *pwd;
	struct group *g;
	struct stat s;
	if ((dp = opendir(".")) == NULL)  {		
		perror("opendir");					
		exit(0);				
	}

	while (dep = readdir(dp))  {
		lstat(dep->d_name,&s);
		if(strcmp(dep->d_name,".")==0 || strcmp(dep->d_name,"..")==0) continue;	
		
		// S_FMT : st_mode 값에서 파일의 종류를 정의한 부분을 가져온다.
		switch (S_IFMT & s.st_mode){
			case S_IFREG:
				printf("-");
				break;
			case S_IFDIR:
				printf("d");
				break;
			case S_IFBLK:
				printf("b");
				break;
			case S_IFCHR:
				printf("c");
				break;
			case S_IFLNK:
				printf("l");
				break;
			case S_IFIFO:
				printf("p");
				break;
			case S_IFSOCK:
				printf("s");
				break;
		}
		
		if(S_IRUSR & s.st_mode)	
			printf("r");
		else printf("-"); 
		
		if(S_IWUSR & s.st_mode)			    
			 printf("w");                 
		else printf("-");
		
		if(S_IXUSR & s.st_mode){
       			// 현재 파일에 대한 접근 권한이 존재하고 SetUID가 활성화 되어있다면 's'를 출력			
			if(S_ISUID & s.st_mode) 
				printf("s");
			else printf("x");         
		}
		else{	
			// 현재 파일에 대한 접근 권한이 존재하지 않고 SetUID가 활성화 되어있다면 'S'를 출력
			if(S_ISUID & s.st_mode)
				printf("S");
			else   	printf("-");
		}

		if(S_IRGRP & s.st_mode)			
			printf("r");				       
       		else printf("-");				
		
		if(S_IWGRP & s.st_mode)				
			printf("w");				
		else printf("-");				
		
		if(S_IXGRP & s.st_mode){
			// 현재 파일에 대한 접근 권한이 존재하고 SetGID가 활성화 되어있다면 's'를 출력			
			if(S_ISGID & s.st_mode)
				printf("s");
			else printf("x");	
		}			
		else{
			// 현재 파일에 대한 접근 권한이 존재하지 않고 SetGID가 활성화 되어있다면 'S'를 출력
			if(S_ISGID & s.st_mode)
				printf("S");
			else printf("-");
		}

  		if(S_IROTH & s.st_mode)			
			printf("r");				       
       		else printf("-");				
		
		if(S_IWOTH & s.st_mode)				
			printf("w");				
		else printf("-");				
		
		if(S_IXOTH & s.st_mode){   
			//g 현재 파일에 대한 접근 권한이 존재하고 Sticky bit가 활성화 되어있다면 't'를 출력
			if(S_ISVTX & s.st_mode)
				printf("t");			
			else printf("x");	
		}			
		else{
			// 현재 파일에 대한 접근 권한이 존재하지 않고 Sticky bit가 활성화 되어있다면 'T'를 출력
			if(S_ISVTX & s.st_mode)
				printf("T");
		        else printf("-");
		}
		char str[MAX];
		change_time(str, s.st_mtime); // 파일이 마지막으로 수정된 시간정보를 change_time 함수에 넣어 원하는 형태로 값을 저장한다.
		pwd=getpwuid(s.st_uid); 
		g=getgrgid(s.st_gid);
		printf(" %ld %s %s %5ld %s %s\n",s.st_nlink, pwd->pw_name, g->gr_name, s.st_size, str, dep->d_name);		
	}					
	closedir(dp);
	return 0;
}
