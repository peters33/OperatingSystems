// myCat.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <string>

using namespace std;


/*
	CopyInputToOutputStream works and copies inputString to the specified output

	need to implement GetFileStream to take an input file and put it into InputString
	same for GetStandardInput

*/ 
void GetFileStream(char* filename, bool numbered);
void GetStandardInput();
void CopyInputToOutputStream(char* outputStreamName);

string inputString;
fstream fileStr;
FILE * newFile;

int main(int argc, char *argv[])
{	
	int lineNum;
	char buffer[50];
	bool isNumbered = false;
	char* outputFile = "standard";
	inputString = "";

	if (argc == 3 && (strcmp(argv[1],"@") == 0))
	{
		GetStandardInput();
		outputFile = argv[2];
		CopyInputToOutputStream(outputFile);
		return 0;
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
			GetStandardInput();
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
			GetFileStream(argv[i], isNumbered);
		}
	}
	CopyInputToOutputStream(outputFile); // if output file is null, then output is standard out
	return 0;
}

void GetFileStream(char* filename, bool numbered)
{

	fileStr.open(filename);

	if (fileStr.fail()) {
		// open failed, output error message
		fileStr.clear();

	} else {
		int	lineNum = 1;

		// ****************** Begin Read File **********************
		while (!fileStr.eof()) {
			string strLine = "";
			getline(fileStr,strLine);

			if (numbered)
			{
				strLine += lineNum;
				lineNum++;
			}

			inputString += strLine;
			
			//fileStr.getline(buffer, sizeof(buffer));
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

void CopyInputToOutputStream(char* outputStream)
{
	/* Create an fstream based on the output file name
	and copy the all the stored input to the output fstream
	*/

	fileStr.clear();
	fileStr.open(outputStream);

	if (outputStream != "standard") 
	{
       newFile = fopen(outputStream, "w");
       fputs(inputString.c_str(), newFile);
       fclose(newFile);
	}
    else
    { 
     cout << inputString << endl;
 	 fileStr.close();
    }

}

void GetStandardInput()
{
	//inputString = "This is hardcoded standard input";
	//Add standard input until ctrl-D
	//Loop getting strings until control D is found
	char buffer[1024];
	
	while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        inputString += buffer;
	}
}




