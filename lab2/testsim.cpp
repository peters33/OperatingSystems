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
		// Output command line, process ID, parent process ID, and iteration
		fprintf(stderr, "Process ID=%i, Parent Process ID=%i, Sleep iteration=%i\n", getpid(), getppid(), i+1);
		sleep(sleeptime);
	}

	return 0;
}

