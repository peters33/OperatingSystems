#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

int main (int argc, char *argv[]) 
{
	if (argc != 3)
	{ /* check for valid number of command-line arguments */
		fprintf(stderr, "Usage: <sleep-time> <# of loops>");
		return 1;
	}
	
	int sleeptime = atoi(argv[1]);
	int loops = atoi(argv[2]);
	
	fprintf(stderr, "testsim %d %d starting...\n", sleeptime, loops);
	
	for(int i = 0; i < loops; i++)
	{	
		// Output command line, process ID, parent process ID, and iteration
		fprintf(stderr, "Process ID=%li, Parent Process ID=%li, Sleep iteration=%i\n", (long)getpid(), (long)getppid(), i+1);
		sleep(sleeptime);
	}

	return 0;
}

