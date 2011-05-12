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

#define MAXTHREADS	5

static char destFiles[2056][1024];
static char sourceFiles[2056][1024];

struct thread_data
{
    int totalFiles;
};

struct thread_data curThreadData[MAXTHREADS];

void *CopyDirectory3(void *threadargs)
{
    int ttlFiles;
    char srcPaths[2056][1024];
    char destPaths[2056][1024];
    struct thread_data *my_data;

    my_data = (struct thread_data *) threadargs;
    ttlFiles = my_data->totalFiles;
    //srcPaths = my_data->sourcePaths;
    //destPaths = my_data->destinationPaths;

    int localCount = 0;

    getCountThenIncrement(&localCount);

    while (localCount < ttlFiles)
    {
        CopyFile(sourceFiles[localCount],destFiles[localCount]);
        getCountThenIncrement(&localCount);
    }
}

int main (int argc, char *argv[])
{

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s Source Destination\n", argv[0]);
		return 1;
	}
	printf("Copying %s to %s...\n",argv[1],argv[2]);

    //TODO: CHANGE TO CHAR *[1024]

	int numFiles = 0;
	int curCount = 0;

	GetFilePathsAndCount(argv[1],argv[2], &numFiles, sourceFiles, destFiles);

    //printf("Number of files to copy: %i\n\n", numFiles);


	pthread_t threadIDs[MAXTHREADS];

	int curThread = 0;
	int error;

	while (curThread < MAXTHREADS)
	{
	    curThreadData[curThread].totalFiles = numFiles;

	    if (error = pthread_create(&threadIDs[curThread], NULL, CopyDirectory3, curThreadData)) {
            fprintf(stderr, "Failed to create thread %d: %s\n", curThread, strerror(error));
            threadIDs[curThread] = pthread_self();
        }
	    curThread++;
	}

	curThread = 0;
	while (curThread < MAXTHREADS)
	{
	    if (pthread_equal(pthread_self(), threadIDs[curThread])){
            continue;
	    }

        if (error = pthread_join(threadIDs[curThread], NULL)){
            fprintf(stderr, "Failed to join thread %d: %s\n", curThread, strerror(error));
        }
	    curThread++;
    }
}
