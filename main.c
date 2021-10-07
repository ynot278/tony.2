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
#include <signal.h>
#include <limits.h>

#include "config.h"


struct shmseg{
	int cnt;
	int complete;
	char buf[MAX_CANON];
};

char *childProcess = "./testsim";
int *shmptr;
int shmid;
int shmkey = 0x8837;
pid_t pid;
int license;
int size;

char *readFile(const char *fileName){
	FILE *file = fopen(fileName, "r");
	int i = 0;
	int counter = 0;
	while (fscanf(file, "%d", &i) != EOF){
		counter++;
	}

	char *charArray = (char *)malloc(sizeof(int) *counter);
	fseek(file, 0 , SEEK_SET);

	counter = 0;

	char *firstPointer = charArray;
	while (fscanf(file, "%d", &i) != EOF){
		*charArray = i;
		charArray++;
		counter++;
	}

	charArray = firstPointer;
	size = counter;
	fclose(file);

	return charArray;
}

void doCommand(){
	char *args;

	args = readFile("testing.data");
	printf("%s", &args[1]);

	//I was unable to take args from testing.data.
	char *argv[2];
	argv[0] = "2";
	argv[1] = "3";

	execl(childProcess, childProcess, argv[0], argv[1], (char*)NULL);
}

void setupSharedMemory(){
	shmid = shmget(shmkey, sizeof(struct shmseg), 0644|IPC_CREAT);
	if (shmid == -1){
		perror("runsim: Error: Shared memory setup failed");
		exit(EXIT_FAILURE);
	}

	shmptr = (int *)shmat(shmid, 0, 0);
	if (shmptr == (int *) -1){
		perror("runsim: Error: Shared memory attach");
		exit(EXIT_FAILURE);
	}

}

pid_t make_Child(){
	pid = fork();
	if(pid == -1){
		perror("runsim: Error: Fork Failed");
	} else
			doCommand();
	return pid;
}

void sig_handler(int signum){
	if (pid != 0)
		kill(pid, SIGKILL);

	if (shmdt(shmptr) ==  -1)
		perror("runsim: Error: shmdt");

	shmctl(shmid, IPC_RMID, NULL);

	printf("Caught Signal: %d", signum);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {

	//signal setup for timer and ctrl + c
	signal(SIGALRM,sig_handler);
	signal(SIGINT, sig_handler);
  alarm(TIMER);

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
	make_Child();
	return 0;
}