#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define bool short

char* seedname="/dev/random";  // SECURE, but SLOOOW
// char* seedname="/dev/urandom"; // FAST, but INSECURE
char* cyphername="seed.key";   // To compensate for insecurity

void create(char* filename,size_t num_bytes)
{
	FILE* file=fopen(filename,"wb");
	FILE* seed=fopen(seedname,"rb");
	FILE* cypher=fopen(cyphername,"rb");
//			if(!cypher)printf("CANT OPEN %s\n",cyphername);
	long i;
	int c=fgetc(seed);
	int s=fgetc(seed);
	srand(time(NULL));// seed
	for (i = 0; i < num_bytes; i++){
		s=fgetc(seed);
		if(cypher){
			c= fgetc(cypher);
			if(i%1337==0)
				srand(c^s);// re-seed
		}
		if(file)
					fprintf(file,"%c",rand()^c^s);
		else
					printf("%c",rand()^c^s);
	}
}

long size(FILE* fp){
	fseek(fp, 0L, SEEK_END);
	long sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return sz;
}

long min(long a,long b){
			return		a<b?a:b;
}

void usage(){
			printf("USAGE:\n random file\n");
}

bool eq(char* a,char* b){
	return	strstr(a,b)!=0;
}

int main(int cc,char** cv){
	cc--;
	if(cc>1)cyphername=cv[2];
	if(cc>=1){
		char* f=cv[1];
		create(f,0x10000000);
	}else{
		create(0,0x10000000);
//						usage();
	}
}
