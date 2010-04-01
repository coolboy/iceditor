#include "StdAfx.h"
#include "TreeOptimizer.h"

#include <vector>

#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include "ConditionTokenizer.h"

namespace client{

typedef std::vector<std::pair<QueryTreeNodePtr, QueryTreeNodePtr>> NodePairs;

TreeOptimizer::TreeOptimizer(void){}

TreeOptimizer::~TreeOptimizer(void){}

QueryTreeNodePtr TreeOptimizer::optimize( QueryTreeNodePtr node )
{
	root = node->clone();

	step1();
	step2();
	step3();
	step4();
	step5();
	step6();

	return root;
}

//////////////////////////////////////////////////////////////////////////
bool FillInSelect(int /*id*/,
									 const QueryTreeNodePtr /*parent*/, 
									 const QueryTreeNodePtr node)
{
	if (!node)
		return true;

	if (node->getType() != SELECT)
		return true;

	std::string text = boost::get<std::string>(node->getAttr());
	ConditionTokenizer ct(text);
	Conds conds = ct.getCons();
	node->setExInfo(std::string ("SELECT"), conds);

	return true;
}
//////////////////////////////////////////////////////////////////////////

void TreeOptimizer::step1()
{//fill in select exp
	NodeCallBack cb = boost::BOOST_BIND(FillInSelect, _1, _2, _3);
	ForEachNode(root, cb);
}

//////////////////////////////////////////////////////////////////////////
bool GetMergeNodes(int /*id*/,
									 const QueryTreeNodePtr parent, 
									 const QueryTreeNodePtr node,
									 NodePairs& np)
{
	if (!parent || !node)
		return true;

	if (parent->getType() != SELECT || node->getType() != PRODUCT)
		return true;

	np.push_back(std::make_pair(parent, node));

	return true;
}
//////////////////////////////////////////////////////////////////////////

void TreeOptimizer::step2()
{//join product with select
	NodePairs pairs;
	NodeCallBack cb = boost::BOOST_BIND(GetMergeNodes, _1, _2, _3, boost::ref(pairs));
	ForEachNode(root, cb);

	BOOST_FOREACH(NodePairs::value_type val , pairs)
	{
		val.first->setType(JOIN);
		val.first->children = val.second->children;
	}
}

void TreeOptimizer::step3()
{//push select down

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