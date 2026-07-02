#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include "archive.h"
#include <string.h>

void traversal(char *path,FILE *tar){
	struct stat st;
	if (stat(path,&st)!=0){
		perror("stat");
		exit(EXIT_FAILURE);
	}
	archive_file(path,tar);
	if (S_ISDIR(st.st_mode)){
		DIR *dir=opendir(path);
		if (!dir){
			perror("opendir");
			exit(EXIT_FAILURE);
		}
		struct dirent *entry;
		while ((entry=readdir(dir))!=NULL){
			if (strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0){
				continue;
			}
			char newpath[4096];
			int len=snprintf(newpath,4096,"%s/%s",path,entry->d_name);
			if (len<0 || len>=(int)sizeof(newpath)){
				printf("Too long a name\n");
				closedir(dir);
				exit(EXIT_FAILURE);
			}
			traversal(newpath,tar);
		}
		closedir(dir);
	}
}
