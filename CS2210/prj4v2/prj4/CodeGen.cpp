#include "CodeGen.h"

//debug
#include <iostream>
//
#include <map>
#include <vector>
#include <cassert>

#include "RegisterAllocator.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
// For the tree view
// left is the buttom
// right is the up


//////////////////////////////////////////////////////////////////////////
bool IsComma(tree node);

bool IsClassBody(tree root){
	if (root->RightC == 0)
		return false;

	auto so = StackObject::fromNode(root->RightC);
	return so.nodeType == "MethodOp";
}

bool IsDeclsBody(tree root){
	if (root->RightC == 0)
		return false;

	auto so = StackObject::fromNode(root->RightC);
	return so.nodeType == "DeclOp";
}

bool IsFieldDecls(tree root){
	if (root->RightC == 0)
		return false;

	return IsComma(root->RightC);
}

bool IsDummy(tree node){
	auto obj = StackObject::fromNode(node);

	return obj.nodeType == "DUMMYNode";
}

bool IsClass(tree node){
	auto obj = StackObject::fromNode(node);

	return obj.nodeType == "ClassOp";
}

bool IsComma(tree node){
	auto obj = StackObject::fromNode(node);

	return obj.nodeType == "CommaOp";
}

bool IsLTnum(tree node){
	auto obj = StackObject::fromNode(node);

	return obj.nodeType == "LTnum";
}

bool IsAddOp(tree node){
	auto obj = StackObject::fromNode(node);

	return obj.nodeType == "AddOp";
}

bool IsMethodOp(tree node){
	auto obj = StackObject::fromNode(node);

	return obj.nodeType == "MethodOp";
}

bool IsXXXOp(tree node, const char* opstr){
	auto obj = StackObject::fromNode(node);

	return obj.nodeType == opstr;
}

bool IsTerm(tree node){
	auto obj = StackObject::fromNode(node);

	return obj.nodeType == "UnaryNegOp";
}
//////////////////////////////////////////////////////////////////////////

CodeGen::CodeGen(void)
{
	asmOut = ".data\n"
		"Enter:	.asciiz \"\n"
		"\"\n"
		"base:\n"
		".text\n"
		".globl main\n";
}


CodeGen::~CodeGen(void)
{
}

void CodeGen::setAST( tree pAST )
{
	tgtAST = pAST;
}

std::string CodeGen::getMIPSCode()
{
	generateCode();
	return asmOut;
}


void CodeGen::generateCode()
{
	dealProgramOp(tgtAST);
}

void CodeGen::dealProgramOp( tree programRoot )
{
	printnode(programRoot);

	//left is ClassOp
	dealClassOp(programRoot->LeftC);

	//right is IDNode
	// TODO: where is the program ID?
	printnode(programRoot->RightC);
}

void CodeGen::dealClassOp( tree classRoot )
{
	printnode(classRoot);

	//left
	// 1. Another ClassOp
	// 2. DUMMYNode

	if (IsDummy(classRoot->LeftC))
		;//encounter the end of the ClassOp
	else if (IsClass(classRoot->LeftC))
		dealClassOp(classRoot->LeftC);
	else
		assert(false);

	//right
	// 1. ClassDefOp
	dealClassDefOp(classRoot->RightC);
}

void CodeGen::dealClassDefOp( tree classDefRoot )
{
	printnode(classDefRoot);

	//left
	// 1. DUMMYNode
	// 2. ClassBody -> BodyOp

	if (IsDummy(classDefRoot->LeftC))
		;//encounter the end of the ClassDefOp
	else if (IsClassBody(classDefRoot->LeftC))
		dealClassBodyOp(classDefRoot->LeftC);
	else
		assert(false);

	//right
	// 1. IDNode
	printnode(classDefRoot->RightC);

}

void CodeGen::dealClassBodyOp( tree classBodyRoot )
{
	printnode(classBodyRoot);

	//left
	// 1. Another CLassBodyOp -> BodyOp
	// 2. Subtree For Decls
	// 3. Dummy when the class is empty

	if (IsClassBody(classBodyRoot->LeftC))
		dealClassBodyOp(classBodyRoot->LeftC);//class body
	else if (IsDeclsBody(classBodyRoot->LeftC))
		dealDeclsBodyOp(classBodyRoot->LeftC);//decls
	else if (IsDummy(classBodyRoot->LeftC))
		;//Will be dummy when this class is empty
	else
		assert(false);

	//right
	// 1. Subtree For MethodDecl
	dealMethodDeclOp(classBodyRoot->RightC);
}

void CodeGen::dealDeclsBodyOp( tree declsBodyRoot )
{
	printnode(declsBodyRoot);

	//left
	// 1. Dummy
	// 2. DeclsBodyOp

	if (IsDummy(declsBodyRoot->LeftC))
		;
	else if (IsDeclsBody(declsBodyRoot->LeftC))
		dealDeclsBodyOp(declsBodyRoot->LeftC);
	else
		assert (false);

	//right
	// 1. FieldDecl
	dealFieldDeclOp(declsBodyRoot->RightC);
}

void CodeGen::dealFieldDeclOp( tree fieldDeclRoot )
{
	printnode(fieldDeclRoot);

	//left
	// 1. FieldDecl
	// 2. Dummy
	if (IsDummy(fieldDeclRoot->LeftC))
		;
	else if (IsFieldDecls(fieldDeclRoot->LeftC))
		dealFieldDeclOp(fieldDeclRoot->LeftC);
	else
		assert (false);

	//right
	// 1. Tree for one var
	dealOneVar(fieldDeclRoot->RightC);
}

void CodeGen::dealOneVar( tree oneVarRoot )
{
	printnode(oneVarRoot);

	//left
	// 1. Tree for one decl id // Is IDNode

	printnode(oneVarRoot->LeftC);

	//right
	// 1. CommaOp
	// 1.left //Tree for type
	// 1.right //Tree for varinit
	assert (IsComma(oneVarRoot->RightC));

	tree rightRoot = oneVarRoot->RightC;
	dealVariInit(rightRoot->LeftC);
	dealType(rightRoot->RightC);
}

void CodeGen::dealVariInit( tree variInitRoot )
{
	printnode(variInitRoot);

	// 1. Expression
	// 2. Array Initializer //TODO
	// 3. ArrayCreationExpression //TODO

	assert (IsLTnum(variInitRoot));

	//left
	// 1. Tree for SimpleExpr
	dealSimpleExpr(variInitRoot->LeftC);

	//right
	// 1. Tree for SimpleExpr
	dealSimpleExpr(variInitRoot->RightC);

}

void CodeGen::dealSimpleExpr( tree simpleExprRoot )
{
	printnode(simpleExprRoot);

	//left
	// 1. AddOp
	// 2. Subtree for Factor/Term
	if (IsAddOp(simpleExprRoot->LeftC))
		dealSimpleExpr(simpleExprRoot->LeftC);
	else if (IsTerm(simpleExprRoot->LeftC))//term
		dealTerm(simpleExprRoot->LeftC);
	else //if (IsFactor())//factor
		dealFactor(simpleExprRoot->LeftC);
	//else
		//assert (false);

	//right
	// 1. Subtree for Factor/Term

}

void CodeGen::dealTerm( tree termRoot )
{
	printnode(termRoot);

	//left

	//right

}

void CodeGen::dealFactor( tree factorRoot )
{
	printnode(factorRoot);

	//left

	//right

}

void CodeGen::dealType( tree typeRoot )
{
	printnode(typeRoot);

	//left

	//right

}

void CodeGen::dealMethodDeclOp( tree methodDeclRoot )
{
	printnode(methodDeclRoot);

	assert (IsMethodOp(methodDeclRoot));

	//left
	// 1. HeadOp
	// 1. left IDNode
	// 1. right Subtree 4 Parameter

	tree leftRoot= methodDeclRoot->LeftC;

	assert (IsXXXOp(leftRoot, "HeadOp"));

	printnode(leftRoot->LeftC);

	//right
	// 1. Subtree 4 block
	dealBlock(methodDeclRoot->RightC);
}

void CodeGen::dealParameter( tree paraRoot )
{
	printnode(paraRoot);

	//left

	//right

}

void CodeGen::dealBlock( tree blockRoot )
{
	printnode(blockRoot);

	//left

	//right

}