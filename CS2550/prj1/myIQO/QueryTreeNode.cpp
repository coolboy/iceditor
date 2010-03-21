#include "StdAfx.h"
#include "QueryTreeNode.h"

QueryTreeNode::QueryTreeNode(void)
{
}

QueryTreeNode::~QueryTreeNode(void)
{
}

QueryTreeNode::QueryTreeNodePtr QueryTreeNode::createTreeFromRaw( const char* )
{
	return QueryTreeNodePtr(new QueryTreeNode());
}

void QueryTreeNode::swapNode( QueryTreeNodePtr node1, QueryTreeNodePtr node2 )
{

}