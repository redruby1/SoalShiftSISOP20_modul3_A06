#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

void *ekstensi_f(void *file)
{
	char *ekstensi;
	ekstensi = strchr(file, '.');
	if (ekstensi != NULL)
	{
		ekstensi++;
	}
}

int main(int argc, char *argv[])
{
	if(!strcmp(argv[1], "-f"))
	{
			char *file[10000];
			int i, j;
			int k;
			pthread_t thread[100];
		   for(i=2; i<argc+1; i++)
		   {
		   		file[i-1] = argv[i];
		   }
		   
			for(j=1; j<argc+1; j++)
		   {
		   		pthread_create(&thread[j], NULL, ekstensi_f, (void *) file[j]);
		   }
		   
		   for(k=1; k<argc+1; k++)
		   {
		   		pthread_join(thread[k], NULL);
		   }
	}
	
	else if(!strcmp(argv[1], "-d"))
	{
		
	}
	
	else if(!strcmp(argv[1], "*"))
	{
		
	}
	
	else
	{
		printf("Argumen yang dimasukkan salah\n");
	}
}
