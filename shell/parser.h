#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
using namespace std;

struct cmds {
	vector<string> cmd_vector;
	bool ampersand;
	bool greater;
	bool less;
};


cmds parse(string cmds);	

#endif


