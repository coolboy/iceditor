#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>

#include <vector>
#include <string>
#include <map>

namespace fu = boost::fusion;

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

	Children children;//map (id,ptr)

private:
	NodeType ty;//UNDEF for error
	Attribute attr;
};

//////////////////////////////////////////////////////////////////////////
// Typedef for nodes
//////////////////////////////////////////////////////////////////////////

typedef boost::shared_ptr<QueryTreeNode> QueryTreeNodePtr;
typedef boost::fusion::vector2<NodeType,StringVec> NodeType1;
typedef boost::fusion::vector2<NodeType,std::string> NodeType2;
typedef NodeType NodeType3;
typedef boost::variant <NodeType1, NodeType2, NodeType3> NodeVar;
typedef boost::fusion::vector2< IntVec, NodeVar> SubNode;

//////////////////////////////////////////////////////////////////////////
// Node variant visitor
//////////////////////////////////////////////////////////////////////////

class node_visitor : public boost::static_visitor<QueryTreeNode>
{
public:
	QueryTreeNode operator()(const NodeType1& node) const;

	QueryTreeNode operator()(const NodeType2& node) const;

	QueryTreeNode operator()(const NodeType3& node) const;
};

class TreeBuilder{
public:
	QueryTreeNodePtr getRoot();

	// Root node call back
	void onRoot(NodeVar& val);

	// Normal node call back
	void onNode(SubNode& val);

private:
	QueryTreeNodePtr root;
};

QueryTreeNodePtr ParseQueryTree(std::string text);