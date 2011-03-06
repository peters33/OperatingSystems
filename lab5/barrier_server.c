#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "restart.h"
#include "restart.c"
#define FIFO_PERMS (S_IRWXU | S_IWGRP| S_IWOTH)

int main (int argc, char *argv[]) {
	int requestfd;
	int releasefd;
	char* requestPath;
	char* releasePath;
	int i;
	int bytesRead;
	
	if (argc != 3) {
		fprintf(stderr, "Usage: %s [barrier name] [barrier size]\n", argv[0]);
		return 1; 
	}
	
	int barrierSize = atoi(argv[2]);
	char buffer[barrierSize];
	
	strcpy(requestPath, argv[1]);
	strcat(requestPath, ".request");
	
	strcpy(releasePath, argv[1]);
	strcat(releasePath, ".release");
	
	
	/* create a named pipe to handle incoming requests */
	if ((mkfifo(requestPath, FIFO_PERMS) == -1) && (errno != EEXIST)) {
		perror("Server failed to create a FIFO");
		return 1; 
	}
	
	/* create a named pipe to handle writing requests */
	if ((mkfifo(releasePath, FIFO_PERMS) == -1) && (errno != EEXIST)) {
		perror("Server failed to create a FIFO");
		return 1;
	}
	
	/* server loops twice */
	for (i = 0; i < 2; i++) {
	
		/* open a read/write communication endpoint to the pipe */
		if ((requestfd = open(requestPath, O_RDWR)) == -1) {
			perror("Server failed to open its FIFO");
			return 1;
		}
		
		if ((bytesRead = read(requestfd, buffer, sizeof(buffer)) == 0)
			perror("Server faile to read FIFO");
			return 1;
		}
	}	
	
	//copyfile(requestfd, STDOUT_FILENO); 
	return 1; 
}
