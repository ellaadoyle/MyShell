/*
Ella Doyle, 21447276 - Operating Systems Assignment.

Academic Integrity and Plagiarism Policy Acknowledgment.
I have read and understood the DCU Academic Integrity and Plagiarism Policy.
I declare that this material submitted by me in this assignment is my own original work, and has not been taken from the work of others, except to the extent that such work has been cited and acknowledged within the text of my work.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include <dirent.h> // For the ls command, provides functions and data types for working with directories.

// SUPPORTED COMMANDS (INTERNAL COMMANDS)

// Using ANSI escape codes to clear the terminal scree.
void clr() {
    // system("clear");
    printf("\033[2J\033[H");
}

/*
DIR/ LS COMMAND EXPLINATION.
First we declare 2 variables, a pointer to a 'DIR' data type named 'dirp',
and another pointer to a 'struct dirent' data type named 'dp'. Struct dirent is a data structure used to represent directory entries.
We then call the opendir() function to open the current directory and assigns the result to dir.
A while loop starts and used the readdir() function to read the directory objects pointed to by dirp.
The loop continues until readdir() returns NULL, ie: there are no more directory objects to read.
Inside the loop, we print the name of the current directory entry using d_name which holds the name of the directory entry.
Finally we call the closedir() function to close the directory pointer.
*/
void dir() {
	DIR *dirp; // (directory pointer)
    struct dirent *dp;
    dirp = opendir(".");
    while ((dp = readdir(dirp)) != NULL) {
		printf("%s\n", dp->d_name);
    }
    closedir(dirp);
}

/*
ENVIRONMENT COMMAND EXPLINATION.
Firstly we have to declare an external variable, which is a pointer to an array of strinhs representing environemnt variables.
The for loop iterates through the arrau of environment variables pointed to by environ.
The loop uses a pointer variable 'environments' to keep track of the current position in the array and continues as long as the value is not NULL.
The print statement then prints the current environemnt variable.
*/

extern char **environ;

void environ_command() {;
	for (char **environments = environ; *environments != NULL; environments++) {
		printf("%s\n", *environments);
	}
}

void quit() {
	printf("Exiting shell ...\n");
	exit(0);
}

/*
CD COMMAND EXPLINATION.
If the argument is not present, report the current directory.
Change the directory to the parent directory.
Change the PWD environment variable to the new working directory.
If the directory does not exist, report an appropriate error
*/

void cd(char *args[], int arg_count) {
	char *dir = args[1];

	if(dir == NULL) {
		printf("%s\n", getenv("PWD"));
	}
	else if (strcmp(dir, "..") == 0) {
		if(chdir("..") == 0) {
			char new_pwd[1024];
			getcwd(new_pwd, sizeof(new_pwd));
			setenv("PWD", new_pwd, 1);
		}
		// Error handling. Incase 'chdir' fails.
		else {
			printf("Failed to change directory.\n");
		}
	}
	else {
		if(chdir(dir) == 0) {
			char new_pwd[1024];
			getcwd(new_pwd, sizeof(new_pwd));
			setenv("PWD", new_pwd, 1);
		}
		else {
			// Error handling. If the directory does not exist, report an appropriate error
			printf("Directory '%s' does not exist.\n", dir);
		}
	}
}

/*
ECHO COMMAND EXPLINATION.
Define a function which takes in 2 parameters, args and arg_count.
Check is the value of arg is less than or equal to 1, if so just print a new line as there is no arguments to print.
I then start a loop which iterates through each element of the args array and prints them.
Finally after printing each arguemnt with a space after it, it prints a new line character to format the output.
*/

void echo(char *args[], int arg_count) {
// Error handling. If the 'echo' function is called with no arguments, it will print a new line, rather than not printing anything.
	if(arg_count <= 1) {
		printf("\n");
		return;
	}
	for(int i = 1; i < arg_count; i++) {
		printf("%s ", args[i]);
	}
	printf("\n");
}

// Defining the help function by using the system() function with the 'more' argument that allows a file to be displayed in the terminal one page at a time.
void help() {
	system("more ../manual/readme.md");
}

// Deining the pause command that prompts the user to press Enter and waits for the user to do so before continuing execution. 
void pause_command() {
	printf("Press Enter to continue...");
	getchar(); // wait for user to press Enter.
}
