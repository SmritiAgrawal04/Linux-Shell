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

void view_mydir();
void check_input(char[] , pid_t);
vector<char*> split_string(char[]);
void print_history();
void my_alarm(int time);
int change_dir(const char*);