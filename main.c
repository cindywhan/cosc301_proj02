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


//create a struct proc for processes
struct proc{
	struct proc *next;
	pid_t pid;
	char complete; //returns true if process is completed
};

//create a struct job
struct job{
	struct job *next;
	process *first; //list of processes in the job
};


//this function will return true(1) once all processes in the job are completed
int jobstatus (job *job){
	process proc;
	
	for (proc=job->first; proc; proc=proc->next){
		if(!proc->complete){
			return 0;
		}
	return 1;
	}
}

char **split_input(char *input, char *sub){
	// this functions spilts the user input based on the semicolons in the file
	// figure out how long to make the return array 
	char *copy = strdup(input);
	char *tok = strtok(copy, sub);
	int count = 1;
	while (tok != NULL){
		tok = strtok(NULL, sub);
		count ++;
	}
	free(copy);
	// make the return array
	char **rv = malloc((count + 1) * sizeof(char *));
	s
	for (int i = 0; i < count + 1; i++){
		rv[i] = NULL;
	}
	// break the input up based on the semicolons
	int index = 0;
	char *s_copy = strdup(input);
	char *token = strtok(s_copy, sub);
	while (token != NULL){
		char *word = strdup(token);
		rv[index] = word;
		token = strtok(NULL, sub);
		index++;
	}
	free(s_copy);
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

void cut_comments(char **input){
	// takes in the input and gets rid of any comments
	int size = sizeof(*input) / sizeof(char);
	for (int i = 0; i < size; i++){
		if (*input[i] == '#'){
		// anything after # is a comment
			*input[i] = '\0'	
		}
	}
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
    	// take out comments
    	cut_comments(&input);
    	// split the input into sections
    	char **commands = split_input(input, ";");
    	int i = 0;
    	while (command[i] != NULL){
    		char **cmd = split_input(command[i], " \t\n");
    		// insert code for the execv function
    		free_tokens(cmd);
    	}
    	// make a child
    	//pid_t pid = fork();
    	// have the child do the process
    	
    	free_tokens(commands);
    
    }
    while(1){
    	prinf ("$ ");
    	if(fgets(input, 1024, stdin) == NULL){
    		printf("error\n");
    		break;
    	}
    	
    	int child = fork();
    	
    	if (child<0){
    		printf ("Fork failed\n");
    		break;
    	}
    	
    	else if (child == 0){
    		if (strcmp(*commands, "cd") == 0){
    		
    		}
    	}
    	
    	else{
    		wait();
    	}
    }
    return 0;
}

