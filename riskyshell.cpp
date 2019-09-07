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

stack<pid_t> processid;
bool fg =false;
bool bg = false;
int pipecount =0;
int ptr = 0;
int diff = 0;
vector<string> hist; 

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

void my_fork(char** par , bool out , bool apend , char* o , char* a){
		// cout << "fg = " <<fg;
		// cout << "bg = " << bg;
	
		pid_t childpid = fork();

		if(childpid < 0)
			perror("Fork not possible"); 
		
		else if(childpid == 0){
			// In child process
			/*if(in){
				int fd = open(par[k+1] , O_RDONLY , 0641);
				if(fd < 0){
				perror("Error in loading input file.");
				return;
				}
				dup2(fd ,0);
				close(fd);
			}*/	
			

			/*if(bg){
				// cout <<"hi" <<bg;
				bg = false;
				processid.push(getpid());
				if(kill(getpid() , SIGTSTP) < 0)
					perror("kill(SIGTSTP)");
				
			}	

			if(fg){
				// tcgetpgrp();
				// cout << "In FG";
				fg = false;
				if(!processid.empty()){
					pid_t temp = processid.top();
					processid.pop();
					if(kill(temp , SIGCONT) < 0)
						perror("kill(SIGCONT)");
				}
				

			}
			*/	

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
			if(!bg)
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
	cout << "ALERT! ALERT!! ALERT!!!\n";
	alarm(100);
	signal(SIGALRM, my_alarm);
}

void print_history(){
	for(int i = hist.size()-1 ; i > -1 ; i--){
		cout << hist[i] << endl;
	}
}

vector<char*> split_string(char command[256]){
	vector<char*> parameters;
	char* temp = strtok(command, " ");
	while(temp != NULL){
	parameters.push_back(temp);
	temp = strtok(NULL, " ");
	}
	return parameters;
}

void pipe_fork(char** par , int len){
	int fd[2];
	int des = 0;
	int cmd_count = pipecount + 1;
	// cout << "cmd_count =" << cmd_count << endl;
	for(int i = 0 ; i < cmd_count ; i++){
		 pipe(fd);

		pid_t childpid = fork();

		if(childpid < 0)
			perror("Fork not possible"); 
		
		if(childpid == 0){
			dup2(des , 0);
			if(i+1 < cmd_count)
				dup2(fd[1] , STDOUT_FILENO);

			close(fd[0]);
			close(fd[1]);

			/*for(int k = 0 ; k < len ; k++){
				cout << par[k] << endl;
			}*/
			 // cout<<ptr<<"dobara\n"<<len<<"length\n";
		 // cout<<par[ptr]<<"fefefe\n";
		while(ptr < len)
			{   if(strcmp(par[ptr],"|")!=0)
				{ptr++;
                 // cout<<"inside while inside if "<<ptr<<endl;
				}
                else
                	break;
			}// cout << par[ptr] << endl;
			ptr++;		
			   // cout <<"After increment"<< ptr<<endl;
		char** abcd = new char*[2];

		
			abcd[0] = par[diff];
			abcd[1] = par[diff+1];
		
		  // cout << "abcd[0] =" << abcd[0]<<endl;
		  // cout << "abcd[1] =" << abcd[1]<<endl;
		diff = ptr - diff ;
		// cout << "diff = "<< diff;
		execvp(abcd[0] , abcd);
			
		}
		else{
			wait(NULL);
			des = fd[0];
			close(fd[1]);
		}
	}
}

int main(){
	pid_t ppid = getpid();

		cout << "\033[H\033[J";
		cout << ppid;
        signal(SIGALRM, my_alarm);
        alarm(1000);
        char command[256];

        // signal(SIGINT , signal_handler);

         while(1){
        	view_mydir();
        	cin.getline(command , 256);

        	if(strcmp(command , "") == 0)
        		continue;
			// void enterinput();
			string cmd = string(command);
			// cout << cmd;
			hist.push_back(cmd);

			if(strcmp(command , "history") == 0)
				print_history();
			if(strcmp(command , "$$") == 0)
				cout << ppid << endl;

			vector<char*> parameters = split_string(command);
			

			bool in = false , out = false , apend = false;
			char* a;
			char *o;

			char** par = new char*[parameters.size()+1];

    		for ( int i = 0; i < parameters.size(); i++ ){
      			par[i] = parameters[i];
      			if(strcmp(parameters[i] , "|") == 0)
      				pipecount++;
      		}
      		// cout << pipecount;
      		par[parameters.size()] = NULL;

      		for ( int i = 0; i < parameters.size(); i++ ){
      			// cout << par[i+1] << " ";
      			if(strcmp(par[i] ,">") == 0){
      				out = true;
      				o = par[i+1];
      				// cout << o;
      				par[i] = NULL;
      				//par[i+1] = NULL;
      			 }

      			else if(strcmp(par[i] ,">>") == 0){
      				apend = true;
      				a = par[i+1];
      				par[i] = NULL;
      			}
    		}


    		if(pipecount > 0){
    			// cout<< "hi";
      			pipe_fork(par , parameters.size());
      			// cout << "bye";
      		}
			
			if(strcmp(command , "exit") == 0)
				return 0;
			else if (strcmp(par[0] , "cd") == 0){
				if(change_dir(par[1]) == -1)
					cout << "No such directory exists!";
			}

			else
				my_fork(par , out , apend , o , a);
          }
}

