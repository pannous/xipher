#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

long offset_token=1337;// optional offset ('password token') shareable through 3rd channel

short nr_offsets=10;

long size(FILE* fp){
	fseek(fp, 0L, SEEK_END);
	long sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return sz;
}

int* generate_random_offsets(){
	srand(time(NULL));// seed
	rand();rand();rand();rand();// skip first
	int* offsets=(int*)malloc(nr_offsets*sizeof(int));
	for (int i=0; i < nr_offsets; i++)
	{
		unsigned int n=rand();// todo: get real good random number here!
		offsets[i]=n;
		printf("%c", (n >> 24) & 0xFF);
		printf("%c", (n >> 16) & 0xFF);
		printf("%c", (n >> 8) & 0xFF);
		printf("%c", n & 0xFF);
		// fprintf(stderr,"%d\n",n);
	}
	return offsets;
}

int encrypt(char* filename,char* cyphername){
	FILE* file=fopen(filename,"rb");
	FILE* cypher=fopen(cyphername,"rb");
	if(!file)
		return fprintf(stderr,"CANT OPEN %s\n",filename);
	if(!cypher)
		return fprintf(stderr,"CANT OPEN %s\n",cyphername);
	size_t num_bytes=size(file);
	size_t num_cypher_bytes=size(cypher);
	if(num_cypher_bytes<100000)// or num_bytes
		return fprintf(stderr,"This encryption scheme needs big cypher key files!\n create one with the ./random command.");
	
	int* offsets=generate_random_offsets();// and print as header!
	char* cypher_bytes=malloc(num_cypher_bytes*sizeof(char));
	for (long i=0; i < num_cypher_bytes; i++)
	{
		char c= (char)fgetc(cypher);
		cypher_bytes[i]=c;
		// printf("%c",c);
	}
	for (long i=0; i < num_bytes; i++)
	{
		char f=(char)fgetc(file);
		for(int o=0;o<nr_offsets;o++){
			unsigned int offset=offsets[o]%num_cypher_bytes;
			char c=cypher_bytes[(offset+i+offset_token)%num_cypher_bytes];
			f=f^c;
		}
		printf("%c",f);
	}
	return num_bytes;
}

void usage(){
	fprintf(stderr,"USAGE:\n encrypt <target_file> <xor_cypher_file> [<optional_offset>]\n");
}

//srand ((unsigned int) time (NULL));
int main(int cc,char** cv){
	cc--;
	if(cc==2||cc==3){
		char* file=cv[1];
		char* key=cv[2];
      if(cc==3)offset_token=atoi(cv[3]);
		encrypt(file,key);
	}else{
		usage();
	}
}
