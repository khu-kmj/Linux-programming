#include <stdio.h>
#include <stdlib.h>
#define	MAX_BUF	1024

main(int argc, char *argv[])
{
	FILE *src, *dst;
	char buf[MAX_BUF];
	int count;
	//mycp.c와 동일함
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	//옵션이 다르다. rb : 바이너리 파일을 read하겠다.
	if ((src = fopen(argv[1], "rb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	//wb : 바이너리 파일을 write하겠다.
	if ((dst = fopen(argv[2], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	//한 번에 읽는 단위를 1byte로 지정했다.
	//count의 경우 실제로 읽은 바이트의 수가 들어가게 된다.
	while (count=fread(buf,1,MAX_BUF,src)>0)  {
		fwrite(buf, 1, count, dst);
	}

	fclose(src);
	fclose(dst);
}
