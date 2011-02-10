#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <fstream>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>

using namespace std;

int main (int argc, char *argv[]) 
{
	FILE* inFile;
	int pr_count = 0;
	int pr_limit = atoi(argv[1]);
	pid_t childpid = 0;
	char cmdLineStr[256];
	int status;
	fpos_t pos;

	if (pr_limit == 0)
	{
		perror ("Process limit must be greater than 0. Exiting...");
	}

	inFile = fopen("testsim.data", "r");
	if (inFile == NULL)
	{
		perror ("Error opening file. Exiting...");
	}
	else 
	{
		while (fgets(cmdLineStr, 256, inFile) != NULL)
		{
			// Store current position in file.
			fgetpos(inFile, &pos);
			
			// Ignore blank lines in file
			if(!strcmp(cmdLineStr, "\n"))
			{
				continue;
			}
		
			if (pr_count == pr_limit)
			{
				fprintf(stderr, "Reached process limit, waiting...\n");
				
				// Wait for next running process to terminate
				wait(NULL);
				pr_count--;		
			}
			
			fprintf(stderr, "Current process count = %i, process limit = %i\n", pr_count, pr_limit);

			if((childpid = fork()) < 0) 
			{
				fprintf(stderr, "Failed to fork process.\n");
				return 1;
			}

			pr_count++;

			if (childpid == 0)
			{
				// Execute command
				system(cmdLineStr);
				return 0;
			}

			//check to see if any children have finished
			// Decrement pr_count foreach completed child;
			while(waitpid(0, &status, WNOHANG) != 0)
			{
				pr_count--;
			}
		}

		fclose(inFile);
		
		//TODO: Wait for the rest of the children to finish
		while(wait(NULL) > 0) ;
	}

	return 0;
}




