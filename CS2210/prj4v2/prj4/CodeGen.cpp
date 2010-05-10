#include "CodeGen.h"

#include <stack>
#include <iostream>

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
			//cout << "visit: " << cur->data << endl;
			printnode(cur);
			cout<<endl;
			st.pop();
			pre = cur;
			cur = NULL;    
		}
		else
		{
			cur = cur->RightC; 
		}
	}//end of while(p || st.size()!=0)
}
