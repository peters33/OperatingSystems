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

int main (int argc, char *argv[]) 
{
	pid_t childpid = 0;
	int i, n;
	if (argc != 3)
	{ /* check for valid number of command-line arguments */
		fprintf(stderr, "Usage: %s serverName numClients\n", argv[0]);
		return 1;
	}
	n = atoi(argv[2]);
	
	
	//DO I NEED TO DO THIS???
	execl("barrier_server", argv[1], argv[1], '2', NULL);
	
	for (i = 1; i < n; i++)
	{
		childpid = fork();
		if(childpid == -1)
		{
			printf("Failed to fork process.\n");
			return 0;
		}
		
		if (childpid == 0)
		{
		//THIS ISN"T WORKING VERY WELL EITHER
			//char* command = "barrier_client runserver"; 
			//execute the client
			//system(command);
			execl("barrier_client", argv[1], argv[1], NULL );
			return 0;
		}		
	}
	return 0;
}
