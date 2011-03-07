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
	FILE* pipe = NULL;
	int requestfd;
	int releasefd;
	char reqPath[1024];
	char relPath[1024];
	int i;
	int x;
	int n = atoi(argv[2]);
	char buffer[n];
	
	if (argc != 3) {
		fprintf(stderr, "Usage: %s [barrier name] [barrier size]\n", argv[0]);
		return 1; 
	}
	
	strcpy(reqPath, argv[1]);
	strcat(reqPath, ".request");
	
	strcpy(relPath, argv[1]);
	strcat(relPath, ".release");
	
	// create a named pipe to handle incoming requests
	if ((mkfifo(reqPath, FIFO_PERMS) == -1) && (errno != EEXIST)) {
		perror("Server failed to create a FIFO");
		return 1; 
	}
	
	// create a named pipe to handle writing requests
	if ((mkfifo(relPath, FIFO_PERMS) == -1) && (errno != EEXIST)) {
		perror("Server failed to create a FIFO");
		return 1;
	}
	
	// server loops twice 
	for (i = 0; i < 2; i++) {
		fprintf(stderr, "Iteration #%d\n", i + 1);
	
		// open a read/write communication endpoint to the pipe
		fprintf(stderr, "Server opening %s\n", reqPath); 
		if (!(pipe = fopen(reqPath, "r"))) {
			perror("Server failed to open its FIFO");
			return 1;
		}
		
		fprintf(stderr, "Server reading from %s\n", reqPath); 
		for (x = 0; x < n; x++) {
			buffer[x] = fgetc(pipe);
		}
		fprintf(stderr, "Server read successfully from %s\n", reqPath);
		
		fprintf(stderr, "Server closing %s\n", reqPath);
		fclose(pipe);
	}
	return 0; 
}
