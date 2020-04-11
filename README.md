# SoalShiftSISOP20_modul3_A06

## Soal 1

## Soal 2

## Soal 3

## Soal 4
### == 4a ==
1. **Buatlah program C dengan nama "4a.c", yang berisi program untuk melakukan perkalian matriks. Ukuran matriks pertama adalah 4x2, dan matriks kedua 2x5. Isi dari matriks didefinisikan di dalam kodingan. Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).**
2. **Tampilkan matriks hasil perkalian tadi ke layar.**

```
int A[5][5] = { {1,2}, {3,4}, {5,6}, {7,8} };
int B[5][5] = { {11,12,13,14,15}, {16,17,18,19,20} };

int C[5][5] = {0}; //result
```
- inisialisasi matriks A, B, dan C
- matriks C untuk menyimpan hasil perkalian

*Main :*
```
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
		}
        printf("\n");
    }
    
    sleep(20);
    
    shmdt(angka);
    shmctl(shmid, IPC_RMID, NULL);
    
    return 0; 
}
```
- ``` key_t key = 1234; ``` inisialisasi key
- ``` int *angka; ``` variabel yang akan menyimpan data untuk shared memory
- ``` pthread_create(&tid[i], NULL, multip, (void*)&i); ``` membuat thread
- ``` pthread_join(tid[i], NULL); ``` join thread
- ``` angka[i*5 + j] = C[i][j]; ``` memasukkan nilai array ke variabel shared memory
- ``` sleep(1); ``` dan ``` sleep(20) ``` digunakan agar nilai masih tersimpan dalam waktu tertentu bahkan ketika perhitungan sudah selesai dilakukan
- untuk mencetak hasil ke layar
	```
	for(i=0; i<4; i++) { 
	        for(j=0; j<5; j++) {
	        	angka[i*5 + j] = C[i][j];
	        	printf("%d\t", C[i][j]);
        	
	        	sleep(1);
		}
        printf("\n");
	} 
	```

*Fungsi multi (untuk mengalikan 2 matriks) :*
```
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
```
- ``` for(i=(core*5)/5; i<((core+1)*5)/5; i++) ``` untuk membagi pekerjaan ke masing-masing thread
- ``` C[i][j] += A[i][k] * B[k][j]; ``` rumus perkalian matriks

### == 4b ==

1. **Buatlah program C kedua dengan nama "4b.c". Program ini akan mengambil variabel hasil perkalian matriks dari program "4a.c" (program sebelumnya), dan tampilkan hasil matriks tersebut ke layar. (Catatan!: gunakan shared memory)**
2. **Setelah ditampilkan, berikutnya untuk setiap angka dari matriks tersebut, carilah nilai sigmanya, dan tampilkan hasilnya ke layar dengan format seperti matriks.**

```
int E[5][5], D[5][5];
```
- inisialisasi matriks D dan E
- matriks E menyimpan hasil dari 4a
- matriks D menyimpan hasil perhitungan sigma

*Main :*
```
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
```
- ``` key_t key = 1234; ``` inisialisasi key
- ``` int *angka; ``` variabel yang akan menyimpan data untuk shared memory
- ``` pthread_create(&tid[i], NULL, multip, (void*)&i); ``` membuat thread
- ``` pthread_join(tid[i], NULL); ``` join thread
- ``` E[i][j] = angka[i*5 + j]; ``` memasukkan nilai dari shared memory ke variabel
- ``` sleep(1); ``` dan ``` sleep(10) ``` digunakan agar nilai masih tersimpan dalam waktu tertentu bahkan ketika perhitungan sudah selesai dilakukan
- mencetak matriks dari *4a*
	```
	for(i=0; i<4; i++) {
		for(j=0; j<5; j++) {
			E[i][j] = angka[i*5 + j];
			printf("%d\t",E[i][j]);
			
			sleep(1);
		}
		printf("\n");
	}
	```
*Fungsi sum dan jumlah (untuk menghitung sigma) :*
```
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
```
- fungsi *jumlah* untuk menghitung sigma
- fungsi *sum* untuk memasukkan hasil perhitungan sigma ke dalam matriks

### == 4c ==

1. **Buatlah program C ketiga dengan nama "4c.c". Program ini tidak memiliki hubungan terhadap program yang lalu.**
2. **Pada program ini, Norland diminta mengetahui jumlah file dan folder di direktori saat ini dengan command "ls | wc -l". Karena sudah belajar IPC, Norland mengerjakannya dengan semangat. (Catatan! : Harus menggunakan IPC Pipes)**

```
int main() {
	int pipefd[2];
	pipe(pipefd);
	
	pid_t anak;
	anak = fork();
	
	if(anak == 0) {
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		close(pipefd[1]);
		
		char *argv[] = {"wc", "-l", NULL};
	    	execv("/usr/bin/wc", argv);
	} 
	else {
		dup2(pipefd[1], 1);
		close(pipefd[0]);
		close(pipefd[1]);
		
		char *argv[] = {"ls", ".", NULL};
	    	execv("/bin/ls", argv);
	}
	exit(EXIT_FAILURE);
}
```
- menggunakan fork
- pada child process menjalankan ``` wc -l ```
- pada parent process menjalankan ``` ls . ```
- ``` close(pipefd[0]); ``` untuk read data pada pipe
- ``` close(pipefd[1]); ``` untuk write data ke pipe
