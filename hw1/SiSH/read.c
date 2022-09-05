#include "SiSH.h"


char* readLine() {
	char c;
	int pos = 0, buffsize = 1024;
	char* line = (char*)malloc(sizeof(char) * 1024); // dynamically allocate buffer

	// buffer allocation failed
	if (!line) {
		printf("\nBuffer Allocation Error");
		exit(EXIT_FAILURE);
	}

	while (1) {
		c = getchar();

		if (c == EOF || c == '\n') {
			line[pos] = '\0';
			return line;
		}
		else {
			line[pos] = c;
		}
		pos++;

		// if we have exceeded the buffer
		if (pos >= buffsize) {
			buffsize += 1024;
			line = realloc(line, sizeof(char) * buffsize);
			if (!line) {
				printf("\nBuffer Allocation Error");
				exit(EXIT_FAILURE);
			}
		}
	}
}


char** splitLine(char* line) {
	char* token;
	int pos = 0, buffsize = 64;
	char delim[10] = " \t\n\r\a";
	char** tokens = (char**)malloc(sizeof(char*) * 64);

	if (!tokens) {
		printf("\nBuffer Allocation Error");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, delim);
	while (token != NULL) {
		tokens[pos] = token;
		pos++;
		if (pos >= buffsize) {
			buffsize += 64;
			line = realloc(line, buffsize * sizeof(char*));
			if (!line) {
				printf("\nBuffer Allocation Error");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, delim);
	}
	tokens[pos] = NULL;
	return tokens;
}
