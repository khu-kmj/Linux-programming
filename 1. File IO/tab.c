#include <stdio.h>

//FALSE=0, TRUE=1 (C언어는 대문자 boolean 타입이 없기 때문에 열거형으로 지정)
enum { FALSE, TRUE };

void conv(char *fname)
{
	FILE *src, *dst;
	char fdest[40];
	int ch, first;

	//r(read)t(txt) 옵션으로 파일을 연다. NULL인 경우 종료
	if ((src = fopen(fname, "rt")) == NULL)  {
		perror("fopen");
		return;
	}
	
	//fdest에 fname을 복사하고 마지막 끝에는 .t를 붙여서 fname.t라는 파일 이름을 가지게 된다.
	strcpy(fdest, fname);
	strcat(fdest, ".t");
	
	//w(write)t(txt) 옵션으로 파일을 연다. NULL인 경우 종료
	if ((dst = fopen(fdest, "wt")) == NULL)  {
		perror("fopen");
		return;
	}

	//Line마다 TAB간격을 지정하는 것으로 젤 처음 시작하는 Line이라고 알려주기 위해 TRUE를 사용한다.
	first = TRUE;

	//한글자씩 읽고, 파일의 끝이 아닐때 까지 반복한다.
	while ((ch = fgetc(src)) != EOF)  {
		//특정 LINE의 가장 첫부분이고, ch가 '\t'인 경우 ' '을 4번 써준다 
		//즉, 기존의 파일의 탭 크기와 상관없이  ' '을 4번 써서 tab을 4로 지정하는 것과 동일하다.
		if (first && ch == '\t')  {	
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
		}
		//특정 LINE의 처음이 아닐 때
		else  {
			//기존 파일의 내용을 그대로 쓴다.
			fputc(ch, dst);
			
			//처음이 아니기 때문에 FALSE 값을 사용한다.
			if (first) first = FALSE;

			//만약 ch가 줄바꿈 문자라면, 이는 다음 라인으로 진행한다는 말과 동일하기 때문에
			//first에 TRUE값을 사용하여 다시 문장의 첫 부분이라는 것을 알린다.
			if (ch == '\n') first = TRUE;
		}
	}//반복문 종료


	fclose(src);
	fclose(dst);
}

main(int argc, char *argv[])
{
	//입력 파일의 개수가 상관이 없다.
	while (--argc)  {
		conv(argv[argc]);
	}
}
