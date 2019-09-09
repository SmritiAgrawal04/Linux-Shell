#include<stdio.h>
#include<string.h>
#include<string>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<iostream>
#include<fcntl.h>
#include<vector>
#include<fstream>
#include<stack>
#include<cstdlib>

using namespace std;

void my_fork(char** par , bool out , bool apend , char* o , char* a , int* stat){
		// cout << "fg = " <<fg;
		// cout << "bg = " << bg;
	
		pid_t childpid = fork();

		if(childpid < 0)
			perror("Fork not possible"); 
		
		else if(childpid == 0){
			

			if(out){
				out = false;
				// cout << par[k+1];
				int fd = open(o , O_WRONLY | O_CREAT ,0644);
				if(fd < 0){
					perror("Error in opening output file.");
					return;
					// exit(0);
				}
				dup2(fd , 1);
				close(fd);
				
		}

			if(apend){
				apend = false;
				int fd = open(a , O_CREAT | O_WRONLY | O_APPEND , 0644);
    			if (fd < 0) {
        			perror("Error in appending to the file.");
					return;
        		}
        		dup2(fd , 1);
        		close(fd);
        		
    	}
			
			execvp(par[0] ,  par);
	}
			// perror(command);
		else{
			// In parent process
				int status;
				wait(&status);
				*stat = WEXITSTATUS(status);
			
		}
	
}