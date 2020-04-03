#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h>

int main() {
	int pipefd[2];
	pipe(pipefd);
	
	pid_t anak;
	anak = fork();
	
	if(anak == 0) {
		dup2(pipefd[0], 0); //read
		close(pipefd[0]);
		close(pipefd[1]);
		
		char *argv[] = {"wc", "-l", NULL};
    	execv("/usr/bin/wc", argv);
	} 
	else {
		dup2(pipefd[1], 1); //write
		close(pipefd[0]);
		close(pipefd[1]);
		
		char *argv[] = {"ls", ".", NULL};
    	execv("/bin/ls", argv);
	}
	exit(EXIT_FAILURE);
}
