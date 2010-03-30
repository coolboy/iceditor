#include "StdAfx.h"
#include "TreeOptimizer.h"

namespace client{
TreeOptimizer::TreeOptimizer(void)
{
}

TreeOptimizer::~TreeOptimizer(void)
{
}

QueryTreeNodePtr TreeOptimizer::optimize( QueryTreeNodePtr node )
{
	step1();
	step2();
	step3();
	step4();
	step5();
	step6();

	return root;
}

void TreeOptimizer::step1()
{

}

void TreeOptimizer::step2()
{

}

void TreeOptimizer::step3()
{

}

void TreeOptimizer::step4()
{

}

void TreeOptimizer::step5()
{

}

void TreeOptimizer::step6()
{

}
};