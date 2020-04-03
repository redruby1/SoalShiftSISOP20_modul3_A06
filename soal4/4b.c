#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

pthread_t tid[5*2];
int count = 0;
int i, j, k, step = 0, hasil;

int C[4][5], D[4][5];

int jumlah(int a) {
	hasil = hasil + jumlah(a-1);
	return hasil;
}

void sum(void *arg) {
	int core = count++;
	
//	for(i=0; i<4; i++) {
	
	for(i=(core*5)/5; i<((core+1)*5)/5; i++) {
		for (j=0; j<5; j++) {
			D[i][j] = jumlah(C[i][j]);
		}
	}
}

int main() {
	key_t key = 5678;
	
	int C[4][5], D[4][5];
	
	int shmid = shmget(key, sizeof(int)*5*4, IPC_CREAT | 0666);
	C[4][5] = shmat(shmid, NULL, 0);
	
	int i, j;
	for(i=0; i<4; i++) {
		for(j=0; j<5; j++) {
			printf("%d\t",C[i][j]);
		}
		printf("\n");
	}
	
	for(i=0; i<5; i++) { 
		int *p; 
		pthread_create(&tid[i], NULL, sum, (void*)(p));
//		step++;
    }
    
    for(i=0; i<5*2; i++) {
		pthread_join(tid[i], NULL);
	}

//	sum(C);
    
    printf("++++++++\n");
    for(i=0; i<4; i++) { 
        for(j=0; j<5; j++) {
        	printf("%d ", D[i][j]);
		}
        printf("\n");
    }
	
	shmdt(C);
    shmctl(shmid, IPC_RMID, NULL);
}
