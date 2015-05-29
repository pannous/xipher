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

int* load_offsets(FILE* file){
	int* offsets=(int*)malloc(nr_offsets*sizeof(int));
	for (int i=0; i < nr_offsets; i++)
	{
		unsigned int n=fgetc(file);
		n=(n<<8)+fgetc(file);
		n=(n<<8)+fgetc(file);
		n=(n<<8)+fgetc(file);
		offsets[i]=n;
		// fprintf(stderr,"%d\n",n);
	}
	return offsets;
}

int decrypt(char* filename,char* cyphername){
	FILE* file=fopen(filename,"rb");
	FILE* cypher=fopen(cyphername,"rb");
	if(!file)
		return fprintf(stderr,"CANT OPEN %s\n",filename);
	if(!cypher)
		return fprintf(stderr,"CANT OPEN %s\n",cyphername);
	
	size_t num_bytes=size(file);
	size_t num_cypher_bytes=size(cypher);
	
	int* offsets=load_offsets(file);// skips header!!
	if(num_cypher_bytes<100000)// or num_bytes
		return fprintf(stderr,"This encryption scheme needs big cypher key files!\n create one with the ./random command.");
	
	char* cypher_bytes=malloc(num_cypher_bytes*sizeof(char));
	for (long i=0; i < num_cypher_bytes; i++)
	{
		char c= (char)fgetc(cypher);
		cypher_bytes[i]=c;
		// printf("%c",c);
	}
	for (long i=0; i < num_bytes - nr_offsets*sizeof(int); i++)
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
	fprintf(stderr,"USAGE:\n decrypt <encrypted_file> <xor_cypher_file> [<optional_offset>]\n");
}

//srand ((unsigned int) time (NULL));
int main(int cc,char** cv){
	cc--;
	if(cc==2||cc==3){
		char* file=cv[1];
		char* key=cv[2];
      if(cc==3)offset_token=atoll(cv[3]);
		decrypt(file,key);
	}else{
		usage();
	}
}
