#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "restart.h"
#include "restart.c"
#define FIFO_PERMS (S_IRWXU | S_IWGRP| S_IWOTH)

// Updated: March 9, 2011

int main (int argc, char *argv[]) {
	FILE* pipe = NULL;
	char reqName[1024];
	char relName[1024];
	int i;
	int x;
	int n = atoi(argv[2]);
	char buffer[n];
	printf("HERE I AM!");
	if (argc != 3) {
		fprintf(stderr, "Usage: %s [barrier name] [barrier size]\n", argv[0]);
		return 1; 
	}
	
	strcpy(reqName, argv[1]);
	strcat(reqName, ".request");
	
	strcpy(relName, argv[1]);
	strcat(relName, ".release");
	
	// create a named pipe to handle incoming requests
	if ((mkfifo(reqName, FIFO_PERMS) == -1) && (errno != EEXIST)) {
		perror("Server failed to create a FIFO\n");
		return 1; 
	}
	
	// create a named pipe to handle writing requests
	if ((mkfifo(relName, FIFO_PERMS) == -1) && (errno != EEXIST)) {
		perror("Server failed to create a FIFO\n");
		return 1;
	}
	
	// server loops twice 
	for (i = 0; i < 2; i++) {
		fprintf(stderr, "Iteration #%d\n", i + 1);
	
		// open a read/write communication endpoint to the pipe
		fprintf(stderr, "Server opening %s\n", reqName); 
		if (!(pipe = fopen(reqName, "r"))) {
			perror("Server failed to open its request FIFO\n");
			return 1;
		}
		
		fprintf(stderr, "Server reading from %s\n", reqName); 
		for (x = 0; x < n; x++) {
			
			if ((buffer[x] = fgetc(pipe)) == EOF) {
				perror("Sever failed to read character\n");
				return 1;
			}
		}
		fprintf(stderr, "Server read successfully from pipe\n");
		
		fprintf(stderr, "Server closing %s\n", reqName);
		fclose(pipe);
		
		pipe = NULL;
		
		fprintf(stderr, "Server opening %s\n", relName);
		if (!(pipe = fopen(relName, "w"))) {
			perror("Server failed to open its release FIFO\n");
			return 1;
		}
		
		fprintf(stderr, "Server writing to %s\n", relName); 
		for (x = 0; x < n; x++) {
			
			fprintf(pipe,"%c",buffer[x]);
		}
		fprintf(stderr, "Server wrote successfully to pipe\n");
		
		fprintf(stderr, "Server closing %s\n", relName);
		fclose(pipe);
		
		pipe = NULL;
	}
	
	if (unlink(relName) == -1) {
		perror("Failed to remove FIFO\n");
	}
	
	if (unlink(reqName) == -1) {
		perror("Failed to remove FIFO\n");
	}
	
	fprintf(stderr, "FIFOs removed, server closing\n");
	return 0; 
}
