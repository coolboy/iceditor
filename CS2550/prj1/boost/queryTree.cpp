#include "stdafx.h"

#include <boost/config/warning_disable.hpp>

#include <boost/spirit/include/qi.hpp>

#include <boost/fusion/include/at_c.hpp>

#include <boost/shared_ptr.hpp>

#include <boost/variant.hpp>

#include <boost/bind.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <set>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
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
// Query tree node
//////////////////////////////////////////////////////////////////////////

class QueryTreeNode{
public:
	typedef boost::variant<std::string, StringVec> Attribute;
	typedef std::set<boost::shared_ptr<QueryTreeNode> > Children;
	QueryTreeNode():id(-1),ty(UNDEF){}

	int getId() const { return id; }
	void setId(int val) { id = val; }

	NodeType getType() const { return ty; }
	void setType(NodeType val) { ty = val; }

	Attribute getAttr() const { return attr; }
	void setAttr(const Attribute& val) { attr = val; }

	Children& getChildren() { return children; }
	void setChildren(const Children& val) { children = val; }

private:
	int id;//0 stands for root -1 stands for error
	NodeType ty;//UNDEF for error
	Attribute attr;
	//
	Children children;
};

//////////////////////////////////////////////////////////////////////////
// Typedef for nodes
//////////////////////////////////////////////////////////////////////////

typedef boost::fusion::vector2<NodeType,StringVec> NodeType1;
typedef boost::fusion::vector2<NodeType,std::string> NodeType2;
typedef NodeType NodeType3;
typedef boost::variant <NodeType1, NodeType2, NodeType3> NodeVar;

//////////////////////////////////////////////////////////////////////////
// Root node call back
//////////////////////////////////////////////////////////////////////////

class node_visitor : public boost::static_visitor<QueryTreeNode>
{
public:
	QueryTreeNode operator()(const NodeType1& node) const
	{
		QueryTreeNode qtn;
		qtn.setType (fu::at_c<0>(node));
		qtn.setAttr (fu::at_c<1>(node));
		return qtn;
	}

	QueryTreeNode operator()(const NodeType2& node) const
	{
		QueryTreeNode qtn;
		qtn.setType (fu::at_c<0>(node));
		qtn.setAttr (fu::at_c<1>(node));
		return qtn;
	}

	QueryTreeNode operator()(const NodeType3& node) const
	{
		QueryTreeNode qtn;
		qtn.setType(node);
		return qtn;
	}
};

void onRoot(NodeVar& val)
{
	QueryTreeNode root = boost::apply_visitor( node_visitor(), val );
}

//////////////////////////////////////////////////////////////////////////
// Normal node call back
//////////////////////////////////////////////////////////////////////////
typedef boost::fusion::vector2< IntVec, NodeVar> SubNode;

void onNode(SubNode& val)
{
	IntVec lvl = fu::at_c<0>(val);
	QueryTreeNode node = boost::apply_visitor( node_visitor(), fu::at_c<1>(val) );
}

int main()
{
	using qi::int_;
	using qi::parse;
	using qi::lit;

	using ascii::space;

	std::string text = "UNION"
		"\t1 PROJECT([FirstName, MiddleName, LastName])"
		"\t\t1,1 SCAN(DOCTOR)"
		"\t2 PROJECT([FirstName, MiddleName, LastName])"
		"\t\t2,1 SELECT(PATIENT.SSN=PRESCRIPTION.Patient)";
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

	BOOST_AUTO(start, begRule[onRoot] >> * expRule[onNode] );

	bool ret = true;
	while (first != last && ret == true)
	{
		ret = qi::phrase_parse(first, last, start, space);
	}

	return 0;
}
