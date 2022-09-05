#include "SiSH.h"


int main(int argc, char** argv) {
	// initialization
	QUIT = 0;
	builtin_cmd[0] = "cd";
	builtin_cmd[1] = "quit";
	strcpy(SHELL_NAME, "SiSH");

	// read from myShell configurration files
	readConfig();

	// parsing commands interactive mode or script mode
	if (argc == 1) SiSH_Interact();
	else if (argc == 2) SiSH_Script(argv[1]);
	else printf("\nInvalid Number of Arguments.");

	// exit the shell
	return EXIT_SUCCESS;
}
