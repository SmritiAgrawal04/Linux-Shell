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

vector<char*> split_pipe(char* command){
	vector<char*> parameters;
	char* temp = strtok(command, "|");
	while(temp != NULL){
	parameters.push_back(temp);
	temp = strtok(NULL, "|");
	}
	return parameters;
}

vector<char*> split_spacestring(char* command){
	vector<char*> parameters;
	char* temp = strtok(command, " ");
	while(temp != NULL){
	parameters.push_back(temp);
	temp = strtok(NULL, " ");
	}
	return parameters;
}

void pipe_fork(char command[] , int len ,int ptr , int diff , int pipecount, int* stat){
	// cout << "in pipe";
	vector<char*> param = split_pipe(command);
	for(int i = 0; i < param.size();i++){
		cout << param[i] << endl;
	}
	int fd[2];
	int des = 0;
	// cout << "pipecount =" << pipecount;
	// cout << "len = " << len << endl;
	int cmd_count = pipecount + 1;
	//cout<<"cmd_count"<<cmd_count;

	for(int i = 0; i< cmd_count ; i++){

		pipe(fd);
		pid_t childpid = fork();
		if(childpid < 0)
			perror("Fork not possible.");
		else if(childpid == 0){
			vector<char*> tempo = split_spacestring(param[i]);
			// cout<<"i = "<<i<<endl;

			char** pari = new char*[tempo.size()+1];

    	for( int i = 0; i < tempo.size(); i++ )
      		pari[i] = tempo[i];
      			

      	pari[tempo.size()] = NULL;

			dup2(des , 0);
			if(i+1 < cmd_count)
				dup2(fd[1] , STDOUT_FILENO);
			close(fd[0]);
			close(fd[1]);
			// cout<<"before ptr "<<ptr<<endl;
			/*while(ptr < len && strcmp(par[ptr]  , "|") != 0 )
				ptr++;
			ptr++;
			cout << "ptr = " << ptr << endl;;

			char** abcd = new char*[ptr-diff];
			int temp = diff;
			cout << "size =" <<ptr-diff;
			// cout << "temp = " << temp << endl;
			int k;
			for(k = 0; k < ptr-diff-1 ; k++){
				abcd[k] = par[temp];
				temp++;
			}
			abcd[k] = NULL;
			diff = ptr - diff;
			const;

			cout << "diff = "  << diff << endl;
			cout<< "abcd[0] = " << abcd[0] << endl;
			cout<< "abcd[1] = " << abcd[1] << endl;
			// cout<< "abcd[2] = " << abcd[2] << endl;
			cout<<"abcd: "<<*abcd<<endl;*/

			execvp(pari[0] , pari);


		}
		else{
			// cout<<"parent"<<endl;
			int status;
				wait(&status);
				*stat = WEXITSTATUS(status);
			des = fd[0];
			close(fd[1]);
		}
	}
	
}