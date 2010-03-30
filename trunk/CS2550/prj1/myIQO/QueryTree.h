#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>

#include <vector>
#include <string>
#include <map>

namespace client{
//////////////////////////////////////////////////////////////////////////
// Typedefs
//////////////////////////////////////////////////////////////////////////
typedef std::vector<std::string> StringVec;
typedef std::vector<int> IntVec;

//////////////////////////////////////////////////////////////////////////
// enum for node type
//////////////////////////////////////////////////////////////////////////
enum NodeType{
	UNDEF,
	SCAN,
	INDEX_SCAN,
	HASH_SCAN,
	SELECT,
	JOIN,
	UNION,
	PRODUCT,
	PROJECT
};

//////////////////////////////////////////////////////////////////////////
// Query tree node
//////////////////////////////////////////////////////////////////////////

class QueryTreeNode{
public:
	typedef boost::variant<std::string, StringVec> Attribute;
	typedef boost::shared_ptr<QueryTreeNode> QueryTreeNodePtr;
	typedef std::map<int, QueryTreeNodePtr > Children;

	QueryTreeNode();

	NodeType getType() const;
	void setType(NodeType val);

	Attribute getAttr() const;
	void setAttr(const Attribute& val);

	bool hasChild();
	bool hasChild(int id);

	QueryTreeNodePtr getChild(int id);
	bool setChild(int id, QueryTreeNodePtr node, bool bFailOnExist = false);

	friend bool SwapNode(const QueryTreeNodePtr root, const IntVec& lv1, const IntVec& lv2);
	friend void PrintTree(const QueryTreeNodePtr root, int depth);
	friend bool AppendNode(const QueryTreeNodePtr root, QueryTreeNodePtr node, const IntVec& lv1);
	friend bool RemoveNode(const QueryTreeNodePtr root, const IntVec& lv1);

private:
	NodeType ty;//UNDEF for error
	Attribute attr;
	Children children;//map (id,ptr)
};
typedef QueryTreeNode::QueryTreeNodePtr QueryTreeNodePtr;
typedef std::vector<QueryTreeNodePtr> QueryTreeNodePtrs;

QueryTreeNodePtrs ParseQueryTree(const std::string& text);

bool SwapNode(const QueryTreeNodePtr root, const IntVec& lv1, const IntVec& lv2);

bool RemoveNode(const QueryTreeNodePtr root, const IntVec& lv1);

bool InsertNode(const QueryTreeNodePtr root, QueryTreeNodePtr node, const IntVec& lv1);

bool AppendNode(const QueryTreeNodePtr root, QueryTreeNodePtr node, const IntVec& lv1);

void PrintTree(const QueryTreeNodePtr root, int depth = 0);

};