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
	char testChar = 'Q';
	char rBuffer[1];
	char wBuffer[1];
	int i;
	char reqName[1024];
	char relName[1024];

	if (argc != 2) {  /* name of server fifo is passed on the command line */
		fprintf(stderr, "Usage: %s fifoname\n", argv[0]);
		return 1; 
	}
	
	wBuffer[0] = testChar;
	
	strcpy(reqName, argv[1]);
	strcat(reqName, ".request");
	
	strcpy(relName, argv[1]);
	strcat(relName, ".release");
	printf("here");
	// server loops twice 
	for (i = 0; i < 2; i++) {
		fprintf(stderr, "Client %d Iteration #%d\n", getpid(), i + 1);
	
		// open a read/write communication endpoint to the pipe
		fprintf(stderr, "Client %d opening %s\n", getpid(), reqName); 
		if (!(pipe = fopen(reqName, "w"))) {
			perror("Client failed to open its request FIFO\n");
			return 1;
		}
		
		fprintf(stderr, "Client %d writing '%c' to %s\n", getpid(), wBuffer[0], reqName); 

		if (fputs(wBuffer, pipe) == EOF) {
			perror("Client failed to write character\n");
			return 1;
		}
		
		fprintf(stderr, "Client %d wrote successfully to pipe\n", getpid());
		
		fprintf(stderr, "Client %d closing %s\n", getpid(), reqName);
		fclose(pipe);
		
		pipe = NULL;
		
		fprintf(stderr, "Client %d opening %s\n", getpid(), relName);
		if (!(pipe = fopen(relName, "r"))) {
			perror("Client failed to open its release FIFO\n");
			return 1;
		}
		
		fprintf(stderr, "Client %d reading from %s\n", getpid(), relName); 

		
		if ((fgets(rBuffer, 2, pipe)) == NULL) {
			perror("Client failed to read character\n");
			return 1;
		} 

		fprintf(stderr, "Client %d successfully read '%c' from pipe\n", getpid(), rBuffer[0]);
		
		fprintf(stderr, "Client %d closing %s\n", getpid(), relName);
		fclose(pipe);
		
		pipe = NULL;
	}	

	
	return 0; 
}
