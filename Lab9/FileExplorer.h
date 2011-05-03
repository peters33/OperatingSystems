#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>

void printdir(char *dir, int depth);
int ShowDirectory(char*, int, int);
int IsDirectory(char*);
int IsLink(char*);
int IsFile(char*);