#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <fstream>

using namespace std;

int main (int argc, char *argv[])
{
	ifstream inFile;
	int pr_count = 0;
	int pr_limit = atoi(argv[1]);
	pid_t childpid = 0;

	inFile.open("testsim.data");
	if (!inFile)
	{
		cerr << "Unable to open testsim.data";
		exit(1);
	}

	char command[256];
	while (! inFile.eof() )
	{
		if (pr_count == pr_limit)
		{
			//TODO: Implement waiting stuff
			// Decrement count
			fprintf(stderr, "\nReached the limit, pr_count=%i, pr_limit=%i \n", pr_count, pr_limit);
			wait(NULL);
			pr_count--;
			fprintf(stderr, "A child process finished\n");
		}

		inFile.getline (command, 256);

		// Execute command on a forked process
		// (fork, makeargv, execvp)

		fprintf(stderr, "The command line is : ");
		fprintf(stderr, command);
		fprintf(stderr, "Current pr_count = %i, pr_limit = %i \n", pr_count, pr_limit);
		pr_count++;
		childpid = fork();
		if(childpid == -1)
		{
			printf("Failed to fork process.");
			return 0;
		}

		if (childpid == 0)
		{
			//RUN THE "command" figure out how to get
			// argument list
			//execvp(program,args);
			system(command);
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

