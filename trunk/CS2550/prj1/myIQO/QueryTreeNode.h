#pragma once

class QueryTreeNode
{
public:
	typedef boost::shared_ptr<QueryTreeNode> QueryTreeNodePtr;
	QueryTreeNode(void);
	~QueryTreeNode(void);

	static QueryTreeNodePtr createTreeFromRaw(const char*);
	static void swapNode(QueryTreeNodePtr node1, QueryTreeNodePtr node2);
};
