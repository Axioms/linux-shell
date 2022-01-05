#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>


int main(int argc, char *argv[]) { 
char *prog1 = NULL, *prog2 = NULL; 
char **args1 = NULL, **args2 = NULL; 

int pipefd[2]; // file descriptors for the pipe 
int childpid1 , childpid2 ;

// we omit the details of parsing a pipe command 
while (readAndParsePipeCmd(&prog1 , &args1 , &prog2 , &args2 ))
 { 
// create a pipe with two open file descriptors 
// can write to pipefd[1] and read from pipefd[0] 
// open file descriptors are inherited across fork 
pipe(pipefd);

// Create the first child process 
childpid1 = fork ();
if ( childpid1 == 0) { 
// I ’m the first child process. 
// Run prog1 with stdout as the write end of the pipe 
dup2(pipefd [1] , stdout ); 
exec(prog1 , args1 );
 		// NOT REACHED 
}
 	else { 
// I ’m the parent 
// Create the second child process 
childpid2 = fork (); 
if ( childpid2 == 0) { 
// I ’m the second child process. 
// Run prog2 with stdin as the read end of the pipe 
dup2(pipefd [0] , stdin ); exec(prog2 , args2 ); 
// NOT REACHED 
} else { 
// I ’m the parent 
wait(childpid2); 
wait(childpid1);
 			return 0; 
} 
  }
}  // end while
}  // end main
