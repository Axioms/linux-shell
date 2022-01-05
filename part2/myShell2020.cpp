#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/time.h>

#define WRITE_END 0
#define READ_END 1

int get_number_of_spaces(std::string str) {
	int count = 0;
	
	if(str.length() > 1) {
		count = 1;
	}
	for (uint i = 0; i < str.length(); i ++) {
		if (str.substr(i, 1) == " ") {
			count ++;
		}
	}
	return count;
}

void get_command(std::string str,char* &prog, char** &args) {
	char* string = strdup(str.c_str());
	char* runner = strtok(string, " ");
	prog = runner;
	args = new char*[get_number_of_spaces(str) + 2];
	args[0] = prog;

	int i = 1;
	while( runner != NULL ) {
      runner = strtok(NULL, " ");
	  args[i] = runner;
	  i++;
   }
   //free(string);
}

bool get_pipe_command(char* &prog1, char** &args1, char* &prog2, char** &args2, bool &run) {
	std::string input;
	
	std::cout << "mysh> ";
	std::getline(std::cin, input);

	if (input.find("|") < input.length() - 1) {
		run = true;
		get_command(input.substr(0,input.find("|") - 1), prog1, args1);

		get_command(input.substr(input.find("|") + 2), prog2, args2);

		return true;
	}
	else if( input == "exit" ){
		return false;
	}

	else {
		run = false;
		return true;
	}
}

int main() {
	char *prog1 = NULL, *prog2 = NULL;
	char **args1 = NULL, **args2 = NULL;
	bool run = true;
	//char *ls_args[] = {(char*)"ls", (char*)"-l", (char*)NULL};
	//char *wc_args[] = {(char*)"wc", (char*)"-l", (char*)NULL};

	int pipefd[2];

	pid_t child_pid1, child_pid2;

	while (get_pipe_command(prog1, args1, prog2, args2, run)) {
		if (run) {
			pipe(pipefd);
			child_pid1 = fork();

			if (child_pid1 == 0) {
				dup2(pipefd[1], 1);
				close(pipefd[1]);
				close(pipefd[0]);
				execvp(prog1, args1);
				//execlp(prog1, prog1, args1, (char*) NULL);
				//execvp(ls_args[0], ls_args);
				
				//execlp(ls_args[0], ls_args[0], ls_args[1], ls_args[2]);
				fprintf(stderr, "Failed to execute '%s'\n", prog1);
				exit(1);
			}

			else {
				child_pid2 = fork();

				if(child_pid2 == 0) {
					dup2(pipefd[0], 0);
					close(pipefd[0]);
					close(pipefd[1]);
					execvp(prog2, args2);
					//execlp(prog2, prog2, args2, (char*) NULL);
					//execvp(wc_args[0], wc_args);
					//execlp(wc_args[0], wc_args[0], wc_args[1], wc_args[2]);
					fprintf(stderr, "Failed to execute '%s'\n", prog2);
					exit(1);
				}

				else {
					//wait(NULL); 
					//wait(NULL); 
					//wait(&child_pid2);
					//wait(&child_pid1);
					//std::cout << "ran " << prog1 << " " << args1[1] << " | " << prog2 << " " << args2[2];
					
					int status1;
					int status2;
					close(pipefd[READ_END]);
					close(pipefd[WRITE_END]);
					waitpid(child_pid1, &status1, 0);
					waitpid(child_pid2, &status2, 0);
					
					//std::cout << "Done" << std::endl;
					prog1 = NULL;
					prog2 = NULL;
					args1 = NULL;
					args2 = NULL;
				}
			}
		}
	}
}