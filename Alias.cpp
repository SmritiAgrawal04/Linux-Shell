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
#include <unordered_map> 
#include<fstream>
#include<stack>
#include<cstdlib>

using namespace std;

unordered_map<string, string> umap;

void call_my_shell(){
		// clear();
        cout << "\t\t\t\tSMRITI'S SHELL\n\n\n" ;
        // cout << "\t\tUSE AT YOUR OWN RISK\n\n";
        // sleep(2);
} 

void my_temp_fork(char ali[]){
	vector<char*> arguments;
	char* temp = strtok(ali , " ");
	while(temp != NULL){
		arguments.push_back(temp);
		temp = strtok(NULL," ");
	}

	char** arg = new char*[arguments.size()+1];
	for(int i = 0 ;i < arguments.size();i++)
		arg[i] = arguments[i];

	arg[arguments.size()] = NULL;

	pid_t childpid = fork();
	if(childpid < 0)
		perror("Fork not possible");

	else if(childpid == 0){
		// cout << "var = " << var;
		// cout << "arg = " << arg;
		execvp(arg[0] , arg);
	}
	else
		wait(NULL);

}

int main(){
	cout << "\033[H\033[J";
	call_my_shell();
	char command[256];
	string dummy;
	while(1){
		cout<< "$";
		
        cin.getline(command , 256);
        bool flag = false;

        vector<char*> parameters;
			char* temp = strtok(command, " ");
			while(temp != NULL){
				parameters.push_back(temp);
				temp = strtok(NULL, " ");
			}

			char** par = new char*[parameters.size()+1];
    		for ( int i = 0; i < parameters.size(); i++ ){
      			par[i] = parameters[i];
      		}

      		par[parameters.size()] = NULL;

      		if(strcmp(par[0] , "aliase") == 0){
      			// cout<<"in aliase";
      			if(par[3] == NULL)
      				umap.insert(make_pair(par[1], par[2])); 
      			else{
      				strcat(par[2] , " ");
      				// par[2] = par[2] + par[3];
      				strcat(par[2] , par[3]);
      				umap.insert(make_pair(par[1], par[2]));
      			}

      		}

      		/*unordered_map<string, string>:: iterator itr; 
	for (itr = umap.begin(); itr != umap.end(); itr++) 
    { 
        cout << itr->first << "  " << itr->second << endl; 
     } */

      		unordered_map<string, string>:: iterator itr;
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
     			/*for(int i = 0; i < dummy.length() ; i++){
     				ali[i] = dummy[i];
     			}*/
     			strcpy(ali , dummy.c_str());
     			// cout << ali;
     			// ali = const_cast<char*>(dummy.ali_str());
     			 my_temp_fork(ali);
     		}

      		
	}
	


	return 0;
}