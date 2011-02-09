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
			fprintf(stderr, "Reached the limit, pr_count=%i, pr_limit=%i \n", pr_count, pr_limit);
			wait(NULL);
			pr_count--;
		}
		
		inFile.getline (command, 256);
		
		// TODO: Execute command on a forked process 
		//instead of displaying it
		// (fork, makeargv, execvp)
		cout << command << endl;
		
		pr_count++;
		
		//TODO: check to see if any children have finished
		// Decrement pr_count foreach completed child;
	}
	inFile.close();	
	
	//TODO: Wait for the rest of the children to finish

	return 0;
}

