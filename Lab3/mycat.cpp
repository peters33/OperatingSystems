#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{
	fstream fileStr;
	int lineNum;
	char buffer[50];
	bool isNumbered = false;
	
	// Iterate through all command line args
	for (int i = 1; i < argc; i++)
	{
		fileStr.clear();
		
		if (strcmp(argv[i],"-n") == 0) {
			isNumbered = true;
			
		} else {
			fileStr.open(argv[i]);
			
			if (fileStr.fail()) {
				// open failed, output error message
				fileStr.clear();
				
			} else {
				lineNum = 1;
				
				// ****************** Begin Read File **********************
				while (!fileStr.eof()) {
					fileStr.getline(buffer, sizeof(buffer));
					
					if (isNumbered) {
						printf("%i\t%s\n", lineNum, buffer);
						lineNum++;
					} else {
						printf("%s\n", buffer);
					}
				}
				// ****************** End Read File ************************
			}
			fileStr.close();
		}
	}
	return 0;
}
