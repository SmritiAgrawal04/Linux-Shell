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

vector<char*> split_endline(char waste_array[2000]){
		vector<char*> lines;
		int i =0 ;
		//cout<<waste_array;
		char* s = strtok(waste_array , "10");
		while(i < 5){
		//	cout<<s;
			lines.push_back(s);
			s = strtok(NULL , "10");
			i++;
		}
		return lines;
	}

vector<char*> split_colon(char waste_array[2000]){
		vector<char*> lines;
		int i =0 ;
		//cout<<waste_array;
		char* s = strtok(waste_array , ":");
		while(i < 7){
		//	cout<<s;
			lines.push_back(s);
			s = strtok(NULL , ":");
			i++;
		}
		return lines;
	}

void bash(){
	string host , path;
	// cout << "in bash";
	ifstream fd_host;
	fd_host.open("/etc/hostname" , ifstream::in);
	 if(fd_host){
		getline(fd_host ,host);
		// fd_host.close();
	 }
	 fd_host.close();
	// cout << host << endl;

	ifstream fd_path;
	fd_path.open("/etc/environment" , ifstream::in);
	if(fd_path){
		getline(fd_path , path);
		// fd_path.close();
	}
	fd_path.close();
	// cout << path << endl;

	ifstream fd;
	string user_name, home, waste;
	fd.open("/etc/passwd" , ifstream::in);
	for(int i = 1 ; i < 42 ; i++){
		getline(fd , waste);
		  //cout<<waste;
		}
     // cout<<waste;
     
	 char waste_array[2000];
	 int i;
	for(i = 0 ; i < waste.length() ; i++)
		waste_array[i] = waste[i];
	waste_array[i] = '\0';

	vector<char*> lines = split_colon(waste_array);
	/*for(int i = 0; i < lines.size() ; i++){
		cout << lines[i] << endl;
	}*/
	fd.close();

	int fd_bash = open("bash_rc.txt" , O_CREAT | O_WRONLY | O_APPEND , 0644);
	if(fd_bash < 0){
		perror("ERROR");
		return;
	}
	int savep=dup(1);
	dup2(fd_bash , 1);
	cout << "$" << endl;
	cout << host << endl;
	cout << path << endl;
	cout << lines[5] << endl;
	cout << lines[0] << endl;

    dup2(savep , 1);
    close(fd_bash);
	return;

}