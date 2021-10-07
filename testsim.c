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
#include <time.h>

#include "config.h"

char* getTime(){
	time_t now = time(0);
	char* dt = ctime(&now);
	return dt;
}


pid_t pid;

int main(int argc, char *argv[]) {
	int shmid;
	int *shmptr;

	int key = 0x8837;
	pid = getpid();

	struct shmid_ds shmbuffer;


	shmid = shmget(key, MAX_CANON, 0644);
	if (shmid < 0){
		perror("testsim: Error: shmget failed");
		exit(EXIT_FAILURE);
	}

	shmptr = (int *) shmat(shmid, 0, 0);

	shmctl (shmid, IPC_STAT, &shmbuffer);
	int *charArray = shmptr;

	//convert argv[1] and argv[2] to an integer.
	char *p;
	int sleepTime;
	int repeatFactor;
	errno = 0;
	
	long sleepConvert = strtol(argv[1], &p, 10);
	if (errno != 0 || *p != '\0' || sleepConvert < 1){
		printf("Sleep Time can't be lower than 1");
		exit(EXIT_FAILURE);
	} else {
		sleepTime = sleepConvert;
		printf("Sleep Time is %d\n", sleepTime);
	}
	
	long repeatConvert = strtol(argv[2], &p, 10);
	if (errno != 0 || *p != '\0' || repeatConvert < 1){
		printf("Sleep Time can't be lower than 1");
		exit(EXIT_FAILURE);
	} else {
		repeatFactor = repeatConvert;
		printf("Repeat Factor %d\n", repeatFactor);
	}

	int i;
	for (i = 0; i < repeatFactor; i++){
		char *timer = getTime();
		printf("%s", timer);
		printf("Iteration %d of %d ", (i+1), repeatFactor);
		sleep(sleepTime);
	}
	return 0;
}