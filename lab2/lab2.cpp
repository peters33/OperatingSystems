#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main (int argc, char *argv[]) 
{
	pid_t childpid = 0;
	int i, n;
	int pr_count; //Holds number of active children
	int pr_limit = 10; //Max number of child processes
	
	if (argc != 2)
	{ /* check for valid number of command-line arguments */
		fprintf(stderr, "Usage: %s processes\n", argv[0]);
		return 1;
	}
	
	n = atoi(argv[1]);
	
	for (i = 1; i < n; i++)
	{
		if ((childpid = fork()) <= 0)
		{
			fprintf(stderr, "Error!);
			break;	
		}
		
		if (pr_count == pr_limit)
		{
			//TODO: 
			// 1.	print a message
			// 2.	wait for a child process to finish
			// 3.	decrement pr_count
		}		
	}

	fprintf(stderr, "i:%d process ID:%ld parent ID:%ld child ID:%ld\n",i, (long)getpid(), (long)getppid(), (long)childpid);
	return 0;
}