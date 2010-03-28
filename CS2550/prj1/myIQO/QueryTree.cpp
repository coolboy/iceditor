#include "StdAfx.h"
#include "QueryTree.h"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/bind.hpp>

namespace client{
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace fu = boost::fusion;

//////////////////////////////////////////////////////////////////////////
// Typedef for nodes
//////////////////////////////////////////////////////////////////////////

typedef boost::fusion::vector2<NodeType,StringVec> NodeType1;
typedef boost::fusion::vector2<NodeType,std::string> NodeType2;
typedef NodeType NodeType3;
typedef boost::variant <NodeType1, NodeType2, NodeType3> NodeVar;
typedef boost::fusion::vector2< IntVec, NodeVar> SubNode;

//////////////////////////////////////////////////////////////////////////
//Tree build helper
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// Symbol table for keyword
//////////////////////////////////////////////////////////////////////////
struct relation_keywords_ : qi::symbols<char, NodeType>
{
	relation_keywords_()
	{
		add
			("SCAN", SCAN)
			("INDEX_SCAN", INDEX_SCAN)
			("HASH_SCAN", HASH_SCAN);
	}

} relation_keywords;

struct condition_keywords_ : qi::symbols<char, NodeType>
{
	condition_keywords_()
	{
		add
			("SELECT", SELECT)
			("JOIN", JOIN);
	}

} condition_keywords;

struct attr_keywords_ : qi::symbols<char, NodeType>
{
	attr_keywords_()
	{
		add
			("PROJECT"   , PROJECT);
	}

} attr_keywords;

struct null_keywords_ : qi::symbols<char, NodeType>
{
	null_keywords_()
	{
		add
			("UNION"   , UNION)
			("PRODUCT"   , PRODUCT);
	}

} null_keywords;

//////////////////////////////////////////////////////////////////////////
// Covert raw text to Tree
//////////////////////////////////////////////////////////////////////////
QueryTreeNodePtr ParseQueryTree(const std::string& text){
	using qi::int_;
	using qi::parse;
	using qi::lit;

	using ascii::space;

	BOOST_AUTO (first, text.begin());
	BOOST_AUTO (last, text.end());

	BOOST_AUTO(levelsRule, int_ % ',' );

	qi::rule<std::string::const_iterator, std::string(), ascii::space_type>
		stringRule = *(qi::alnum | '_' | '.');

	qi::rule<std::string::const_iterator, std::string(), ascii::space_type>
		conExpRule = *(qi::alnum | '_' | '.' | '\'' | '>' | '=' | '<');

	BOOST_AUTO(attrLstRule, lit("([") >> stringRule % ',' >> lit("])") );
	BOOST_AUTO(relationRule, lit("(") >> stringRule >> lit(")"));
	BOOST_AUTO(conditionRule, lit("(") >> conExpRule >> lit(")"));

	BOOST_AUTO(nodeRule, ( 
		(attr_keywords >> attrLstRule) 
		| (relation_keywords >> relationRule ) 
		| (condition_keywords>> conditionRule) 
		| (null_keywords) 
		) );

	BOOST_AUTO(begRule, nodeRule );
	BOOST_AUTO(expRule, levelsRule >> nodeRule );

	TreeBuilder tb;

	BOOST_AUTO(start,
		begRule[boost::bind(&TreeBuilder::onRoot, &tb, _1)] >> 
		* expRule[boost::bind(&TreeBuilder::onNode, &tb, _1)] 
	);

	bool ret = true;
	while (first != last && ret == true)
	{
		ret = qi::phrase_parse(first, last, start, space);
	}

	return tb.getRoot();
}

//////////////////////////////////////////////////////////////////////////
// Swap node functions
//////////////////////////////////////////////////////////////////////////

struct NodeInfo {
	NodeInfo ():id (-1) {}
	bool isEmpty(){return id == -1;}
	//
	int id;//id for node
	QueryTreeNodePtr node;
	QueryTreeNodePtr parent;
};

NodeInfo getNode (const QueryTreeNodePtr root, const IntVec& levels)
{
	NodeInfo ret;
	if (!root)
		return ret;

	if (levels.size() == 0)
	{
		ret.id = 0;//0 stands for the root
		ret.node = root;
		return ret;
	}

	QueryTreeNodePtr tmpPar = root;
	int index;
	for (index = 0; index != levels.size() - 1; ++index){
		tmpPar = tmpPar->children[index];
	}

	ret.id = levels[levels.size() - 1];//last
	ret.parent = tmpPar;
	ret.node = tmpPar->children[ret.id];

	return ret;
}

bool SwapNode(const QueryTreeNodePtr root, const IntVec& lv1, const IntVec& lv2)
{
	NodeInfo node1 = getNode(root, lv1);
	NodeInfo node2 = getNode(root, lv2);

	if (node1.id == -1 || node2.id == -1)
		return false;

	std::swap(node1.node->children, node2.node->children);
	node1.parent->children[node1.id] = node2.node;
	node2.parent->children[node2.id] = node1.node;

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Print Tree Helper
//////////////////////////////////////////////////////////////////////////
const char* NodeType2Str(NodeType ty)
{
	switch (ty)
	{
	case UNDEF:
		return "UNDEF";

	case SCAN:
		return "SCAN";

	case  INDEX_SCAN:
		return "INDEX_SCAN";

	case HASH_SCAN:
		return "HASH_SCAN";
		
	case SELECT:
		return "SELECT";

	case JOIN:
		return "JOIN";

	case UNION:
		return "UNION";

	case PRODUCT:
		return "PRODUCT";

	case PROJECT:
		return "PROJECT";

	default :
		assert(0);
	}
}

#include <iostream>

void PrintTree( const QueryTreeNodePtr root , int depth)
{//mid left->right
	using namespace std;
	cout<< ' '<< NodeType2Str(root->getType())<< endl;

	if (root->children.size() == 0)
		return;

	BOOST_FOREACH (QueryTreeNode::Children::value_type val,
		root->children){
			int dep = depth + 1;

			while (dep-- != 0)
				cout<<'\t';

			cout<<val.first;
			PrintTree(val.second, depth + 1);
	}
}

//////////////////////////////////////////////////////////////////////////
// Tree Nodes
//////////////////////////////////////////////////////////////////////////

QueryTreeNode::QueryTreeNode() :ty(UNDEF){}

NodeType QueryTreeNode::getType() const
{
	return ty;
}

void QueryTreeNode::setType( NodeType val )
{
	ty = val;
}

QueryTreeNode::Attribute QueryTreeNode::getAttr() const
{
	return attr;
}

void QueryTreeNode::setAttr( const Attribute& val )
{
	attr = val;
}

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

QueryTreeNode node_visitor::operator()( const NodeType1& node ) const
{
	QueryTreeNode qtn;
	qtn.setType (fu::at_c<0>(node));
	qtn.setAttr (fu::at_c<1>(node));
	return qtn;
}

QueryTreeNode node_visitor::operator()( const NodeType2& node ) const
{
	QueryTreeNode qtn;
	qtn.setType (fu::at_c<0>(node));
	qtn.setAttr (fu::at_c<1>(node));
	return qtn;
}

QueryTreeNode node_visitor::operator()( const NodeType3& node ) const
{
	QueryTreeNode qtn;
	qtn.setType(node);
	return qtn;
}

QueryTreeNodePtr TreeBuilder::getRoot()
{
	return root;
}

void TreeBuilder::onRoot( NodeVar& val )
{
	root = QueryTreeNodePtr (new QueryTreeNode(boost::apply_visitor( node_visitor(), val ) ));
}

void TreeBuilder::onNode( SubNode& val )
{
	IntVec lvl = fu::at_c<0>(val);

	int depth = 0;

	QueryTreeNodePtr parent = root;
	while (++depth != lvl.size())
	{
		int childId = lvl.at(depth - 1);//array starts at 0
		parent = parent->children[childId];
	}

	int nodeId = lvl.at(lvl.size() - 1);//last

	QueryTreeNode node = boost::apply_visitor( node_visitor(), fu::at_c<1>(val) );
	parent->children[nodeId] = QueryTreeNodePtr (new QueryTreeNode(node));
}

//////////////////////////////////////////////////////////////////////////
// Insert/Append node functions
//////////////////////////////////////////////////////////////////////////
bool InsertNode( const QueryTreeNodePtr root, QueryTreeNodePtr node, const IntVec& lv1 ){
	NodeInfo curNode = getNode(root, lv1);
	if (curNode.isEmpty())
		return false;

	assert (curNode.parent->children[curNode.id] == curNode.node);

	curNode.parent->children[curNode.id] = node;
	node->children[1] = curNode.node;//insert node at the begining

	return true;
}

bool AppendNode(const QueryTreeNodePtr root, QueryTreeNodePtr node, const IntVec& lv1){
	NodeInfo curNode = getNode(root, lv1);
	if (curNode.isEmpty())
		return false;

	int index = curNode.node->children.size();
	assert (curNode.node->children.find(index + 1) == curNode.node->children.end());
	curNode.node->children[index + 1] = node;

	return true;
}

};