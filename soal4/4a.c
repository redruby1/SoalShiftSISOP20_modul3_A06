#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

pthread_t tid[5];
int count = 0;
int i, j, k, hasil = 0;

int A[4][2] = { {1,2}, {3,4}, {5,6}, {7,8} };
int B[2][5] = { {11,12,13,14,15}, {16,17,18,19,20} };

int C[4][5]; //result

void *multi(void *arg) {
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
	key_t key = 5678;
	
	int shmid = shmget(key, sizeof(int)*5*4, IPC_CREAT | 0666);
	C[4][5] = shmat(shmid, NULL, 0);
	
	memset(C, 0, sizeof(C));
	
	for(i=0; i<5; i++) { 
		int *p; 
		pthread_create(&tid[i], NULL, multi, (void*)(p));
    }
    
	for(i=0; i<5; i++) {
		pthread_join(tid[i], NULL);
	}
	
	for(i=0; i<4; i++) { 
        for(j=0; j<5; j++) {
        	printf("%d\t", C[i][j]);
		}
        printf("\n");
    }
    
    shmdt(C);
//    shmctl(shmid, IPC_RMID, NULL);
    
    return 0; 
}
