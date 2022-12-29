#include "SiSH.h"

int SiSH_cd(char** args) {
	if (args[1] == NULL) {
		printf("SiSH: expected argument to \"cd\"\n");
	}
	else {
		if (chdir(args[1]) != 0) {
			perror("SiSH");
		}
	}
	return 1;
}


int SiSH_exit() {
	QUIT = 1;
	return 0;
}
