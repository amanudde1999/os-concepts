/**
    Assignment 1: Processes
	Name: Amro Amanuddein
	StudentID: 1572498
	Course: CMPUT 379 Fall 2021
*/

#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "parser.h"

using namespace std;

cmds parse(string cmd_string){
	cmds cmd_input;
	boost::split(cmd_input.cmd_vector, cmd_string, boost::is_any_of(" "));
	// Check if there's >, < or & 
	cmd_input.ampersand = cmd_input.cmd_vector[cmd_input.cmd_vector.size()-1] == "&";
	cmd_input.greater = cmd_string.find('>') != string::npos;
	cmd_input.less = cmd_string.find('<') != string::npos;
	if (cmd_input.ampersand){
		cmd_input.cmd_vector.pop_back();
	} 
	return cmd_input;
}
