#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>


int main() {
	pid_t pid;
	
	bool should_run = true;

	std::string str_command[2];
	std::string input;
	
	while (should_run) {
		
		std::cout << "mysh> ";
		std::getline(std::cin, input);
		str_command[0] = input.substr(0, input.find(" "));

		if (input.find(" ") <= input.length() - 1) {
			str_command[1] = input.substr(input.find(" ") + 1);
		} else {
			str_command[1] = strdup("");
		}

		char* command[3] = {strdup(str_command[0].c_str()),strdup(str_command[1].c_str()),NULL};


		pid = fork();
		if (str_command[0] == "exit") {
			should_run = false;
		}
		else if (pid < 0) {
			fprintf(stderr, "Fork Failed"); 
			return 1;
		}

		else if (pid == 0) {
			execvp(command[0], command);
			free(command[0]);
			free(command[1]);
		}

		else {
			wait(NULL);
		} 
		
	}

	return 0;
}