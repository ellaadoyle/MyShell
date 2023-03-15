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
// Used for I/O Redirection
#include <fcntl.h> // for open(), close(), O_RDONLY, dup(), etc.

/*
This code is executed when the shell wants to execute a command that is not one of the Supported Commands (Internal Commands).
This allows the shell to execute commands by creating a child process to execute them.
While also ensuring that the parent process waits for the child process to complete before resuming control.
*/

void execute_command(char **args, int arg_count) {
// This line creates a new process by duplicating the calling process, it returns, once in the parent process and once in the child process.
	pid_t pid = fork();
/*
Child process code. In the child process, pid is set to 0.
The first if statement checks if the current process is the child process.
Then we set the shell environment variable to the shell's full path
Using the getenv() function we retrieve the value of the shell environemnt variable.
If the shell environment variable is not set, getenv will return NULL and the if statement checks this.
The program then prints an error message and exits.
The last line sets the value of the shell environment variable to shell_path.
*/
	if (pid == 0) {
		char *shell_path = getenv("SHELL");
	    if (shell_path == NULL) {
            printf("Error: shellenvironment variable not set\n");
            exit(0);
		}
        setenv("shell", shell_path, 1);
		// setenv("parent", "/users/comsci2/doylee62/ca216/2023-ca216-myshell/myshell", 1); hardcoding the shell path.


// I/O REDIRECTION
/*
Explination of my IO redirection code.
The variables input_fd and output_fd stand for input/ output file descriptors.
File descriptors are a number that the operating system assigns to a file when it's opened. Commonly used manage input and output  operations.
They are initialized to -1 as a default value in case they are not assiged a file descriptor.
-1 is used to indicate an error within a file descriptor.

The for loop iterates over each argument in the argument list.
Inside the for loop, the code checks for input or output redirection operators, '<', '>', or '>>' using strcmp().

If the '<' operator is found, open() is used to open the file with read only access (O_RDONLY).
It returns a file descriptor, which is assigned to the input_fd variable.
([i+1] points to the filename argument following the < operator).
The next if statment checks whether the open() was successful, as mentioned earlier if the input_fd is less than 0 (-1), an error occured.
Is so, an error message is printed and the program terminates.

The next 2 lines set the '<' operator and the filename to NULL, effectivly removing them.
This is so that they dont get passed to the command being executed.
i++ then allows the loop to skip over the operator and filename that have already been dealt with.

The same process is repeated if the '>' operator is found.
Except instead of opeing with read only access the file is opened with write only access (O_WRONLY).
Also, if the file doesnt exist it is created (O_CREAT) and if it does exist it is truncated/erased (O_TRUNC).

Similarly again, if the '>>' operator is found, the file is opened with write only access (O_WRONLY), and the file is created if it does not exist.
But if the file exisits, new data is appended to the end (O_APPEND).
*/
		int input_fd = -1;
		int output_fd = -1;
	    for (int i = 0; i < arg_count; i++) {
	        if (strcmp(args[i], "<") == 0) {
        		input_fd = open(args[i+1], O_RDONLY);
        		if (input_fd < 0) {
       				printf("Error: could not open input file.\n");
        			exit(1);
        		}
        		args[i] = NULL;
        		args[i+1] = NULL;
        		i++;
        	}
        	else if (strcmp(args[i], ">") == 0) {
// The '0644' part represents the file permissions, it defines read, write and execute permissions.
// '0644' specifies that the file has read and write permissions for the owner and read permissions for the group owner and all other users.
        		output_fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        	    if (output_fd < 0) {
        	    	printf("Error: could not open output file.\n");
        	        exit(1);
        	    }
        	    args[i] = NULL;
        	    args[i+1] = NULL;
        	    i++;
			}
			else if (strcmp(args[i], ">>") == 0) {
				output_fd = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			    if (output_fd < 0) {
			    	printf("Error: could not open output file.\n");
			        exit(1);
			    }
			    args[i] = NULL;
			    args[i+1] = NULL;
			    i++;
			}
		}
/*
Outside of the for loop this code sets up I/O redirecton.
Firstly we check if an input file has been specefied.
Then, using the dup2() function we duplicate the file descriptors.
It also replaces the standard input stream (STDIN_FILENO) (ie: from the keyboard) with the file descriptor of the input file that was opened earlier (input_fd).
The close() function then closes the original file descriptor for the input file.
The same process is repeated for the output file.
*/
		if (input_fd != -1) {
			dup2(input_fd, STDIN_FILENO);
		    close(input_fd);
		}
		if (output_fd != -1) {
			dup2(output_fd, STDOUT_FILENO);
		    close(output_fd);
		}

// END OF I/O REDIRECTION

/*
This code is excecuted when the shell need to execute a command that is not one of the built-in commands.
The code allows the shell to execute external commands by creating child process tp execute them.
While also ensuring that the parent process waits for the child process to complete before resuming control.
*/
		if(execvp(args[0], args) < 0) {
			// Error handling. Incase 'execvp()' fails to execute.
			printf("Could not execute command.\n");
			//exit(0);
		}
		// Error handling. The child process exits if 'execvp() fails.
		exit(0);

	}
	else if(pid > 0) {

// BACKGROUND EXECUTION
/*
Parent process code. Checks if the last arguemtn is '&'.
Do not wait for the child process to finish, instead add it to the background process list.
Assumes the use of a dynamically allocated array to store the background process IDs.
Also assumes the use of a global variable to keep track of the number of background processes running.
*/
		if (strcmp(args[arg_count-1], "&") == 0) {
		//if (args[arg_count-1][0] == '&') {
			printf("[%d] %s\n", pid, args[0]);
			// printf("%s", args[arg_count-1]);
// Small issue with the ampersand.
			args[arg_count-1][0] = '\0';
			// args[arg_count-1] = NULL;
		}
		else {
			waitpid(pid, NULL, 0);
		}
	}
	else {
		// Error handling.
		printf("Fork failed.\n");
	}
}
