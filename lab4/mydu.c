// mydu.c - Replicates the behavior of the UNIX os command du

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

/* 
* Special function below is used to process options in the command line args
*
* int getopt(int argc, char * const argv[], const char *optstring);
*
* Pass args argc and argv from main and optstring containing option chars you require
* Returns integer char value of next option found, -1 if end reached
*/

int ShowDirectory(char*, int, int);
int IsDirectory(char*);
int IsLink(char*);
int IsFile(char*);

int main(int argc, char **argv)
{
	int opt;
	int size = 0;
	char* filepath;
	struct stat statbuf;

	int refLinks = 0; 													// Option flags
	int showFiles = 0;

	while ((opt = getopt(argc, argv, "aL")) != -1) { 					// Process options first
		switch (opt) {
			case 'a':
				showFiles = 1;
				break;
			case 'L':
				refLinks = 1;
				break;
			default:
				break;
		}
	}

	if (optind == argc) {												// no filepath was specified as an arg
		filepath = ".";
	} else {
		filepath = argv[optind]; 
	}

	size = ShowDirectory(filepath, showFiles, refLinks);				// returns index of first non-option arg in argv

	stat(filepath, &statbuf); 											// output original filepath
	size += (int)statbuf.st_size;

	printf("%-10d", size);
	printf("\t%s\n", filepath);

	return 0;
}

/* ShowDirectory - traverses a file directory and outputs accordingly
* returns: size of ALL elements inside directory */
int ShowDirectory(char *param, int aFlag,int lFlag) {
	struct dirent* entry;
	struct stat statbuf;
	DIR* dir;
	char fullpath[1024];
	char chrdir[1024];
	int size = 0;
	int dirSize = 0;

	if ((dir = opendir(param)) == NULL) {
		perror("Failed to open directory");
		return 1;
	}

	while ((entry = readdir(dir))) { 									// iterate through each element in directory
		strcpy(chrdir, "");
		strcat(chrdir, param); 											// construct full path to read element
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
