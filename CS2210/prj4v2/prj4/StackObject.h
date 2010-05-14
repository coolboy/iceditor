#pragma once

#include <string>

extern "C"{
#include "proj2.h"
#include "proj3.h"
};

class StackObject
{
public:
	StackObject(void){
		preDefined = false;
		argNum = -1;
		nestedLevel = -1;
	}
	~StackObject(void){}

public:
	static StackObject fromNode(tree node);

	std::string nodeType;
	std::string lexVal;

	//symbol attr
	std::string symbolType;
	std::string varType;
	bool preDefined;
	int argNum;
	int nestedLevel;

};

