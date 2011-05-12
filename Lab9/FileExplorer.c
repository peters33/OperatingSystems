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



typedef struct {
    char sourcePath[1024];
    char destinationPath[1024];
} processcopy_t;


int CopyDirectory1(char *source, char *destination){

	struct dirent* entry;
	struct stat statbuf;
	DIR* sourceDir;
	DIR* destinationDir;
	char fullpath[1024];
	char chrdir[1024];
	char newDest[1024];
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
			CopyDirectory1(chrdir, newDest);	    //CURRENT ELEMENT IS A DIRECTORY SO RECURSIVELY CALL COPYDIRECTORY()
		}
		else        //CURRENT ELEMENT IS A FILE
		{
		    CopyFile(chrdir,newDest);
		}
	}

	closedir(destinationDir);
	closedir(sourceDir);
	return 1;
}

int CopyFile( char sourcePath[1024], char destPath[1024] ){
	int targs[3];
	if (((targs[0] = open(sourcePath, READ_FLAGS)) == -1) || (targs[1] = open(destPath, WRITE_FLAGS, PERMS)) == -1)    //TRY TO OPEN BOTH SOURCE AND DEST FILES
	{
		perror("Failed to open the files\n");
		perror(sourcePath);
		perror(destPath);
		return 1;
	}
	if (copyfilepass(targs) == 0)       //COPY FILE
	{
		perror("Failed to copy the file");
		return 1;
	}
	//fprintf(stderr,"Copied %s \nto \n%s\n\n", sourcePath,destPath);
	//fprintf(stderr,".");
}

int GetFilePathsAndCount3(char *source, char *destination, int *fileCount, char sourcePaths[2056][1024], char destPaths[2056][1024]){

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
			GetFilePathsAndCount(chrdir, newDest, fileCount, sourcePaths, destPaths);	    //CURRENT ELEMENT IS A DIRECTORY SO RECURSIVELY CALL COPYDIRECTORY()
		}
		else        //CURRENT ELEMENT IS A FILE
		{
		    //TODO: ADD FILES TO **files
            strcpy(sourcePaths[*fileCount],"");
            strcat(sourcePaths[*fileCount],chrdir);
            strcpy(destPaths[*fileCount],"");
            strcat(destPaths[*fileCount],newDest);
		    //strcat(sourcePaths[0],newDest);
            *fileCount = *fileCount + 1;
		}
	}
	closedir(destinationDir);
	closedir(sourceDir);
	return 1;
}

int GetFilePathsAndCount(char *source, char *destination, int *fileCount, char sourcePaths[2056][1024], char destPaths[2056][1024]){

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
			GetFilePathsAndCount(chrdir, newDest, fileCount, sourcePaths, destPaths);	    //CURRENT ELEMENT IS A DIRECTORY SO RECURSIVELY CALL COPYDIRECTORY()
		}
		else        //CURRENT ELEMENT IS A FILE
		{
		    //TODO: ADD FILES TO **files
            strcpy(sourcePaths[*fileCount],"");
            strcat(sourcePaths[*fileCount],chrdir);
            strcpy(destPaths[*fileCount],"");
            strcat(destPaths[*fileCount],newDest);
		    //strcat(sourcePaths[0],newDest);
            *fileCount = *fileCount + 1;
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
