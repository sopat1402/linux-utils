#include <stdio.h>
#include <unistd.h>

struct parameters{
	int number_lines;
	int show_ends;
	int replace_tabs;
	int line_count;
};

void print_stream(FILE *fp,struct parameters *params){
	int c;
	int at_start=1;
	while ((c=fgetc(fp))!=EOF){
		(at_start && params->number_lines)?printf("%d\t",params->line_count):printf("");
		if (c=='\n'){
			params->line_count++;
			params->show_ends?printf("$"):printf("");
			at_start=1;
			printf("\n");
			continue;
		}
		else if (c=='\t' && params->replace_tabs){
			printf("^I");
		}
		else putchar(c);
		at_start=0;
	}
	if (fp!=stdin){
		fclose(fp);
	}
}

int main(int argc, char *argv[]){
	FILE *fp;
	struct parameters params={0,.line_count=1};
	int opt;
	if (argc==1){
		fp=stdin;
		print_stream(fp,&params);
	}
	while ((opt=getopt(argc,argv,"nET"))!=-1){
		switch(opt){
			case 'n':
				params.number_lines=1;
				break;
			case 'E':
				params.show_ends=1;
				break;
			case 'T':
				params.replace_tabs=1;
				break;
		}
	}
	for (int i=optind;i<argc;i++){
		fp=fopen(argv[i],"r");
		if (fp==NULL){
			perror("fopen");
			return 1;
		}
		print_stream(fp,&params);
	}
	return 0;
}
