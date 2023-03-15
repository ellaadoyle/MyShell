/*
Ella Doyle, 21447276 - Operating Systems Assignment.

Academic Integrity and Plagiarism Policy Acknowledgment.
I have read and understood the DCU Academic Integrity and Plagiarism Policy.
I declare that this material submitted by me in this assignment is my own original work, and has not been taken from the work of others, except to the extent that such work has been cited and acknowledged within the text of my work.
*/



// These C include files are used to include external libraries in the program.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h> 
/*
#include preprocessor directives,
Which copies the contents of the header file into the source code file.
This allows the compiler to access the declarations of functions that are defined in the header files.
*/
#include "commands.h"
#include "fork.h"

/*
The input buffer is a temporary storage area used to hold data before it is processed by a program.
In this case its used to store the user's command line input before it is processed by my shell.
I specified the maximum size of the input buffer that will be used to read input from the user and use it throughout my code.
*/
#define BUFFER_SIZE 1024

//Defining the main function which declares and initializies several variables.
int main(int argc, char *argv[]) {
	char buffer[BUFFER_SIZE];
	char *args[100];
	int arg_count = 0;
	char *token;

/*
The first line of this code gets the value of the shell environment variable using the getenv() function and stores it in a char pointer.
The getenv() function is used to retrieve the variable that contains the path to the user's default shell program. The value is stored in the shell_path variable as a string.
Then an if statement checks if shell_path is NULL. If it is, it means that the shell environment variable is not set.
An error is printed out and the program exits.

A character array called pwd with a size of 1024 is declared.
The getcwd() function is used to get the current working directory and store it in the pwd array.
The second argument in getcwd() specifies the size of the array to prevent buffer overflow.
The next line sets the PWD environment variable to the value stored in the pwd array using the setenv() function.
Similarly, the next line sets the shell environment variable to the value stored in the shell_path pointer also using the setenv() function.
This line is setting the shell variable to the full path of the shell executable.
The third argument in both setenv() functions, '1', specifies that any existing values should be overwritten.
*/
	char *shell_path = getenv("SHELL");
	if (shell_path == NULL){
		printf("Error: unable to get the shell environment variable\n");
		exit(0);
	}
    char pwd[1024];
    getcwd(pwd, sizeof(pwd));
    setenv("PWD", pwd, 1);
    setenv("shell", shell_path, 1);

// This code allows the command line prompt to contain the pathname of the current directory.
	char prompt[1024];
	sprintf(prompt, "[%s] myshell ==> ", getcwd(NULL, 0));


// BATCH MODE
/*
Firstly, I declare a character array of size BUFFER_SIZE named batchfile_path to store the path to the batchfile.
Then declare a pointer variable batchfile of type FILE* and initializes it to NULL.
Checking whether the argc variable, which represents the number of command line arguments passed to the program, is equal to 2.
If so, it means that a batch file was specified as a command line argument.
The sprintf() function is used to write formatted data to a character string.
Calling the sprintf() function which makes the path to the batchfile (ie: adding '../' to the beginning of the file name.)
Then we open the batchfile by the batchfile_path in read mode only.
Then assigning the resulting file pointer to batchfile. If the file is empty it will return NULL & an error message.
*/
	char batchfile_path[BUFFER_SIZE];
	FILE *batchfile = NULL;
	if(argc == 2) {
		//sprintf(batchfile_path, "../%s", argv[1]); // Depending on which file the batchfile in.
		sprintf(batchfile_path, "%s", argv[1]);

	    batchfile = fopen(batchfile_path, "r");
		// Error handling.
		if(batchfile == NULL) {
			printf("Could not open file '%s'.\n", argv[1]);
			exit(1);
		}
	}

/*
Start an infinite loop that will continue running until the program is terminated.
Check whether a batch file has been opened, as indicated by the batchfile variable being not NULL.
Call the fgets() function to read a line of input from the batch file, until it reaches the end (NULL), into the buffer variable.
Then, close the batch file using the fclose() function & exit.
Print the input read from the batch file to the console, prefixed with the ==> string.
*/
	while(1) {
		if(batchfile != NULL) {
			// Error handling.
			if(fgets(buffer, BUFFER_SIZE, batchfile) == NULL) {
				fclose(batchfile);
				exit(0);
			}
			printf("%s %s", prompt, buffer);
		}

/*
This code block is executed when a batch file was not opened, as indicated by the batchfile variable being NULL.
Print the ==> string to the console to prompt the user that they can enter input.
Calling the fgets() function to read a line of input from the user via standard input into the buffer variable.
If fgets() returns NULL, exit.
*/
		else {
			printf("%s", prompt);
			if(fgets(buffer, BUFFER_SIZE, stdin) == NULL){
				exit(0);
			}
		}

/*
Tokenize the user's input into individual words/tokens.
Initialize the token variable with the strtok() function which takes a pointer to the string to be parsed and a delimiter character to split the string.
Start a while loop which continues until there are no tokens left.
Then, assign the current token to an element in the args array and inremeent the arg_count variable to keep trak of the command line argument.
The last line uses the strtok() function again to find the next token. The NULL tells us it should continue parsing from where it left off.
*/
		token = strtok(buffer, " \n");
		while(token != NULL) {
			args[arg_count] = token;
			arg_count++;
			token = strtok(NULL, " \n");
		}


// SUPPORTED COMMANDS (INTERNAL COMMANDS)
/*
Using the strcmp() function, I have created a series of conditional statements comparing the first command line argument to a set of strings.
The first line sets the last element of args array to NULL to mark the end of the arry.
Then, depending on the value of args[0], a corresponding function is called to preform a shell command, which are all located in the commands.c file.
*/
		args[arg_count] = NULL;

		if(strcmp(args[0], "clr") == 0) {
			clr();
		}

		else if(strcmp(args[0], "dir") == 0) {
			dir();
		}

		else if(strcmp(args[0], "environ") == 0) {
			environ_command();
		}

		else if(strcmp(args[0], "quit") == 0) {
			quit();
		}

		else if(strcmp(args[0], "cd") == 0) {
			cd(args, arg_count);
		}

		else if(strcmp(args[0], "echo") == 0) {
			echo(args, arg_count);
		}

		else if(strcmp(args[0], "help") == 0) {
			help();
		}

		else if(strcmp(args[0], "pause") == 0) {
			pause_command();
		}

// The final else statement indicates that none of the other statement were met therefore the execute_command function is called which is loacted in the fork.c file.
		else {
			execute_command(args, arg_count);
		}

		// Reset the arguments for the next command.
		arg_count = 0;
	}
	return 0;
}
