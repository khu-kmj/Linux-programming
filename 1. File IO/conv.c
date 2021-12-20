#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record.h"

main(int argc, char *argv[])
{
	FILE *src, *dst;
	Record	rec;
	//argc==3이 아닌 경우 종료한다(argv[0]~argv[2]가 존재해야함)
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source dest\n", argv[0]);
		exit(1);
	}
	//txt파일 read 옵션으로 open한다.
	if ((src = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	//바이너리 파일 write 옵션으로 open한다.
	if ((dst = fopen(argv[2], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	// fgets 사용하여 한 줄씩 읽는다.
	while (fgets(rec.stud, SMAX, src))  {
		//srtchr()은 특정 문자를 검색하여 가장 먼저 나타나는 곳의 위치를 찾고 포인터를 return 한다.
		//그러므로 \n를 찾고 위치를 가리키는 포인터를 리턴하여 여기에 \0값을 넣는다.
		//Kim Yong Man\n  => Kim Youn Man\0
		*strchr(rec.stud, '\n') = '\0';
		fgets(rec.num, NMAX, src);
		*strchr(rec.num, '\n') = '\0';
		fgets(rec.dept, DMAX, src);
		*strchr(rec.dept, '\n') = '\0';
		//학생,학번,전공을 dst 파일에 write한다.
		fwrite(&rec, sizeof(rec), 1, dst);
	}//while문 종료

	fclose(src);
	fclose(dst);
}
