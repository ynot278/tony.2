#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "config.h"

int main(int argc, char *argv[]) {
	//If not 3 arguments, display usage.
	if (argc != 3){
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

	for (int i = 0; i < repeatFactor; i++){
		
	}
	
}