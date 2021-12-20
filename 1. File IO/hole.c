#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

main()
{
	int fd;
	char ch = '\0';
	//file.hole이라는 파일을 생성한다
	//0400 : owner만 read 가능 
	if ((fd = creat("file.hole", 0400)) < 0)  {
		perror("creat");
		exit(1);
	}
	//lseek를 통해파일의 앞 부분에서  1023만큼 떨어진 곳에 파일 포인터가 이동한다.
	if (lseek(fd, 1023, SEEK_SET) < 0)  {
		perror("lseek");
		exit(1);
	}
	//\0값을 마지막에 쓴다
	write(fd, &ch, 1);
	close(fd);
}
