#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

struct tar_header{
	char name[100];
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char mtime[12];
	char checksum[8];
	char typeflag;
	char linkname[100];
	char magic[6];
	char version[2];
	char uname[32];
	char gname[32];
	char devmajor[8];
	char devminor[8];
	char prefix[155];
	char padding[12];
};

static unsigned int checksum(struct tar_header *header){
	unsigned int sum=0;
	unsigned char *p=(unsigned char *)header;
	for (int i=0;i<512;i++){
		sum+=p[i];
	}
	return sum;
}

static void write_file(char *path, FILE *tar){
	char buffer[4096];
	FILE *src=fopen(path,"rb");
	if (!src){
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	size_t capacity=4096,bytes_written=0;
	size_t chunk;
	while ((chunk=fread(buffer,1,capacity,src))>0){
		bytes_written+=fwrite(buffer,1,chunk,tar);
	}
	size_t padding=(512-(bytes_written%512))%512;
	char pad[512]={0};
	fwrite(pad,1,padding,tar);
	fclose(src);
}

void write_to_end(FILE *tar){
	char zeros[512]={0};
	if (fwrite(zeros,1,512,tar)!=512 || fwrite(zeros,1,512,tar)!=512){
		perror("fwrite");
		exit(EXIT_FAILURE);
	}
}

void archive_file(char *path, FILE *tar){
	struct stat st;
	struct tar_header header={0};
	int is_dir=0;
	if (stat(path,&st)!=0){
		perror("stat");
		exit(EXIT_FAILURE);
	}
	if (S_ISDIR(st.st_mode)){
		is_dir=1;
		snprintf(header.size,sizeof(header.size),"%011lo",(unsigned long)0);
		header.typeflag='5';
	}
	else{
	snprintf(header.size,sizeof(header.size),"%011lo",(unsigned long)st.st_size);
	header.typeflag='0';
	}
	struct passwd *pw=getpwuid(st.st_uid);
	struct group *gw=getgrgid(st.st_gid);
	snprintf(header.mode,sizeof(header.mode),"%07o",st.st_mode & 0777);
	snprintf(header.uid,sizeof(header.uid),"%07o",st.st_uid);
	snprintf(header.gid,sizeof(header.gid),"%07o",st.st_gid);
	snprintf(header.mtime,sizeof(header.mtime),"%011lo",(unsigned long)st.st_mtime);
	snprintf(header.name,sizeof(header.name),"%s",path);
	header.linkname[0]='\0';
	memcpy(header.magic,"ustar\0",6);
	memcpy(header.version,"00",2);
	if (pw){
		snprintf(header.uname,sizeof(header.uname),"%s",pw->pw_name);
	}
	if (gw){
		snprintf(header.gname,sizeof(header.gname),"%s",gw->gr_name);
	}
	memset(header.checksum,' ',8);
	snprintf(header.checksum,sizeof(header.checksum),"%06o",checksum(&header));
	header.checksum[6]='\0';
	header.checksum[7]=' ';
	if (fwrite(&header,1,512,tar)!=512){
		perror("fwrite");
		exit(EXIT_FAILURE);
	}
	if (!is_dir) write_file(path,tar);
}
