#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
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
	CopyDirectory(argv[1],argv[2]);

	return 0;
}

