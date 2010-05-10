#pragma once

#include <string>

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
	//internal stack
	//back order tree travel

private:
	static void postOrderTravel(tree root);
};
