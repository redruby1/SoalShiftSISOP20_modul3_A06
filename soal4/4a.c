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

pthread_t tid[5];
int i, j, k;
int count = 0;

int A[5][5] = { {1,2}, {3,4}, {5,6}, {7,8} };
int B[5][5] = { {11,12,13,14,15}, {16,17,18,19,20} };

int C[5][5] = {0}; //result

void *multip(void *arg) {
	int core = count++; 
	
	for(i=(core*5)/5; i<((core+1)*5)/5; i++) {
        for(j=0; j<5; j++) {
            for(k=0; k<5; k++) {
            	C[i][j] += A[i][k] * B[k][j];
			}
        }	
    }
} 

int main() {
	key_t key = 1234;
	int *angka;
	
	int shmid = shmget(key, sizeof(int)*5*5, IPC_CREAT | 0666);
	angka = shmat(shmid, NULL, 0);
	
	for(i=0; i<5; i++) {
		pthread_create(&tid[i], NULL, multip, (void*)&i);
	}
    
	for(i=0; i<5; i++) {
		pthread_join(tid[i], NULL);
	}
	
	for(i=0; i<4; i++) {
        for(j=0; j<5; j++) {
			angka[i*5 + j] = C[i][j];
        	printf("%d\t", C[i][j]);
        	
        	sleep(1);
		}
	printf("\n");
    }
    
	sleep(20);
    
	shmdt(angka);
	shmctl(shmid, IPC_RMID, NULL);
    
	return 0; 
}
