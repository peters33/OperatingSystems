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

#define PERMS (S_IRUSR | S_IWUSR)
#define FOLDERPERMS (S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#define READ_FLAGS O_RDONLY
#define WRITE_FLAGS (O_WRONLY | O_CREAT | O_TRUNC)

int CopyDirectory(char *source, char *destination){

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
	char sourceStr[1024];
	char destStr[1024];


	if ((sourceDir = opendir(source)) == NULL)  //OPEN SOURCE DIRECTORY
	{
		perror("Failed to open source directory\n");
		return 1;
	}

	if ((destinationDir = opendir(destination)) == NULL)    //TRY TO OPEN DESTINATION DIRECTORY
	{
		mkdir(destination, FOLDERPERMS);    //IF IT FAILS TRY TO CREATE IT

		if ((destinationDir = opendir(destination)) == NULL)    //TRY TO OPEN NEWLY CREATED DESTINATION DIRECTORY
		{
			perror("Failed to open destination directory\n");
			return 1;
		}
	}

	while ((entry = readdir(sourceDir)))    // ITERATE THROUGH EVERTYHING IN DIRECTORY
	{

		strcpy(chrdir, "");
		strcat(chrdir, source);             //CONSTRUCT CURRENT SOURCE ELEMENT
		strcat(chrdir, "/");                //PATH RELATIVE TO ORIGINAL SOURCE
		strcat(chrdir, entry->d_name);

		strcpy(newDest, "");                //CONSTRUCT CURRENT DESTINATION ELEMENT
		strcat(newDest, destination);       //PATH RELATIVE TO ORIGINAL DESTINATION
		strcat(newDest, "/");
		strcat(newDest, entry->d_name);


		if ((strcmp(entry->d_name, "..") == 0) || (strcmp(entry->d_name, ".") == 0))    //SKIP PROCESSING IF "." OR ".."
		{
			continue;
		}

		if (IsDirectory(chrdir))
		{
			CopyDirectory(chrdir, newDest);	    //CURRENT ELEMENT IS A DIRECTORY SO RECURSIVELY CALL COPYDIRECTORY()
		}
		else        //CURRENT ELEMENT IS A FILE
		{
            if (((targs[0] = r_open2(chrdir, READ_FLAGS)) == -1) || (targs[1] = r_open3(newDest, WRITE_FLAGS, PERMS)) == -1)    //TRY TO OPEN BOTH SOURCE AND DEST FILES
            {
                perror("Failed to open the files");
                return 1;
            }
            if (copyfilepass(targs) == 0)       //COPY FILE
            {
                perror("Failed to copy the file");
                return 1;
            }
            printf("Copied %s \nto \n%s\n\n", chrdir,newDest);
		}
	}
	closedir(destinationDir);
	closedir(sourceDir);
	return 1;
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

/* IsFile - determines if a filepath is a file */
int IsFile(char *path) {
	struct stat statbuf;
    int fd;
	if (fstat(open(path,O_RDONLY), &statbuf) == -1) {
		return 0;
	} else {
		return S_ISREG(statbuf.st_mode);
	}
}

void *copyfilepass(void *arg){
	int *argint;
	argint = (int *)arg;
	argint[2] = copyfile(argint[0], argint[1]);
    r_close(argint[0]);
    r_close(argint[1]);

	return argint + 2;
}
