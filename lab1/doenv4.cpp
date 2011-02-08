#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

void displayAll(char**);
void modifyAdd(char*);
char** makeVarArray(char*);
int getNumVars(char*);
bool setVariable(char*, char*);
char* getVariable(char*);

extern char ** environ;

int main(int argc, char * argv[])
{
	if (argc == 1)
	{
		displayAll(environ);
	}
    
    if (argc == 2)
    {
        if (strcmp(argv[1],"-") != 0 && strcmp(argv[1],"-i") != 0)
		{
			// Command does not include - or -i, modify/add
			modifyAdd(argv[1]);
		}
    }
    
	if (argc == 3)
	{
		// Command includes - or -i, completely replace env list
		char* variable = argv[2];
		char* assign[10];
		int i = 0;
		char* var;
		char* val;
		if (strcmp(argv[1],"-") != 0 && strcmp(argv[1],"-i") != 0)
		{
            cout << "Error incorrect format - or -i is required to assign values." << endl;
        }
        else
        {
            variable = strtok(variable, ",");
            while (variable != NULL)
            {
                  assign[i] = variable;
                  ++i;
                  variable = strtok(NULL, ",");
            }
            for(int j = 0; j < i; ++j)
            {    
        	    if(strcmp(assign[j], "=") != 0)
        	    {
                    var = strtok(assign[j], "=");
                    val = strtok(NULL, "");
                    cout << var << " " << val <<endl;
                    setVariable(var,val);
                }
                else
                {
                    cout << "Error incorrect format an = is required to assign values." << endl; 
                }
            }
        }
	}
	exit(0);
}

void displayAll(char** env)
{       
	while(*env)
	{
		printf("%s\n",*env);
		env++;
	}
}

int getNumVars(char* vStr) {
	int i = 0;
	char* ptr = vStr;
	
	while (ptr != NULL) {
		i++;
		ptr = strchr(ptr, 44);
		while (ptr != NULL && *ptr == ',') {
			ptr++;
		}
	}
	
	return i;
}

void modifyAdd(char* varStr) {
	char** varArray;
	char* assign[getNumVars(varStr)+1];					// Allows for dynamic sizing of array
	char* name;
	char* value;
	char* ptr;
	char* var;
	char* str;
	int size = 0;
	int i = 0;
	
	var = strtok(varStr, ",");										
	while (var != NULL)									// Break up comma delmited Vars
	{													// and place in array
		assign[i] = var;
        ++i;
		var = strtok(NULL, ",");
	}
	
	varArray = (char**) calloc(i+1,2);					// Can have at most i elements
	for(int j = 0; j < i; j++)							
    {    
		name = strtok(assign[j],"=");					// Extract name and value from each var
		value = strtok(NULL, "");
		
		str = (char*) malloc(strlen(assign[j])+strlen(value)+2);

		if (!str) {
			fprintf(stderr, "Out of Memory!\n");
			free(str);
			exit(0);
		}
		strcpy(str,assign[j]);							// Recreate var string
		strcat(str,"=");
		strcat(str,value);
		
		ptr = getenv(name);
		if (ptr != NULL) {
			putenv(str);								// Replace value in env if var exists
		} else {
			varArray[size++] = str;						// Add to "new" list if var doesnt exist
		}
	}
	
	displayAll(varArray);								// Display new variables
	displayAll(environ);								// Display (modified) env variables
	
	free(assign);
	free(varArray);
}

bool setVariable(char* var, char* value)
{
	char* str;
	str = (char*) malloc(strlen(var)+strlen(value)+2);
	
	if (!str)
	{
		fprintf(stderr, "Out of Memory!\n");
		return false;
	}

	strcpy(str,var);
	strcat(str,"=");
	strcat(str,value);

	cout << "Calling putenv with " << str << endl;

	//IF IT FAILS
	if(putenv(str) != 0)
	{
		fprintf(stderr, "putenv FAILED!\n");
		free(str);
		return false;
	}

	//DISPLAY THE NEW ONE
	value = getVariable(var);
	cout << endl << "Result: " << var << "=" << value << endl;	
	return true;
}

char* getVariable(char* var)
{
	char * value = getenv(var);
	if(value == NULL)
	    return "false";
    else
        return value;
}
