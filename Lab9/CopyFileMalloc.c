#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "restart.h"
#define PERMS (S_IRUSR | S_IWUSR)
#define READ_FLAGS O_RDONLY
#define WRITE_FLAGS (O_WRONLY | O_CREAT | O_TRUNC)

void *copyfilemalloc(void *arg);

/* copy fromfile to tofile */
int main (int argc, char *argv[]) 
{	
	int *bytesptr;
	int error;
	int fds[2];

	pthread_t tid;
  
	if (argc != 3) 
	{
		fprintf(stderr, "Usage: %s fromfile tofile\n", argv[0]);
		return 1;
	}
  
	if (((fds[0] = open(argv[1], READ_FLAGS)) == -1) || ((fds[1] = open(argv[2], WRITE_FLAGS, PERMS)) == -1)) 
	{
		perror("Failed to open the files");
		return 1;
	}
  
	if (error = pthread_create(&tid, NULL, copyfilemalloc, fds)) 
	{
		fprintf(stderr, "Failed to create thread: %s\n", strerror(error));
		return 1;
	}
  
	if (error = pthread_join(tid, (void **)&bytesptr)) 
	{
		fprintf(stderr, "Failed to join thread: %s\n", strerror(error));
		return 1;
	}
  
	printf("Number of bytes copied: %d\n", *bytesptr);
	return 0;
}

/* copy infd to outfd with return value */
void *copyfilemalloc(void *arg) 
{
	int *bytesp;
	int infd;
	int outfd;

	infd = *((int *)(arg));
	outfd = *((int *)(arg) + 1);

	if ((bytesp = (int *)malloc(sizeof(int))) == NULL)
		return NULL;
	
	*bytesp = copyfile(infd, outfd);
	
	r_close(infd);
	r_close(outfd);
	
	return bytesp;
}