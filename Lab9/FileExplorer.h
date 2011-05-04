#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>


int CopyDirectory1(char*, char*);
int CopyDirectory2(char[2056][1024], char[2056][1024], int);
int GetFilePathsAndCount(char*, char*, int*, char[2056][1024], char[2056][1024]);
int IsDirectory(char*);
int IsFile(char*);
void *copyfilepass(void *arg);
