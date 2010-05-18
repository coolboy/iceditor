#include "CodeGen.h"

//debug
#include <iostream>
//
#include <map>
#include <vector>

#include "RegisterAllocator.h"

using namespace std;

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
	postOrderTravel(tgtAST);
	return asmOut;
}

void CodeGen::postOrderTravel( tree root)
{
	std::stack<tree> st;//定义栈，节点类型为TreeNode
	tree cur = root;
	tree pre = NULL;//pre表示最近一次访问的结点

	while(cur || st.size()!=0)
	{
		//沿着左孩子方向走到最左下 。
		while(cur)
		{
			st.push(cur);
			cur = cur->LeftC;
		}
		//get the top element of the stack
		cur = st.top();
		//如果p没有右孩子或者其右孩子刚刚被访问过
		if(cur->RightC == NULL || cur->RightC == pre)
		{
			//visit this element and then pop it
			parseNode(cur);
			st.pop();
			pre = cur;
			cur = NULL;    
		}
		else
		{
			cur = cur->RightC; 
		}
	}//end of while(p || st.size()!=0)

	//generate code from the stack
	generateCode();
}

int CodeGen::parseNode( tree node )
{
	if (IsNull(node))
		return -1;

	StackObject so = StackObject::fromNode(node);
	postStack.push_back(so);

	//debug
	printnode(node);
	cout<<endl;
	//

	return 0;
}

class VarInfo{
public:
	VarInfo(){
		initVal = 0;
	}
	string regName;
	//string type;//only int
	int initVal;
};

std::string CodeGen::generateMethodCode()
{
	//using r16-r23 for stack val
	RegisterAllocator ralloc;

	map<string, VarInfo> stackRegs;//name -> reg
	//using stack for temp val
	std::string retAsm = "\tla	$28	base		#store global area address into $gp\n"
		"\tmove	$t1	$28		#init base\n"
		"\tadd	$t1	$t1	0	#init main access link in $t1; .data 0\n"
		"\tli	$t2	0		#init $t2 as 0-global access\n"
		"\tmove	$fp	$sp		#init fp pointer\n"
		"\tsw	$ra	0($sp)		#save return address on stack\n"
		"\taddi	$sp	$sp	-4	#increase st\n";

	auto& so = postStack.front();
	std::string methodName, className/*Not used by main*/;

	//main...
	if (so.lexVal == "main"){
		methodName = so.lexVal;
	}else{//normal...
		methodName = so.lexVal;
	}

	postStack.pop_front();

	while (true){ //parse the data
		auto& so = postStack.front();
		//encounter the end condition
		if (so.nodeType == "STNode" && (so.symbolType == "procedure" || so.symbolType == "class")){
			//if (so.symbolType == "procedure")
				//retAsm += generateMethodCode();
			//else if (so.symbolType == "class")
				//className = so.lexVal;
			break;//when the method code is done
		}else{

		}

		postStack.pop_front();
	}

	//generateCode
	return retAsm;
}

void CodeGen::generateCode()
{
	//while (postStack.empty() == false){
		auto& so = postStack.front();
		if (so.symbolType == "procedure")
			asmOut += generateMethodCode();
		//else if (so.symbolType == "")
			//;
	//}
}
