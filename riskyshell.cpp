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
//#include<conio>
#include<cstdlib>

using namespace std;

void call_my_shell(){
		// clear();
        cout << "\t\t\t\tSMRITI'S SHELL\n\n\n" ;
        // cout << "\t\tUSE AT YOUR OWN RISK\n\n";
        // sleep(2);
} 

void view_mydir(){
	char mydir[1024];
	getcwd(mydir,sizeof(mydir));
	cout << mydir;
	cout<< "$" << endl;
}

/*void redirect_input(char* input){
	int fd = open(input , O_RDONLY | O_CREAT , 0641);
	if(fd  < 0){
		perror("Error in loading input file");
		return;
	}
	else{

	}
	close(fd);
}*/

/*void redirect_output(char* output){
	int fd = open(output , O_WRONLY | O_CREAT , 0641);
	if(fd < 0){
		perror("Error in opening output file.");
		return;
	}
	dup2(fd , 1);
	close(fd);

}*/

void my_fork(char** par , bool in , bool out , bool apend , int k){
	
		pid_t childpid = fork();

		if(childpid < 0)
			perror("Fork not possible"); 
		
		else if(childpid == 0){
			// In child process
			if(in){
				/*int fd = open(par[k+1] , O_RDONLY , 0641);
				if(fd < 0){
				perror("Error in loading input file.");
				return;
				}
				dup2(fd ,0);
				close(fd);*/
			}			

			else if(out){
				// cout << par[k+1];
				int fd = open(par[k+1] , O_WRONLY | O_CREAT , 0641);
				if(fd < 0){
				perror("Error in opening output file.");
				return;
			}
				dup2(fd , 1);
				close(fd);
		}

			else if(apend){
				int fd = open(par[k+1] , O_CREAT | O_WRONLY | O_APPEND , 0644);
    			if (fd < 0) {
        			perror("Error in appending to the file.");
					return;
        		}
        		dup2(fd , 1);
        		close(fd);
    	}
			
		execvp(par[0] , par);
	}
			// perror(command);
		else{
			// In parent process
			wait(NULL);
			
		}
	
}


/*void signal_handler(int sig_num){
	signal(SIGINT , signal_handler);
	cout << "\n";
	fflush(stdout);
	// exit(0);
	// exit(sig_num);
}*/

int change_dir(const char* path){
	if(strcmp(path , "~") == 0)
		path = "/home/smriti/";
	return(chdir(path));
}

void my_alarm(int time){
	cout << "Alert Bitch!\n";
	alarm(10);
	signal(SIGALRM, my_alarm);
}

int main(){
		cout << "\033[H\033[J";
        call_my_shell();
        // signal(SIGALRM, my_alarm);
        // alarm(10);
        char command[256];

        // signal(SIGINT , signal_handler);

         while(1){
        	view_mydir();
        	cin.getline(command , 256);
			// void enterinput();
			vector<char*> parameters;
			char* temp = strtok(command, " ");
			while(temp != NULL){
				parameters.push_back(temp);
				temp = strtok(NULL, " ");
			}

			bool in = false , out = false , apend = false;
			int k = 0;

			char** par = new char*[parameters.size()+1];
    		for ( int i = 0; i < parameters.size(); i++ ){
      			par[i] = parameters[i];
      			if(strcmp(par[i] ,"<") == 0){
      				in = true;
      				k = i;
      				 par[i] = NULL;
      			}
      			if(strcmp(par[i] ,">") == 0){
      				out = true;
      				k = i;
      				par[i] = NULL;
      			}
      			if(strcmp(par[i] ,">>") == 0){
      				apend = true;
      				k = i;
      				par[i] = NULL;
      			}
    		}

    		par[parameters.size()] = NULL;

			/*int i = 0;
			while(i < parameters.size()){
				cout << parameters[i] << endl;
				i++;
			}*/
			if(strcmp(command , "exit") == 0)
				return 0;
			else if (strcmp(par[0] , "cd") == 0){
				if(change_dir(par[1]) == -1)
					cout << "No such directory exists!";
			}

			 /*else if(in || out)
				 in ? redirect_input(par[k+1]) : redirect_output(par[k+1]);*/

			else
				my_fork(par ,in , out , apend , k);
         }
}
