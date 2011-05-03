#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>

void printdir(char*, int);
int CopyDirectory(char*, char*); 
int ShowDirectory(char*, int, int);
int IsDirectory(char*);
int IsLink(char*);
int IsFile(char*);
void *copyfilepass(void *arg);