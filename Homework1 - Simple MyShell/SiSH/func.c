#include "SiSH.h"


// array of function pointers for call from execShell
int (*builtin_func[]) (char**) = { &SiSH_cd, &SiSH_exit }; 
int numBuiltin() { return sizeof(builtin_cmd) / sizeof(char*); }


int SiSH_Launch(char** args) {
	int status;
	pid_t pid, wpid;

	pid = fork();
	if (pid == 0) {
		// child process
		if (execvp(args[0], args) == -1) {
			perror("SiSH");
		}
		exit(EXIT_FAILURE);
	}

	// forking error
	else if (pid < 0) {
		perror("SiSH");
	}

	// parent process
	else {
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}


int SiSH_Exec(char** args) {
	int ret;
	
	// empty command
	if (args[0] == NULL) {
		return 1;
	}

	// loop to chek for builtin functions
	for (int i = 0; i < numBuiltin(); i++) {
		// check if user function matches the builtin function name
		if (strcmp(args[0], builtin_cmd[i]) == 0)
			// call respective builtin function with arguments
			return (*builtin_func[i])(args);
	}
	ret = SiSH_Launch(args);
	return ret;
}


int readConfig() {
	time_t t = time(NULL);
	char* pwd = getenv("PWD");
	char* user = getenv("USER");
	
	printf("\nUser: %s", user);
	printf("\nTime: %s", ctime(&t));
	printf("Path: %s\n\n", pwd);
	return 1;
}

int SiSH_Interact() {
	char* line;
	char** args;
	while (QUIT == 0) {
		printf("%s> ", SHELL_NAME);
		line = readLine();
		args = splitLine(line);
		SiSH_Exec(args);
		free(line);
		free(args);
	}
	return 1;
}

int SiSH_Script(char filename[100]) {
	FILE* fptr;
	char** args;
	char line[200];
	printf("Received Script. Opening %s", filename);

	fptr = fopen(filename, "r");
	if (fptr == NULL) {
		printf("\nUnable to open file.\n");
		return 1;
	}

	else {
		printf("\nFile Opened. Parsing. Parsed commands displayed first.");
		while (fgets(line, sizeof(line), fptr) != NULL) {
			printf("\n%s", line);
			args = splitLine(line);
			SiSH_Exec(args);
		}
	}
	free(args);
	fclose(fptr);
	return 1;
}
