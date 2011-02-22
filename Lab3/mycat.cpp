#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{
	fstream fileStr;
	int lineNum = 1;
	char buffer[50];
	char* str;
	bool isNumbered = false;
	
	// Iterate through all command line args
	for (int i = 1; i < argc; i++)
	{
		fileStr.clear();
		str = argv[i];
		
		if (strcmp(str,"-n") == 0) {
			isNumbered = true;
			
		} else {
			fileStr.open(argv[i]);
			
			if (fileStr.fail()) {
				// open failed, output error message
				fileStr.clear();
				
			} else {
				// ****************** Begin Read File **********************
				while (!fileStr.eof()) {
					fileStr.getline(buffer, sizeof(buffer));
					cout << buffer << endl; // Temporary
				}
				
				// ****************** End Read File ************************
			}
			fileStr.close();
		}
	}
	return 0;
}
