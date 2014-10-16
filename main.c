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

/*
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

*/

//check if this modifies in place
//modifies string to lowercase in place
char *modestr(char *str){
	for (int i = 0; i <= strlen(str); i++){
		if (str[i]>=65 && str[i]<=90){
			str[i] = str[i]+32;i
		}
	}
	return str;
}


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

/*
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
*/

//do we need to do this? prompt the user for a mode? 
int usermode(){
//this function return true or false about whether a user wants a particular type of mode (sequential or parallel)
//this function is used to determine the initial mode

	char mode[128];
	
	//prompt user for a preferred mode
	printf("Preferred mode? Sequential or Parallel: \n);
   fflush(stdout);
   fgets(mode, 128, stdin);
   if (strcmp (mode, "parallel") == 0 || strcmp (mode, "mode p") == 0){
   	return 0;
   }
   //if user types otherwise (whether it be "sequential", "mode s", or even something random), default is to run it in sequential mode
   return 1;
}

//how to deal with mode changing within line of commands?
//compare to Sommers' from lab3
void runseq(char *cmdarr){
//this function will run through the array of commands, fork for each command, and call the execv() for each process
//sequential mode

  	int childstatus;
  //go through command tokens  
   for (int i = 0; i < sizeof(cmdarr); cmdarr[i+1]){
    	
		//if the current command isn't the last one but is the exit command
    	if (strcmp(cmdarr[i], "exit") && i!=sizeof(cmdarr)-1){
    		;
    	}
    
    	//fork each process as the for loop goes through array of commands, cmdarr
    	pid_t child = fork();
    	
    	if (child < 0){
    		printf ("Fork failed\n");
    		break;
    	}
    	else if (child == 0){
    		//C: is this necessary? mode p is not a command, it is just the user input, no?
    		//checks to see if the current command is a mode change
    		if (strcmp(cmdarr[i],"mode p") == 0 || strcmp(cmdarr[i],"parallel") ==0){
    		runpar(char *cmdarr);
    	}
    		
    		//am i handling this correctly?
    		if(strcmp(cmdarr[i], "mode")){
    			continue;
    		}
    		
    		//else, run the command
    		execv(cmdarr[i], cmdarr);
    		
    		//if execv runs properly, error statement below will not print
    		printf("error");
    	} 
    	else{
    		//need to change to waitpid at some point
    		wait(&childstatus);
    	}
    }
}

void runpar(char *cmdarr){
//this function will run through the array of commands, fork for each command, and call the execv() for each process
//parallel mode

	int childstatus;
	
	//go through command tokens, saving any mode changes or exit commands for last
    for (int i = 0; i < sizeof(cmdarr); cmdarr[i+1]){
    	
    	//checks to see if the current command is a mode change or exit, ignores it for now, but saves it for later
    	//how to deal with multiple mode changes and exit commands? which to execute at the end?
    	if (strcmp(cmdarr[i],"mode s") == 0 || strcmp(cmdarr[i],"mode p") == 0 || strcmp(cmdarr[i],"sequential") ==0 || strcmp(cmdarr[i],"parallel") ==0 || strcmp(cmdarr[i], "mode") == 0 || strcmp(cmdarr[i],"exit") == 0){
    		char *lastcmd[128];
    		strncpy(lastcmd, cmdarr[i]);
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
    		//do i need to fork here? is this done correctly? after I return from child process, do the last command
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


int main(int argc, char **argv) {
	char input[1024]; // array for the input

    // prompt the user
    do {
    	printf("Type in a prompt: \n");
    	//printf ("$ ");
    	fflush(stdout);
    	fgets(input, 1024, stdin);
    	
    	//is this checking just the first node?
    	// check if the user wants to quit
    	if (strcmp(input, "exit\n") == 0){
    		break;
    	}
    	// split the input into sections
    	char **commands = split_input(input);
    	
    	int mode = usermode();
    	
    	//check what mode user wants commands to run in
    	if (mode == 1){
    		runseq(input);
    	}
    	else if (mode == 0){
    		runpar(input);
    	}
    	
    	//int i = 0;
    	// make a child
    	//pid_t pid = fork();
    	// have the child do the process
    	
    	free_tokens(commands);
    
    }

