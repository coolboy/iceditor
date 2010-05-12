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
		symbolTableId = -1;
		//stringTableId = -1;
	}
	~StackObject(void){}

public:
	static StackObject fromNode(tree node);

	std::string nodeType;
	std::string lexVal;

	int symbolTableId;
	//int stringTableId;//

};

