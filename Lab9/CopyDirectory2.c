#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include "restart.h"
#include "FileExplorer.h"

int main (int argc, char *argv[])
{

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s Source Destination\n", argv[0]);
		return 1;
	}
	printf("Copying %s to %s...\n",argv[1],argv[2]);

	char destFiles[2056][1024];
	char sourceFiles[2056][1024];
	int numFiles = 0;
	int curCount = 0;

	GetFilePathsAndCount(argv[1],argv[2], &numFiles, sourceFiles, destFiles);
    //CopyDirectory2(sourceFiles, destFiles, numFiles, &curCount);

    int pr_count = 0;
	int pr_limit = 5;
	pid_t childpid = 0;

	while (curCount < numFiles)
	{
		if (pr_count == pr_limit)
		{
			//fprintf(stderr, "Reached the limit, process count = %i, limit = %i\n\n", pr_count, pr_limit);
			wait(NULL);
			pr_count--;
			//fprintf(stderr, "\nA child process finished...retrieving next command.\n");
		}

		//fprintf(stderr, "Current process count = %i, limit = %i\n\n", pr_count, pr_limit);
		pr_count++;

		// fork process
		childpid = fork();
		if(childpid == -1)
		{
			printf("Failed to fork process.\n");
			return 0;
		}

		if (childpid == 0)
		{
            CopyFile(sourceFiles[curCount],destFiles[curCount]);
			return 0;
		}

		//check to see if any children have finished
		// Decrement pr_count foreach completed child;
		int status;
		while(waitpid(0, &status, WNOHANG) != 0)
		{
			pr_count--;
		}
		curCount++;
	}
	//TODO: Wait for the rest of the children to finish
	while(wait(NULL) > 0) ;

    printf("Number of files: %i",numFiles);

	return 0;
}
