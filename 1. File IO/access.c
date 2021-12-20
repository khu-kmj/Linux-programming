#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "record.h"

void print_record(Record *rp)
{		
	puts(rp->stud);
	puts(rp->num);
	puts(rp->dept);
}

main(int argc, char *argv[])
{
	FILE	*fp;
	Record	rec;
	
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(1);
	}

	if ((fp = fopen(argv[1], "r+b")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	printf("-----Ordered Record List-----\n");

	//파일을 읽고 print_record를 통해 출력한다.
	while (fread(&rec, sizeof(rec),1, fp))  {
		print_record(&rec);									
	}

	//파일의 현재 포인터를 파일의 시작점(0)으로 이동하는 함수
	rewind(fp);
	getchar();

	printf("-----Shuffled Record List (3, 6, 2, 4, 1, 5)-----\n");
	
	//현재 포인터를 2로 이동하여 읽는다.
	fseek(fp, sizeof(rec)*2L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	//현재 포인터를 5로 이동하여 읽는다.
	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	//현재 포인터를 1로 이동하여 읽는다.
	fseek(fp, sizeof(rec)*1L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);
	
	//현재 포인터를 3으로 이동하여 읽는다.
	fseek(fp, sizeof(rec)*3L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	//현재 포인터를 0으로 이동하여 읽는다.
	fseek(fp, sizeof(rec)*0L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);
	
	//현재 포인터를 4로 이동하여 읽는다.
	fseek(fp, sizeof(rec)*4L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);
	getchar();

	printf("-----Updated Record List-----\n");
	
	//현재 포인터를 2로 이동하고 전공을 Movie로 변경한다.
	fseek(fp, sizeof(rec)*2L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	strcpy(rec.dept, "Movie");

	//현재 포인터를 2로 이동 후 바뀐 정보를 write한다.
	fseek(fp, sizeof(rec)*2L, SEEK_SET);
	fwrite(&rec, sizeof(rec), 1, fp);
	
	//현재 포인터를 5로 이동하고 전공을 Baseball로 변경한다.
	fseek(fp, sizeof(rec)*5L, SEEK_SET);					
	fread(&rec, sizeof(rec), 1, fp);	
	strcpy(rec.dept, "Basebal1");			
	
	//현재 포인터를 5로 이동 후 바뀐 정보를 write한다.
	fseek(fp, sizeof(rec)*5L, SEEK_SET);								
	fwrite(&rec, sizeof(rec), 1, fp);
	
	rewind(fp);
	while (fread(&rec, sizeof(rec), 1, fp))  {
		print_record(&rec);
	}
	fclose(fp);
}
