#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include "restart.h"
#include "restart.c"

// Updated: March 9, 2011

int main (int argc, char *argv[]) {
	FILE* pipe = NULL;
	char buffer;
	char testChar = 'Q';
	int i;
	char reqName[1024];
	char relName[1024];

	if (argc != 2) {  /* name of server fifo is passed on the command line */
		fprintf(stderr, "Usage: %s fifoname\n", argv[0]);
		return 1; 
	}
	
	strcpy(reqName, argv[1]);
	strcat(reqName, ".request");
	
	strcpy(relName, argv[1]);
	strcat(relName, ".release");
	
	// server loops twice 
	for (i = 0; i < 2; i++) {
		fprintf(stderr, "Iteration #%d\n", i + 1);
	
		// open a read/write communication endpoint to the pipe
		fprintf(stderr, "Client %d opening %s\n", getpid(), reqName); 
		if (!(pipe = fopen(reqName, "r+"))) {
			perror("Client failed to open its request FIFO\n");
			return 1;
		}
		
		fprintf(stderr, "Client %d writing '%c' to %s\n", getpid(), testChar, reqName); 

		if (fputc(testChar, pipe) == EOF) {
			perror("Client failed to write character\n");
			return 1;
		}
		
		fprintf(stderr, "Client %d wrote successfully to pipe\n", getpid());
		
		fprintf(stderr, "Client %d closing %s\n", getpid(), reqName);
		fclose(pipe);
		
		fprintf(stderr, "Client %d opening %s\n", getpid(), relName);
		if (!(pipe = fopen(relName, "r"))) {
			perror("Client failed to open its release FIFO\n");
			return 1;
		}
		
		fprintf(stderr, "Client %d reading from %s\n", getpid(), relName); 

		if ((buffer = fgetc(pipe)) == EOF) {
			perror("Client failed to read character\n");
			return 1;
		} 

		fprintf(stderr, "Client %d successfully read '%c' to pipe\n", getpid(), buffer);
		
		fprintf(stderr, "Client %d closing %s\n", getpid(), relName);
		fclose(pipe);
	}	

	
	return 0; 
}
