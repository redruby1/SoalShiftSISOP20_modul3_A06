#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

pthread_t tid[20];
int i, j, k, hasil;

int E[5][5], D[5][5];

int jumlah(int a) {
	if(a == 0) return 0; 
    return (a+jumlah(a - 1));
}

void *sum(void *arg) {	
	for(i=0; i<4; i++) {
		for(j=0; j<5; j++) {
			D[i][j] = jumlah(E[i][j]);
		}
	}
	
	pthread_exit(NULL);
}

int main() {
	key_t key = 1234;
	int *angka;
	
	int shmid = shmget(key, sizeof(int)*5*5, IPC_CREAT | 0666);
	angka = shmat(shmid, NULL, 0);
	
	for(i=0; i<4; i++) {
		for(j=0; j<5; j++) {
			E[i][j] = angka[i*5 + j];
			printf("%d\t",E[i][j]);
			
			sleep(1);
		}
		printf("\n");
	}
	
	sleep(10);
	
	for(i=0; i<20; i++) {
		pthread_create(&tid[i], NULL, sum, (void*)&i);
    }
    
    for(i=0; i<20; i++) {
		pthread_join(tid[i], NULL);
	}
    
    printf("++++++++\n");
    for(i=0; i<4; i++) { 
        for(j=0; j<5; j++) {
        	printf("%d\t", D[i][j]);
		}
        printf("\n");
    }
	
	shmdt(angka);
    shmctl(shmid, IPC_RMID, NULL);
}
