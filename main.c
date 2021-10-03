/*
Author: Tony Pham
Date: 9/25/2021

Project 2 Concurrent Linux Processes and shared memory
*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include "config.h"

#include <sys/types.h> 
#include <sys/wait.h> 
#include <limits.h>

int shmid;
int shmkey = 0x8837;

struct shmseg{
	int cnt;
	int complete;
	char buf[MAX_CANON];
	char buf2[MAX_CANON];
	int num;
};

int main(int argc, char *argv[]) {

	//If not 2 arguments, display usage.
	if (argc != 2){
		printf("Usage: ./runsim n (n = runs up to n application processes at a time).\n");
		exit(EXIT_FAILURE);
	}

	int opt;

	//if invoked with -h or anything else, display usage.
	while ((opt = getopt(argc, argv, "h")) != -1){
		switch (opt){
			case 'h':
				printf("Usage: ./runsim n (n = runs up to n application processes at a time).\n");
				exit(EXIT_FAILURE);
				break;
			default:
				printf("Usage: ./runsim n (n = runs up to n application processes at a time).\n");
				exit(EXIT_FAILURE);
		}
	}

	//convert argv[1] to an integer.
	char *p;
	int license;
	errno = 0;
	long conv = strtol(argv[1], &p, 10);

	if (errno != 0 || *p != '\0' || conv > N_LICENSE || conv < 1){
		printf("Number of licenses should not be greater than 20.");
		exit(EXIT_FAILURE);
	} else {
		license = conv;
		printf("%d\n", license);
	}
	
	//set up shared memory
	shmid = shmget(shmkey, sizeof(struct shmseg), 0644|IPC_CREAT);
	if (shmid == -1){
		perror("runsim: Error: Shared memory");
		return 1;
	} else {
		printf("Shared memory created\n");
	}

	//attach
	struct shmseg *shmp;
	shmp = shmat(shmid, NULL, 0);
	if (shmp == (void *) -1){
		perror("runsim: Error: Shared memory attach");
		return 1;
	} else {
		printf("Shared memory attach success\n");
	}

	char *bufptr;
	int spaceavailable;
	bufptr = shmp->buf;
	spaceavailable = MAX_CANON;

	fgets(bufptr, spaceavailable, stdin);
	printf("Read: %s\n", bufptr);

	pid_t cpid, w;
	int status;

	cpid = fork();
	if (cpid == -1){
		perror("runsim: Error: Fork");
		exit(EXIT_FAILURE);
	} 
	if (cpid == 0){
		printf("Child PID is %ld\n", (long)getpid());
	} else {
			do {
				w = waitpid(cpid, &status, WUNTRACED | WCONTINUED);
				if (w == -1){
					perror("Waitpid");
					exit(EXIT_FAILURE);
				}
				if (WIFEXITED(status)){
					printf("exited, status = %d\n", WEXITSTATUS(status));
				} else if (WIFSIGNALED(status)) {
					printf("killed by signal %d\n", WTERMSIG(status));
				} else if (WIFSTOPPED(status)){
					printf("stopped by signal %d\n", WSTOPSIG(status));
				} else if (WIFCONTINUED(status)){
					printf("continued\n");
				}
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			exit(EXIT_SUCCESS);
	}

	//detach
	if (shmdt(shmp) ==  -1){
		perror("runsim: Error: shmdt");
		return 1;
	}
	//deallocate memory?
	//shmctl(shmid, IPC_RMID, NULL);
}