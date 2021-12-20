#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

//i-nodes 역할
int main(int argc, char *argv[])
{
	struct stat statbuf;
	char *mode;
	int i;
	
	for (i = 1 ; i < argc ; i++)  {
		printf("%s: ", argv[i]);

		// lstat : 파일에 대한 정보를 얻고 성공적으로 수행되면 stat 구조체에 복사한다.
		// lstat는 stat와 유사하나 지정한 파일이 심볼릭 링크인 경우 링크 파일 자체의 정보를 전달한다.
		if (lstat(argv[i], &statbuf)<0)  {
			perror("lstat");
			continue;
		}
		// statbuf에 저장되어 있는 정보 중 st_mode 값을 불러와서 mode 파악 
		if (S_ISREG(statbuf.st_mode))
			mode = "regular";
		else if (S_ISDIR(statbuf.st_mode))
			mode = "directory";
		else if (S_ISCHR(statbuf.st_mode))			
			mode = "character special";
		else if (S_ISBLK(statbuf.st_mode))
			mode = "block special";
		else if (S_ISFIFO(statbuf.st_mode))
			mode = "FIFO";	
		else if (S_ISLNK(statbuf.st_mode))
			mode = "symbolic link";
		else if (S_ISSOCK(statbuf.st_mode))
			mode = "socket";
		
		//i-nodes (파일 정보 출력)
		printf("%s\n", mode);
		printf("\tst_mode = %d\n", statbuf.st_mode);
		printf("\tst_ino = %ld\n", statbuf.st_ino);		
		printf("\tst_dev = %ld\n", statbuf.st_dev);
		printf("\tst_rdev = %ld\n", statbuf.st_rdev);
		printf("\tst_nlink = %ld\n", statbuf.st_nlink);		
		printf("\tst_uid = %d\n", statbuf.st_uid);
		printf("\tst_gid = %d\n", statbuf.st_gid);
		printf("\tst_size = %ld\n", statbuf.st_size);						
		printf("\tst_atime = %ld\n", statbuf.st_atime);
		printf("\tst_mtime = %ld\n", statbuf.st_mtime);	
		printf("\tst_ctime = %ld\n", statbuf.st_ctime);
		printf("\tst_blksize = %ld\n", statbuf.st_blksize);
		printf("\tst_blocks = %ld\n", statbuf.st_blocks);
	}
	return 0; 
}
