#pragma once

#include "QueryTree.h"

namespace client{

class TreeOptimizer
{
public:
	TreeOptimizer(void);
	~TreeOptimizer(void);

	QueryTreeNodePtr optimize(QueryTreeNodePtr node);

private:
	void step1();
	void step2();
	void step3();
	void step4();
	void step5();
	void step6();

private:
	QueryTreeNodePtr root;
};
};
