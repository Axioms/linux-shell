#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>

main(){
    int p[2];
    pipe(p); // Creates a pipe with file descriptors Eg. input = 3 and output = 4 (Since, 0,1 and 2 are not available)

    if (fork() == 0) {
    // Child process
        close(p[1]);
        close(p[0]);
        dup2(p[0], 0);
        exec("cmd2");
    } else {
        //Parent process
        close(1); // Release fd no - 1
        close(p[0]); // Close pipe fds since useful one is duplicated
        close(p[1]);
        dup(p[1]); // Create duplicate of fd - 4 (pipe write end) with fd 1.
        exec("cmd1");
    }
}
