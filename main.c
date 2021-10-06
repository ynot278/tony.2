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
#include <string.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <limits.h>

#include "config.h"


struct shmseg{
	int cnt;
	int complete;
	char buf[MAX_CANON];
};

struct shmseg *shmp;
int shmid;
int shmkey = 0x8837;
pid_t pidList;
int license;

void doCommand(){
	char filename[] = "testing.data";
	FILE *file = fopen (filename, "r");

	char line[MAX_CANON];

	execl("./testsim", "./testsim", "2", "5");
		

}

void setupSharedMemory(){
	shmid = shmget(shmkey, sizeof(struct shmseg), 0644|IPC_CREAT);
	if (shmid == -1){
		perror("runsim: Error: Shared memory setup failed");
		exit(EXIT_FAILURE);
	}

	
	shmp = shmat(shmid, NULL, 0);
	if (shmp == (void *) -1){
		perror("runsim: Error: Shared memory attach");
		exit(EXIT_FAILURE);
	}

}

int getEmptyProcessIndex(){
  int i = 0;
  for (i = 0; i < license; i++){
    if (pidList == 0)
      return i;
  }
  return -1;
}

void createChild(){
	int i, position;
	for (i = 0; i < license; i++){
		position = getEmptyProcessIndex();
		pidList = fork();
		if(pidList == -1){
			perror("runsim: Error: Fork Failed");
		} else
				doCommand();
	}
}


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
	errno = 0;
	long conv = strtol(argv[1], &p, 10);

	if (errno != 0 || *p != '\0' || conv > N_LICENSE || conv < 1){
		printf("Number of licenses should not be greater than 20.");
		exit(EXIT_FAILURE);
	} else
		license = conv;
	
	setupSharedMemory();
	createChild();
	
	



	//detach
	if (shmdt(shmp) ==  -1){
		perror("runsim: Error: shmdt");
		return 1;
	}
	//deallocate memory?
	shmctl(shmid, IPC_RMID, NULL);
}