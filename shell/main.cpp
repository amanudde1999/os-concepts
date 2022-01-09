/**
    Assignment 1: Processes
	Name: Amro Amanuddein
	StudentID: 1572498
	Course: CMPUT 379 Fall 2021
*/


#include <iostream>
#include <string>
#include "parser.h"
#include "cmdexecuter.h"
#include <signal.h>

using namespace std;

void run(){
	while(true){
		string cmd = "";
		cout << "SHELL 379: ";
		getline(cin, cmd);
		cmds cmd_input = parse(cmd); 
		execute_cmd(cmd_input);

	}
}

int main(){
	run();
}


