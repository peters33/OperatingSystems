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
#include "restart.h"
#define PERMS (S_IRUSR | S_IWUSR | S_IXUSR  | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH )
#define FOLDERPERMS (S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#define READ_FLAGS O_RDONLY
#define WRITE_FLAGS (O_WRONLY | O_CREAT | O_TRUNC)


void printdir(char *dir, int depth)
{

}

int CopyDirectory(char *source, char *destination) 
{
	struct dirent* entry;
	struct stat statbuf;
	DIR* sourceDir;
	DIR* destinationDir;
	char fullpath[1024];
	char chrdir[1024];
	char newDest[1024];
	int size = 0;
	int dirSize = 0;
	int targs[3];

	if ((sourceDir = opendir(source)) == NULL) 
	{
		perror("Failed to open directory");
		return 1;
	}

	if ((sourceDir = opendir(destination)) == NULL) 
	{
		mkdir(destination, FOLDERPERMS);
		if ((sourceDir = opendir(destination)) == NULL) 
		{
			perror("Failed to open directory GAY");
			return 1;
		}
	}
	while ((entry = readdir(sourceDir))) 
	{ 									// iterate through each element in directory
		
		strcpy(chrdir, "");
		strcat(chrdir, source); 											// construct full path to read element
		strcat(chrdir, "/");
		strcat(chrdir, entry->d_name);
		printf("\t%s\n", chrdir);
		strcpy(newDest, "");
		strcat(newDest, destination); 											// construct full path to read element
		strcat(newDest, "/");
		strcat(newDest, entry->d_name);
		printf("\t%s\n", newDest );

		if ((strcmp(entry->d_name, "..") == 0) || (strcmp(entry->d_name, ".") == 0)) 
		{		
			continue;
		}
		printf("HERE\n");
		if (IsDirectory(chrdir)) 
		{
			dirSize = CopyDirectory(chrdir, newDest);				// element is a directory, goto next level first

			stat(chrdir, &statbuf);
			dirSize += (int)statbuf.st_size; 							// add size of cwd link to total

			printf("%-10d", dirSize);
			printf("\t%s\n", chrdir);

			size += dirSize; 											// must add size of this dir to running total	
		} 
		else if (IsFile(chrdir)) 
		{
			if (IsLink(chrdir)) 
			{								// handles the -L option
				lstat(chrdir, &statbuf);
			} 
			else 
			{
				if (((targs[0] = open(chrdir, READ_FLAGS)) == -1) || (targs[1] = open(newDest, WRITE_FLAGS, PERMS)) == -1) 
				{
					perror("Failed to open the files");
					return 1;
				}
				copyfilepass(targs);	//copy files
			}

			stat(chrdir, &statbuf);		
			size += (int)statbuf.st_size;

			printf("%-10d",(int)statbuf.st_size);
			printf("\t%s\n", chrdir);
		}
	}
	closedir(destination);
	closedir(source);
	return size;
}


/* ShowDirectory - traverses a file directory and outputs accordingly
* returns: size of ALL elements inside directory */
int ShowDirectory(char *source, int aFlag,int lFlag) 
{
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
	printf("HERE\n");
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

void *copyfilepass(void *arg)
{
	int *argint;
	argint = (int *)arg;
	argint[2] = copyfile(argint[0], argint[1]);
	r_close(argint[0]);
	r_close(argint[1]);
	return argint + 2;
}
