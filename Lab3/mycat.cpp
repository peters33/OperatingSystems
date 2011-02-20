#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{
	ifstream inFile;
	char* filename;
	
	for (int i = 1; i < argc; i++)						// Iterate through all command line args
	{
		
		inFile.open(argv[i]);
		if (!inFile)
		{
			cerr << "Unable to open ";
			cerr << argv[i];
			exit(1);
		}
		
		inFile.close();
	}
	return 0;
}