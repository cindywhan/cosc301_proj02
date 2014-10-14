#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <signal.h>





int main(int argc, char **argv) {
	char input[1024]; // array for the input
    // prompt the user
    do {
    	printf("Type in a prompt: \n");
    	fflush(stdout);
    	fgets(input, 1024, stdin);
    	// check if the user wants to quit
    	if (strcmp(input, "exit") == 0){
    		break;
    	}
    	// split the input into sections
    	
    	// make a child
    	pid_t pid = fork();
    	// have the child do the process
    	
    	
    
    }
    return 0;
}

