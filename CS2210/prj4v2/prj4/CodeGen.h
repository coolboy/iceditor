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

private:
	int parseNode(tree node);

	void generateCode();

private:
	static void dealProgramOp(tree programRoot);
	static void dealClassOp(tree classRoot);
	static void dealClassDefOp(tree classDefRoot);
	static void dealClassBodyOp(tree classBodyRoot);
	static void dealDeclsBodyOp(tree declsBodyRoot);
	static void dealMethodDeclOp(tree methodDeclRoot);
	static void dealFieldDeclOp( tree fieldDeclRoot );
	static void dealOneVar( tree oneVarRoot );
	static void dealVariInit( tree variInitRoot );
	static void dealType( tree typeRoot );
	static void dealSimpleExpr( tree simpleExprRoot );
	static void dealTerm( tree termRoot );
	static void dealFactor( tree factorRoot );
	static void dealParameter( tree paraRoot );
	static void dealBlock( tree blockRoot );
};