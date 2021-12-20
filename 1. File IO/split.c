#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define	MAX_BUF	1024

long filesize(FILE *fp)
{
	long cur, size;

	//현재 파일 포인터의 위치를 저장
	cur = ftell(fp);
	
	//피일 포인터를 가장 파일의 끝으로 보낸다.
	fseek(fp, 0L, SEEK_END);

	//가장 끝으로 포인터가 갔기 때문에 결국은 파일의 크기를 size에 저장한다.
	size = ftell(fp);

	//다시 파일 포인터의 위치를 처음으로 이동
	fseek(fp, cur, SEEK_SET);
	return(size);
}

main(int argc, char *argv[])
{
	FILE *src, *dst1, *dst2;
	char buf[MAX_BUF];
	int count, size;
	//aggc==4가 아니면 종료(arg[0]~arg[3]이 존재해야한다)
	if (argc != 4)  {
		fprintf(stderr, "Usage: %s source dest1 dest2\n", argv[0]);
		exit(1);
	}
	//바이너리 파일 read 옵션을 사용하여 open
	if ((src = fopen(argv[1], "rb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	//바이너리 파일 write 옵션을 사용하여 write
	if ((dst1 = fopen(argv[2], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	if ((dst2 = fopen(argv[3], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	//size는 src파일 사이즈의 반으로 지정한다.
	size = filesize(src) / 2;

	//절반의 내용을 dst1파일에 쓴다.
	while (size > 0)  {
		count = (size > MAX_BUF) ?  MAX_BUF : size;
		fread(buf, 1, count, src); 
		fwrite(buf,1,count,dst1);
		size -=count;											    }	
	//나머지 절반의 내용을 dst2파일에 쓴다.
	while ((count = fread(buf, 1, MAX_BUF, src)) > 0)  {
		fwrite(buf,1,count,dst2);		
	}
	fclose(src);
	fclose(dst1);	
	fclose(dst2);
}
