/*
Ella Doyle, 21447276 - Operating Systems Assignment.

Academic Integrity and Plagiarism Policy Acknowledgment.
I have read and understood the DCU Academic Integrity and Plagiarism Policy.
I declare that this material submitted by me in this assignment is my own original work, and has not been taken from the work of others, except to the extent that such work has been cited and acknowledged within the text of my work.
*/

/*
The fork.h file is a header file that contains the declaration of the execute_command function.
The purpose of a header file is to provide a way to declare functions and variables to be used by other parts of the program.
By including this header file in the source files where the execute_command function is used, the compiler knows how to call the function and what arguments it takes. 

Firstly we have preprocessor directives - special instructions that are processed before the actual compilation of the code starts.
#ifndef stands for "if not defined". It checks whether the identifier FORK_H has been defined previously.
If it has not been defined, the code between #ifndef and #endif will be in the compilation process.
#define is used to define a preprocessor. FORK_H is defined here.
Then we have the declaration of the execute_command function.
#endif marks the end of the conditional compilation block.
*/

#ifndef FORK_H
#define FORK_H

void execute_command(char **args, int arg_count);

#endif // FORK_H
