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
#include<unordered_map>
#include<stack>
#include<cstdlib>
#include<termios.h>
#include "fork_it.h"
#include "pipe_it.h"
// #include "AliasD.h"
#include "Bash.h"

using namespace std;

stack<pid_t> processid;
vector<string> hist;
unordered_map<string, string> umap; 
int stat;

void check_input(char [] ,pid_t );


void view_mydir(){
	char mydir[1024];
	getcwd(mydir,sizeof(mydir));
	cout << mydir;
	cout<< "$" << endl;
}

void noncanonical_mode(){
 struct termios term;
 term.c_lflag &= ~(ICANON);
 tcgetattr(1 , &term);
 tcsetattr(1,TCSAFLUSH , &term);
}

/*void signal_handler(int sig_num){
	signal(SIGINT , signal_handler);
	cout << "\n";
	fflush(stdout);
	// exit(0);
	// exit(sig_num);
}*/



int change_dir(const char* path , pid_t ppid){
	if(strcmp(path , "~") == 0){
		char command[] = "echo $HOME"; 
		check_input(command , ppid);
	}
		// path = "/home/smriti/";
		
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

void check_input(char command[256] ,pid_t ppid){
	string dummy;
    bool flag = false;
    int pipecount = 0;
	
    if(strcmp(command , "exit") == 0)
		exit(0);

	if(strcmp(command , "history") == 0){
		stat = 0;
		print_history();
	}

	if(strcmp(command , "$$") == 0){
		stat = 0;
		cout << ppid << endl;
	}

	if(strcmp(command , "$?") == 0){
		stat = 0;
		cout << stat << endl;
	}

	vector<char*> parameters = split_string(command);

	char** par = new char*[parameters.size()+1];

    for ( int i = 0; i < parameters.size(); i++ )
      	par[i] = parameters[i];
      			

      	par[parameters.size()] = NULL;

	if (strcmp(par[0] , "cd") == 0){
		if(change_dir(par[1] , ppid) == -1 && par[1] != "~")
			cout << "No such directory exists!";
		stat = 0;
	}

	if(strcmp(par[0] , "echo") == 0 ){
		stat = 0;
		ifstream des;
		string file;
		vector<string> lines ;
		des.open("bash_rc.txt" , ifstream::in);
		for(int i = 0 ; i < 5 ; i++){
			getline(des , file);
			lines.push_back(file);
		}
	des.close();
		if(strcmp(par[1] , "$PS1") == 0){
			cout << lines[0] << endl;
		}
		else if(strcmp(par[1] , "$HOSTNAME") == 0){
			cout << lines[1] << endl;
		}
		else if(strcmp(par[1] , "$PATH") == 0){
			cout << lines[2] << endl;
		}
		else if(strcmp(par[1] , "$HOME") == 0){
			cout << lines[3] << endl;
		}
		else if(strcmp(par[1] , "$USER") == 0){
			cout << lines[4] << endl;
		}
	}

	/*unordered_map<string, string>:: iterator itr;
      		for (itr = umap.begin(); itr != umap.end(); itr++){ 
        		if(itr->first == par[0]){
        			flag = true; 
        			dummy = itr->second;
        			// cout << dummy;
        			break;
        		}
     		} 

     		char ali[dummy.length() + 1];

     		if(flag){
     			
     			strcpy(ali , dummy.c_str());
     			// cout << ali;
     			// ali = const_cast<char*>(dummy.ali_str());
     			 my_temp_fork(ali ,umap);
     		}
*/
	bool in = false , out = false , apend = false;
	char* a;
	char *o;

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

      			if(strcmp(parameters[i] , "|") == 0){
      				pipecount++;
      			}
      			// cout << pipecount;
    }

    		if(pipecount > 0){
    			// cout<< "hi";
      			pipe_fork(command , parameters.size() , 0 ,0 , pipecount , &stat);
      			// cout << "bye";
      		}
      		
				my_fork(par , out , apend , o , a , &stat);
		}	


int main(){
	noncanonical_mode();
	pid_t ppid = getpid();
	
		cout << "\033[H\033[J";
		// cout << ppid;
        signal(SIGALRM, my_alarm);
        alarm(1000);
        bash();
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
			check_input(command ,ppid);
          }
          return 0;
}