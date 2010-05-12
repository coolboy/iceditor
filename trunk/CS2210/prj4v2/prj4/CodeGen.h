#pragma once

#include <string>
#include <stack>

#include "StackObject.h"

extern "C"{
#include "proj2.h"
};

class CodeGen
{
public:
	CodeGen(void);
	~CodeGen(void);

	void setAST(tree pAST);
	std::string getMIPSCode();

private:
	std::string asmOut;
	tree tgtAST;
	//internal stack : post order tree travel
	typedef std::stack<StackObject> PostStack;
	PostStack postStack;

private:
	void postOrderTravel(tree root);
	int parseNode(tree node);
};
