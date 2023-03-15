/*
Ella Doyle, 21447276 - Operating Systems Assignment.

Academic Integrity and Plagiarism Policy Acknowledgment.
I have read and understood the DCU Academic Integrity and Plagiarism Policy.
I declare that this material submitted by me in this assignment is my own original work, and has not been taken from the work of others, except to the extent that such work has been cited and acknowledged within the text of my work.
*/

/*
The commands.h header file contains declarations for functions that correspond to various built-in shell commands.

Firstly, we have preprocessor directives - special instructions that are processed before the actual compilation of the code starts.
They are also used to prevent multiple inclusions of the same file, which can cause errors.
Then, we have the declarations of the command functions.
#endif marks the end of the conditional compilation block.
*/

#ifndef COMMANDS_H
#define COMMANDS_H

void clr();
void dir();
void environ_command();
void quit();
void cd(char *args[], int arg_count);
void echo(char *args[], int arg_count);
void help();
void pause_command();

#endif // COMMANDS_H
