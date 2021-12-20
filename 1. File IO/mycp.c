#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define	MAX_BUF	1024

main(int argc, char *argv[])
{
	int fd1, fd2, count;
	char buf[MAX_BUF];
	// argc카운터가 3이 아니면 프로그램을 종료한다. 
	// ./mycp mycp.c yourcp.c와 같이 argv[0], argv[1], argv[2]까지 존재해야 실행된다는 의미이다.
	if (argc != 3)  {
		printf("Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	// 소스파일을 오픈하는데 READ만 하겠다.
	if ((fd1 = open(argv[1], O_RDONLY)) < 0)  {
		perror("open");
		exit(1);
	}
	// 소스파일을 오픈하는데 WRITE만 하겠다. 그리고 새 파일을 생성하고 기존에 파일이 존재하면 버리고 새로만든다.
	//0644 => read write read read
	if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)  {
		perror("open");
		exit(1);
	}
	//읽은 크기만큼 반복하여 쓴다.(count를 사용해 짜투리 공간까지 write한다.
	while (count=read(fd1, buf, MAX_BUF)>0)  {
		write(fd2, buf, count);
	}
	close(fd1);
	close(fd2);
}
