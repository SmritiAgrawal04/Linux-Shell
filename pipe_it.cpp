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

void pipe_fork(char** par , int len ,int ptr , int diff , int pipecount){
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
			cout<<"i = "<<i<<endl;
			dup2(des , 0);
			if(i+1 < cmd_count)
				dup2(fd[1] , STDOUT_FILENO);
			close(fd[0]);
			// close(fd[1]);

			while(ptr < len && strcmp(par[ptr]  , "|") != 0 )
				ptr++;
			ptr++;
			// cout << "ptr = " << ptr << endl;;

			char** abcd = new char*[ptr-diff];
			int temp = diff;
			// cout << "temp = " << temp << endl;
			int k;
			for(k = 0; k < ptr-diff-1 ; k++){
				abcd[k] = par[temp];
				temp++;
			}
			abcd[k] = NULL;
			diff = ptr - diff;
			

			cout << "diff = "  << diff << endl;
			cout<< "abcd[0] = " << abcd[0] << endl;
			cout<< "abcd[1] = " << abcd[1] << endl;
			cout<< "abcd[2] = " << abcd[2] << endl;
			

			execvp(abcd[0] , abcd);


		}
		else{
			cout<<"parent"<<endl;
			wait(NULL);
			des = fd[0];
			close(fd[1]);
		}
	}
	
}