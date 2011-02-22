#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>

using namespace std;

void GetFileStream(char* filename);
void GetStandarInput();
void CopyOutputStreamToOutputFile();

//WHAT DO WE STORE EVERYTHING (input filestreams and standard input) IN UNTIL WE ARE DONE?

int main(int argc, char *argv[])
{
	fstream fileStr;
	int lineNum;
	char buffer[50];
	bool isNumbered = false;
	char* outputFile;
	
	if (argc == 3 && (strcmp(argv[i],"@") == 0))
	{
		//WE HAVE A SPECIAL BLANK SCENARIO
		// cat > outputfile.txt
		// aka mycat @ outfile.txt
	}

	// Iterate through all command line args
	for (int i = 1; i < argc; i++)
	{
		fileStr.clear();
		
		// -N
		if (strcmp(argv[i],"-n") == 0) 
		{
			isNumbered = true;			
		} 
		// -
		if (strcmp(argv[i],"-") == 0)
		{
			GetStandarInput();
		}
		// @ outputfilename
		if (strcmp(argv[i],"@") == 0)
		{
			i++;
			outputFile = argv[i];
		}
		// filename
		else
		{
			GetFileStream(argv[i])
		}
	}
	CopyOutputStreamToOutputFile(outputFile); // if output file is null, then output is standard out
	return 0;
}

void GetFileStream(filename)
{
	fileStr.open(filename);

	if (fileStr.fail()) {
		// open failed, output error message
		fileStr.clear();

	} else {
		lineNum = 1;

		// ****************** Begin Read File **********************
		while (!fileStr.eof()) {
			fileStr.getline(buffer, sizeof(buffer));

			//if (isNumbered) {
			//	outputstream << lineNum << " " << buffer << endl;
			//	//	printf("%i\t%s\n", lineNum, buffer);
			//	//lineNum++;
			//} else {
			//	outputstream << buffer << endl;
			//	//printf("%s\n", buffer);
			//}
		}
		// ****************** End Read File ************************
	}
	fileStr.close();
}

void CopyOutputStreamToOutputFile(char* filename)
{
	/* Create an fstream based on the output file name
	and copy the all the stored input to the output fstream
	*/
}

void GetStandarInput()
{
	Add standard input until ctrl-D
		Loop Get-Line Calls until control D is found
}

