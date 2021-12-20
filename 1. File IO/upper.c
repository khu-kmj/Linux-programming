#include <stdio.h>
#include <stdlib.h>
#define MAX_BUF 1024

main(int argc, char *argv[]){
	FILE *fp;
	char buf[MAX_BUF];
	char ch;
	//argc가 2가 아니면 종료(argv[0] argv[1] 존재해야 한다)
	if(argc!=2){
		fprintf(stderr, "Usage: %s source\n",argv[0]);
		exit(1);
	}

	// read 옵션을 사용하여 open한다.
	if((fp=fopen(argv[1],"r"))==NULL){
		perror("fopen");
		exit(1);
	}

	//while문과 fgetc를 사용하여 한글자씩 읽는다.
	//아스키코드를 이용해서 소문자를  대문자로 출력했다.
	while((ch=fgetc(fp))!=EOF){
		int a=ch;
		if (a>=97 && a<=122) printf("%c",a-32);
		else printf("%c",ch);
	}//반복문 종료

	fclose(fp);
}
