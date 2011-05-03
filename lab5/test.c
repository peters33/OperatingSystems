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
	
	for (i = 0; i < n; i++)
	{
		pid_t childpid = fork();
		char command[1024];
		
		if (childpid == 0)
		{
			sprintf(command,"barrier_client %s", argv[1]);
			//execl("barrier_client", argv[1], argv[1], NULL );
			system(command);
		}		
	}
	return 0;
}
