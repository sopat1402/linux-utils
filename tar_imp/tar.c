#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "archive.h"
#include "traversal.h"

int main(int argc, char *argv[]){
	if (argc<3){
		printf("usage: tar [TARGET NAME] [SOURCE(S)]\n");
		exit(EXIT_FAILURE);
	}
	char target[100];
	int len=snprintf(target,100,"%s",argv[1]);
	if (len>(int)sizeof(target)){
		printf("error: Size of target name has to be 100 bytes or less.\n");
		exit(EXIT_FAILURE);
	}
	FILE *tar=fopen(target,"wb");
	for (int i=2;i<argc;i++){
		char *path=argv[i];
		struct stat st;
		if (stat(path,&st)!=0){
			perror("stat");
			exit(EXIT_FAILURE);
		}
		if (S_ISDIR(st.st_mode)){
			traversal(path,tar);
		}else{
			archive_file(path,tar);
		}
	}
	write_to_end(tar);
	fclose(tar);
}
