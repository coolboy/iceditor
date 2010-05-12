#include "CodeGen.h"

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
	std::stack<tree> st;//����ջ���ڵ�����ΪTreeNode
	tree cur = root;
	tree pre = NULL;//pre��ʾ���һ�η��ʵĽ��

	while(cur || st.size()!=0)
	{
		//�������ӷ����ߵ������� ��
		while(cur)
		{
			st.push(cur);
			cur = cur->LeftC;
		}
		//get the top element of the stack
		cur = st.top();
		//���pû���Һ��ӻ������Һ��Ӹոձ����ʹ�
		if(cur->RightC == NULL || cur->RightC == pre)
		{
			//visit this element and then pop it
			//cout << "visit: " << cur->data << endl;
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

}

int CodeGen::parseNode( tree node )
{
	if (IsNull(node))
		return -1;

	StackObject so = StackObject::fromNode(node);
	postStack.push(so);

	printnode(node);
	cout<<endl;
	return 0;
}
