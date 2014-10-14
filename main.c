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


char **split_input(char *input){
	// this functions spilts the user input for the execv function to use
	// find out how long to make the return array
	char *cp = strdup(input);
	char *tok = strtok(cp, " \t\n");
	int count = 1;
	while (tok != NULL){
		tok = strtok(NULL, " \t\n");
		count++;
	}
	free(cp);
	// make the return array
	char **rv = malloc((count + 1) * sizeof(char *));
	char *copy = strdup(input);
	// split the input into sections
	char *token = strtok(copy, " \t\n");
	int index = 0;
	while (token != NULL){
		rv[index] = strdup(token);
		token = strtok(NULL, " \t\n");
		index++;
	}
	free(copy);
	return rv;
	

}

void free_tokens(char **arr){
	int i = 0;
	while (arr[i] != NULL){
		free(arr[i]); // free each string
		i++;
	}
	free(arr);
}


int main(int argc, char **argv) {
	char input[1024]; // array for the input
    // prompt the user
    do {
    	printf("Type in a prompt: \n");
    	fflush(stdout);
    	fgets(input, 1024, stdin);
    	
    	// check if the user wants to quit
    	if (strcmp(input, "exit\n") == 0){
    		break;
    	}
    	// split the input into sections
    	char **commands = split_input(input);
    	int i = 0;
    	// make a child
    	//pid_t pid = fork();
    	// have the child do the process
    	
    	free_tokens(commands);
    
    }
    while(1);
    return 0;
}

