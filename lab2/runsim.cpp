#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <fstream>
#include <errno.h>
#include <string.h>

using namespace std;

int makeargv(const char *, const char *, char ***);

int main(int argc, char *argv[])
{
	ifstream inFile;
	char** myargv;
	const char* delim = " ";
	int pr_count = 0;
	int numtokens;
	int pr_limit = atoi(argv[1]);
	pid_t childpid = 0;

	inFile.open("testsim.data");
	if (!inFile)
	{
		cerr << "Unable to open testsim.data\n";
		exit(1);
	}

	fprintf(stderr, "\n***************Begin Simulation***************\n");

	char command[256];
	while (! inFile.eof() )
	{
		if (pr_count == pr_limit)
		{
			//TODO: Implement waiting stuff
			// Decrement count
			fprintf(stderr, "Reached the limit, process count = %i, limit = %i\n\n", pr_count, pr_limit);
			wait(NULL);
			pr_count--;
			fprintf(stderr, "\nA child process finished...retrieving next command.\n");
		}

		inFile.getline (command, 256);

		fprintf(stderr, "Reading in command line...received: %s\n", command);
		fprintf(stderr, "Current process count = %i, limit = %i\n\n", pr_count, pr_limit);
		pr_count++;

		// Execute command on a forked process
		// (fork, makeargv, execvp)
		childpid = fork();
		if(childpid == -1)
		{
			printf("Failed to fork process.\n");
			return 0;
		}

		if (childpid == 0)
		{
			if ((numtokens = makeargv(command, delim, &myargv)) == -1) {
				fprintf(stderr, "Failed to construct an argument array for %s\n", command);
				return 0;
			}
			execvp(myargv[0], &myargv[0]);
			return 0;
		}

		//check to see if any children have finished
		// Decrement pr_count foreach completed child;
		int status;
		while(waitpid(0, &status, WNOHANG) != 0)
		{
			pr_count--;
		}
	}

	inFile.close();

	//TODO: Wait for the rest of the children to finish
	while(wait(NULL) > 0) ;

	return 0;
}

int makeargv(const char *s, const char *delimiters, char ***argvp) {
	int error;
	int i;
	int numtokens;
	const char *snew;
	char *t;

	if ((s == NULL) || (delimiters == NULL) || (argvp == NULL)) {
		errno = EINVAL;
		return -1;
	}

	*argvp = NULL;
	snew = s + strspn(s, delimiters); /* snew is real start of string */

	if ((t = (char*)malloc(strlen(snew) + 1)) == NULL) {
		return -1;
	}
	strcpy(t, snew);
	numtokens = 0;

	if (strtok(t, delimiters) != NULL) { /* count the number of tokens in s */
		for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++);
		/* create argument array for ptrs to the tokens */
		if ((*argvp = (char**)malloc((numtokens + 1)*sizeof(char *))) == NULL) {
			error = errno;
			free(t);
			errno = error;
			return -1;
		}
	}

	/* insert pointers to tokens into the argument array */
	if (numtokens == 0) {
		free(t);
	} else {
		strcpy(t, snew);
		**argvp = strtok(t, delimiters);
		for (i = 1; i < numtokens; i++) {
			*((*argvp) + i) = strtok(NULL, delimiters);
		}
	}

	*((*argvp) + numtokens) = NULL; /* put in final NULL pointer */
	return numtokens;
}

