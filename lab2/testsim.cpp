#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int main (int argc, char *argv[]) 
{
	if (argc != 3)
	{ /* check for valid number of command-line arguments */
		fprintf(stderr, "Usage: <sleep-time> <# of loops>");
		return 1;
	}
	
	int sleeptime = atoi(argv[1]);
	int loops = atoi(argv[2]);
	
	for(int i = 0; i < loops; i++)
	{
		fprintf(stderr, "Start Sleep");
		sleep(sleeptime);
		fprintf(stderr, "End Sleep");		
	}

	return 0;
}

