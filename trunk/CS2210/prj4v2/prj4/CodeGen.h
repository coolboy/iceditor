#pragma once

#include <string>
#include <deque>
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

	typedef std::deque<StackObject> PostStack;

private:
	std::string asmOut;
	tree tgtAST;
	//internal stack : post order tree travel
	PostStack postStack;

private:
	void postOrderTravel(tree root);
	int parseNode(tree node);

	void generateCode();
	std::string generateMethodCode();
};
