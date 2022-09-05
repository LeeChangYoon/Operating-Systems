#pragma once

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strtok() and strcmp()
#include <unistd.h> // for fork() and pid_t
#include <sys/wait.h> // for waitpid() and association marcos


int SiSH_exit();
int numBuiltin();
int SiSH_cd(char** args);
int readConfig(); // read and parse from config file
int SiSH_Interact(); // when myShell is called interactively
int SiSH_Exec(char** args); // function to execute command from terminal
char* readLine(); // function to read a line from command into the buffer
int SiSH_Launch(char** args); // function to create child process and run command
char** splitLine(char* line); // function to split a line into consistent commands
int SiSH_Script(char filename[100]); // when myShell is called with a sscript as argument


int QUIT;
char SHELL_NAME[50];
char* builtin_cmd[2];
