#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[])
{
	int fdin, fdout;
	char *src, *dst;
	struct stat statbuf;

	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	
	// argv[1]의 이름을 가진 파일 open
	if ((fdin = open(argv[1], O_RDONLY)) < 0)  {
		perror("open");
		exit(1);
	}
	// argv[2]의 이름을 가진 파일 open
	if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0)  {
		perror("open");
		exit(1);
	}
	// stat로 파일 사이즈 확인
	if (fstat(fdin, &statbuf) < 0)  {
		perror("fstat");
		exit(1);
	}
	// write의 경우 메모리 매핑 할 것이 없으므로 가상으로 만드는 code
	if (lseek(fdout, statbuf.st_size-1, SEEK_SET) < 0)  {
		perror("lseek");
		exit(1);
	}
	write(fdout, "", 1);
	// code 끝
	
	// 모든 내용 read  
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) 
		== (caddr_t) -1)  {
		perror("mmap");
		exit(1);
	}
	// 모든 내용 write
	if ((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0)) 
		== (caddr_t) -1)  {
		perror("mmap");
		exit(1);
	}
	// src -> dst로 st_size만큼 복사
	memcpy(dst, src, statbuf.st_size);

	close(fdin);
	close(fdout);
	return 0;
}
