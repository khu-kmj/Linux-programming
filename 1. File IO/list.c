#include <stdio.h>
#define	MAX_BUF	256

main(int argc, char *argv[])
{
	FILE *fp;
	char buf[MAX_BUF];
	int line;
	//argc!=2가 아니면 종료한다(argv[0], argv[1]이 존재해야한다)
	if (argc != 2)  {
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}
	//txt파일을 read하겠다 = rt 즉 rt 옵션으로 open하겠다.
	if ((fp = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	
	line = 1;
	//fgets를 사용해서 buf에 파일의 한 라인을 읽는다. 
	//line 변수를 통해 라인 넘버를 나타낸다.
	while (fgets(buf, MAX_BUF, fp))  {
		printf("%4d: %s", line++, buf);
	}
	fclose(fp);
}
