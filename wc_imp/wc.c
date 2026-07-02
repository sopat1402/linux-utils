#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

struct parameters{
	unsigned int show_bytes:1;
	unsigned int show_words:1;
	unsigned int show_lines:1;
};

void count_file(FILE *fp, struct parameters *params){
	int word_count=0,line_count=0,byte_count=0;
	int c;
	int is_word=0;
	while ((c=fgetc(fp))!=EOF){
		byte_count++;
		if (isblank(c)||c=='\r'||c=='\v'||c=='\f'){
			if (is_word){
				is_word=0;
			}else{
				continue;
			}
		}
		else if (c=='\n'){
			is_word=0;
			line_count++;
		}
		else if (!is_word){
			is_word=1;
			word_count++;
		}
	}
	fclose(fp);
	if (!params->show_bytes && !params->show_lines && !params->show_words){
		printf("%d %d %d ",line_count,word_count,byte_count);
	}
	else{
		params->show_lines?printf("%d ",line_count):printf("");
		params->show_words?printf("%d ",word_count):printf("");
		params->show_bytes?printf("%d ",byte_count):printf("");
	}
}

int main(int argc, char *argv[]){
	int opt;
	struct parameters params={0};
	while ((opt=getopt(argc,argv,"cwl"))!=-1){
		switch(opt){
			case 'c':
				params.show_bytes=1;
				break;
			case 'w':
				params.show_words=1;
				break;
			case 'l':
				params.show_lines=1;
				break;
		}
	}
	FILE *fp;
	if (argc==1){
		fp=stdin;
		count_file(fp,&params);
		printf("-\n");
		return 0;
	}
	for (int i=optind;i<argc;i++){
		fp=fopen(argv[i],"r");
		if (fp==NULL){
			perror("fopen");
			return 1;
		}
		count_file(fp,&params);
		printf("%s\n",argv[i]);
	}
	return 0;
}
