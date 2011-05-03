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
#include "FileExplorer.h"


void printdir(char *dir, int depth)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if ((dp = opendir(dir))==NULL)
	{
		perror("Failed to open directory");
		return 1;
	}

	chdir(dir);

	while ((entry = readdir(dp)) != NULL)
	{
		lstat(entry->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode))
		{
			if (strcmp(".",entry->d_name)== 0 || strcmp("..", entry->d_name) == 0)
			{
				continue;
			}
			printf("%*s&s/|n",depth,"",entry->d_name);
			printdir(entry->d_name,depth+4);
		}
		else
		{
			printf("%*s&s/|n",depth,"",entry->d_name);
		}
		chdir("..");
		closedir(dp);
	}
}

/* ShowDirectory - traverses a file directory and outputs accordingly
* returns: size of ALL elements inside directory */
int ShowDirectory(char *source, int aFlag,int lFlag) {
	struct dirent* entry;
	struct stat statbuf;
	DIR* dir;
	char fullpath[1024];
	char chrdir[1024];
	int size = 0;
	int dirSize = 0;

	if ((dir = opendir(source)) == NULL) {
		perror("Failed to open directory");
		return 1;
	}

	while ((entry = readdir(dir))) { 									// iterate through each element in directory
		strcpy(chrdir, "");
		strcat(chrdir, source); 											// construct full path to read element
		strcat(chrdir, "/");
		strcat(chrdir, entry->d_name);

		if ((strcmp(entry->d_name, "..") == 0) || (strcmp(entry->d_name, ".") == 0)) {
			continue;
		}

		if (IsDirectory(chrdir)) {
			dirSize = ShowDirectory(chrdir, aFlag, lFlag);				// element is a directory, goto next level first

			stat(chrdir, &statbuf);
			dirSize += (int)statbuf.st_size; 							// add size of cwd link to total

			printf("%-10d", dirSize);
			printf("\t%s\n", chrdir);

			size += dirSize; 											// must add size of this dir to running total	
		} else if (IsFile(chrdir)) {

			if (IsLink(chrdir) && !lFlag) {								// handles the -L option
				lstat(chrdir, &statbuf);
			} else {
				stat(chrdir, &statbuf);									
			}

			size += (int)statbuf.st_size;

			if (aFlag) { 												// handles the -a option
				printf("%-10d",(int)statbuf.st_size);
				printf("\t%s\n", chrdir);
			}
		}
	}

	closedir(dir);
	return size;
}

/* IsDirectory - determines if a filepath is a directory */
int IsDirectory(char *path) {
	struct stat statbuf;

	if (stat(path, &statbuf) == -1) {
		return 0;
	} else {
		return S_ISDIR(statbuf.st_mode);
	}
}

/* IsLink - determines if a filepath is a symbolic link */
int IsLink(char *path) {
	struct stat statbuf;

	if (lstat(path, &statbuf) == -1) {
		return 0;
	} else {
		return S_ISLNK(statbuf.st_mode);
	}
}

/* IsFile - determines if a filepath is a file */
int IsFile(char *path) {
	struct stat statbuf;

	if (fstat(open(path,O_RDONLY), &statbuf) == -1) {
		return 0;
	} else {
		return S_ISREG(statbuf.st_mode);
	}
}
