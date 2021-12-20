#include <stdio.h>
#include <stdlib.h>
main(int argc, char *argv[])
{
	FILE *src, *dst;
	int ch;
	// argc!=3이면 프로그램 종료(argv[0], argv[1], argv[2] 존재해야함)
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	// txt파일 read 옵션을 사용하여 argv[1]에 입력된 파일을 연다
	if ((src = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	//argv[2]에 입력된 파일을 txt파일 write 옵션을 사용하여 연다.
	if ((dst = fopen(argv[2], "wt")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	//fgetc => 한글자씩 입력받는 것이다.
	//EOF => 파일의 끝인가?
	//그러므로 파일의 끝이 아닐 때까지 while을 사용하여 반복하면서 한 글자씩 dst파일에 쓴다.
	while ((ch=fgetc(src))!=EOF)  {
		fputc(ch, dst);
	}
	fclose(src);
	fclose(dst);
}
