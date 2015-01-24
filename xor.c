#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

long size(FILE* fp){
	fseek(fp, 0L, SEEK_END);
	long sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return sz;
}

long min(long a,long b){
	return a<b?a:b;
}

void apply(char* cyphername,char* filename){
	FILE* cypher=fopen(cyphername,"rb");
	if(!cypher)printf("CANT OPEN %s\n",cyphername);
	FILE* file=fopen(filename,"rb");
	if(!file)printf("CANT OPEN %s\n",filename);
	long i=0;
	size_t num_bytes=min(size(file),size(cypher));
	for (; i < num_bytes; i++)
	{
		int f= fgetc(file);
		int c= fgetc(cypher);
		printf("%c",f^c);
	}
}

void usage(){
	printf("USAGE:\n xor file0 file1\n");
}

int eq(char* a,char* b){
	return	strstr(a,b)!=0;
}

//srand ((unsigned int) time (NULL));
int main(int cc,char** cv){
	cc--;
	if(cc==2){
		char* c=cv[1];
		char* f=cv[2];
		apply(c,f);
	}else{
		usage();
	}
}
