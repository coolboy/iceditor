#include "StdAfx.h"
#include "QueryTree.h"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/bind.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

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

QueryTreeNodePtr ParseQueryTree(std::string	text){
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
		conExpRule = *(qi::alnum | '_' | '.' | '>' | '=' | '<');

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
// Tree Nodes
//////////////////////////////////////////////////////////////////////////

QueryTreeNode::QueryTreeNode() :ty(UNDEF)
{

}

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
		parent = parent->children[lvl[depth - 1]]; //array starts at 0

	int nodeId = lvl[lvl.size() - 1];//last

	QueryTreeNode node = boost::apply_visitor( node_visitor(), fu::at_c<1>(val) );
	parent->children[nodeId] = QueryTreeNodePtr (new QueryTreeNode(node));
}