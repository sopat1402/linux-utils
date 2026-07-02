#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

struct parameters{
	unsigned int recursive:1;
	unsigned int force:1;
	unsigned int interactive:1;
};

int check_copy(char *src,char *dst,struct parameters *params){
	struct stat st;
	if (params->interactive && stat(dst, &st) == 0) {
		printf("overwrite ? [y/N] ");
		int c = getchar();
		int yes=(c=='y'||c=='Y')?1:0;
		while (c != '\n' && c != EOF){
			c = getchar();
		}
		return yes;
	}
	return 1;
}

void copy(FILE *src,FILE *dst,struct parameters *params){
	size_t capacity=4096;
	char buff[4096];
	size_t bytes_read=0;
	while ((bytes_read=fread(buff,1,capacity,src))>0){
		size_t bytes_written=fwrite(buff,1,bytes_read,dst);
		if (bytes_written!=bytes_read){
			perror("fwrite");
			exit(EXIT_FAILURE);
		}
	}
}

void make_path(char destination[],char *dst_dir,char *src_path){
	int last_ind=strlen(src_path)-1;
	int curr=last_ind-1;
	while (curr>=0 && src_path[curr]!='/'){
		curr--;
	}
	char filename[4096];
	for (int i=curr+1;i<=last_ind;i++){
		filename[i-curr-1]=src_path[i];
	}
	filename[last_ind-curr]='\0';
	snprintf(destination,4096,"%s/%s",dst_dir,filename);	
}

void copy_recursive(char *src_dir,char *dst_dir,struct parameters *params){
	DIR *sdir;
	struct dirent *entry;
	sdir=opendir(src_dir);
	if (!sdir){
		perror("opendir");
		exit(EXIT_FAILURE);
	}
	char newpath[4096];
	while ((entry=readdir(sdir))!=NULL){
		if (strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0){
			continue;
		}
		snprintf(newpath,4096,"%s/%s",src_dir,entry->d_name);
		struct stat st;
		if(stat(newpath,&st)!=0){
			perror("stat");
			exit(EXIT_FAILURE);
		}
		if (S_ISDIR(st.st_mode)){
			char destination[4096];
			snprintf(destination,4096,"%s/%s",dst_dir,entry->d_name);
			if (mkdir(destination,0755)!=0){
				perror("mkdir");
				exit(EXIT_FAILURE);
			}
			copy_recursive(newpath,destination,params);
		}else{
			FILE *src,*dst;
			src=fopen(newpath,"rb");
			if (!src){
				perror("fopen");
				exit(EXIT_FAILURE);
			}
			char destination[4096];
			snprintf(destination,4096,"%s/%s",dst_dir,entry->d_name);
			if (check_copy(newpath,destination,params)==0){
				fclose(src);
				return ;
			}
			dst=fopen(destination,"wb");
			if (!dst){
				perror("fopen");
				exit(EXIT_FAILURE);
			}
			copy(src,dst,params);
			fclose(dst);
			fclose(src);
		}
	}
	closedir(sdir);
}

int main(int argc, char *argv[]){
	if (argc<3){
		printf("usage: cp [OPTIONS] [SOURCE(S)] [DESTINATION]\n");
		return 1;
	}
	int opt;
	struct parameters params={0};
	while ((opt=getopt(argc,argv,"rfi"))!=-1){
		switch (opt){
			case 'r':
				params.recursive=1;
				break;
			case 'f':
				params.force=1;
				break;
			case 'i':
				params.interactive=1;
				break;
		}
	}
	FILE *src,*dst;
	//Multiple files
	if (argc-optind>2){
		struct stat dt;
		char *dst_dir=argv[argc-1];
		if (stat(dst_dir,&dt)!=0){
			perror("stat");
			exit(EXIT_FAILURE);
		};
		if (!S_ISDIR(dt.st_mode)){
			printf("Destination is not a directory\n");
			exit(EXIT_FAILURE);
		}
		for (int i=optind;i<argc-1;i++){
			struct stat st;
			if (stat(argv[i],&st)!=0){
				perror("stat");
				exit(EXIT_FAILURE);
			}
			if (S_ISDIR(st.st_mode)){
				if (params.recursive){
					char destination[4096];
					make_path(destination,dst_dir,argv[i]);
					if (mkdir(destination,0755)!=0){
						perror("mkdir");
						exit(EXIT_FAILURE);
					}
					copy_recursive(argv[i],destination,&params);
				}
				else{
					printf("error: Recursive mode not enabled\n");
					exit(EXIT_FAILURE);
				}
			}
			else{
				src=fopen(argv[i],"rb");
				if (!src){
					perror("fopen");
					exit(EXIT_FAILURE);
				}
				char destination[4096];
				make_path(destination,dst_dir,argv[i]);
				if (check_copy(argv[i],destination,&params)==0){
					fclose(src);
					continue;
				}
				dst=fopen(destination,"wb");
				if (!dst){
					perror("fopen");
					exit(EXIT_FAILURE);
				}
				copy(src,dst,&params);
				fclose(dst);
				fclose(src);
			}
		}
	}
	//Single file
	else{
		struct stat st;
		if (stat(argv[optind],&st)!=0){
			perror("stat");
			exit(EXIT_FAILURE);
		}
		if (S_ISDIR(st.st_mode)){
			if (params.recursive){
				struct stat dt;
				if (stat(argv[optind+1],&dt)!=0){
					perror("stat");
					exit(EXIT_FAILURE);
				}
				if (S_ISDIR(dt.st_mode)){
					char destination[4096];
					make_path(destination,argv[optind+1],argv[optind]);
					if (mkdir(destination,0755)!=0){
						perror("mkdir");
						exit(EXIT_FAILURE);
					}
					copy_recursive(argv[optind],destination,&params);
					exit(EXIT_SUCCESS);
				}
				else{
					perror("stat");
					exit(EXIT_FAILURE);
				}
			}
			else{
				printf("error: Recursive mode not enabled\n");
				exit(EXIT_FAILURE);
			}
		}
		else{
			src=fopen(argv[optind],"rb");
			if (check_copy(argv[optind],argv[optind+1],&params)==0){
				return 0;
			}
			dst=fopen(argv[optind+1],"wb");
			if (!src || !dst){
				perror("fopen");
				exit(EXIT_FAILURE);
			}
			copy(src,dst,&params);
			fclose(dst);
			fclose(src);
		}
	}
	return 0;
}
