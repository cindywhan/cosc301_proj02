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
	rv[index] = NULL;
	free(s_copy);
	return rv;

}



void runseq(char **cmdarr){
//this function will run through the array of commands, fork for each command, and call the execv() for each process
//sequential mode
	// make a child
    pid_t pid = fork();
    if (pid < 0){
    	printf ("Fork failed\n");
    }
    else if (pid == 0){
    	if (execv(cmdarr[0], cmdarr) < 0){
    		fprintf(stderr, "execv failed\n"); // child executes the code
    	}
    } 
    int child = 0;
    wait(&child);
}

void runpar(char **cmdarr){
//this function will run through the array of commands, fork for each command, and call the execv() for each process
//parallel mode

	int childstatus;
	char lastcmd[128];
	//go through command tokens, saving any mode changes or exit commands for last
    for (int i = 0; i < sizeof(cmdarr)/sizeof(char*); i++){
    	
    	//checks to see if the current command is a mode change or exit, ignores it for now, but saves it for later
    	if (strcmp(cmdarr[i],"mode s") == 0 || strcmp(cmdarr[i],"mode p") == 0 || strcmp(cmdarr[i],"sequential") ==0 || strcmp(cmdarr[i],"parallel") ==0 || strcmp(cmdarr[i], "mode") == 0 || strcmp(cmdarr[i],"exit") == 0){
    		strcpy(lastcmd, cmdarr[i]);
    		i++;
    	}
    	
    	//fork each process as the for loop goes through array of commands, cmdarr
    	pid_t child = fork();
    	
    	if (child < 0){
    		printf ("Fork failed\n");
    		break;
    	}
    	else if (child == 0){
    		execv(cmdarr[i], cmdarr);
    		
    		//if execv runs properly, error statement below will not print
    		printf("error");
    	} 
    	else{
    		//need to change to waitpid at some point
    		wait(&childstatus);
    		//run the mode change or exit saved from earlier
    		execv(lastcmd, cmdarr);
    	}
    }
}

void free_tokens(char **arr){
	int i = 0;
	while (arr[i] != NULL){
		free(arr[i]); // free each string
		i++;
	}
	free(arr);
}

char **format_input(char **in){
	// format the command for the execv function
	// format = [command, options, NULL]
	char **rv = malloc(3 * sizeof(char *));
	char options[32];
	memset(options, 0, 32);
	rv[0] = strdup(in[0]);
	int i = 1;
	int index = 0;
	// collect all the options for the command
	while(in[i] != NULL){
		if (in[i][0] == '-'){
			for (int n = 1; n < strlen(in[i]); n++){
				options[index] = in[i][n];
				index++;
			}
		}
	}
	options[index] = '\0';
	// copy options into the format return array
	if (index == 0){
		rv[1] = NULL;
	}
	else{
		rv[1] = strdup(options);
	}
	rv[2] = NULL;
	return rv;

}

char *cut_comments(char *input){
	// takes in the input and gets rid of any comments
	char *rv = malloc(sizeof(input));
	
	for (int i = 0; i < strlen(input); i++){
		if (input[i] == '#'){
		// anything after # is a comment
			rv[i] = '\0';	
		}
		else{
			rv[i] = tolower(input[i]);
		}
	}
	return rv;
}



int main(int argc, char **argv) {
	char input[1024]; // array for the input
	memset(input, 0, 1024);
    // prompt the user
    while(1) {
    	printf("prompt>  ");
    	fflush(stdout);
    	fgets(input, 1024, stdin);
    	
    	// take out comments
    	char *in = cut_comments(input);
		int mode = 0; // default mode (0) is seq and mode (1) is par
    	
    	char **commands = split_input(in, ";");
    	free(in);
    	for (int i = 0; i < sizeof(commands)/sizeof(char *); i++){
    		// check if the command is valid
    		char **cmd = split_input(commands[i], " \t\n");
    		char **format = format_input(cmd);
    		free_tokens(cmd);
    		if (strcmp(format[0],"mode") == 0 || strcmp(format[0],"sequential") ==0 || strcmp(format[0],"parallel") == 0){
    			// change the mode 
    			if (strcmp(format[0], "mode") == 0){
    				if (strcmp(format[1], "s") == 0){
    					mode = 0;
    				}
    				else if (strcmp(format[1], "p") == 0){
    					mode = 1;
    				}
    			}
    			else if (strcmp(format[0], "sequential") == 0){
    				mode = 0;
    			}
    			else if (strcmp(format[0], "parallel") == 0){
    				mode = 1;
    			}
    		} 
    		else if (format[0] != NULL){
    			switch(mode){
    			case 1: // run parallel code
    				runpar(commands + i);
    				break;
    			default:
    				runseq(format);
    				break;
    			}
    			
    		}
    		free_tokens(format);
    		}
    	free_tokens(commands);
    }
}

