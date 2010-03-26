#include "stdafx.h"

#include <boost/config/warning_disable.hpp>

#include <boost/spirit/include/qi.hpp>

#include <boost/shared_ptr.hpp>

#include <boost/variant.hpp>

#include <boost/bind.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <set>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

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

class QueryTreeNode{
public:
	typedef boost::variant<std::string, StringVec> Attribute;
	typedef std::set<boost::shared_ptr<QueryTreeNode> > Children;
	QueryTreeNode():id(-1),ty(UNDEF){}

private:
	int id;//0 stands for root -1 stands for error
	NodeType ty;//UNDEF for error
	Attribute attr;
	//
	Children children;
};

void onlevelLst (IntVec& iVec)
{

}

void onType (NodeType& ty)
{

}

void onAttrLst(const StringVec& attrLst)
{

}

void onRelation(const std::string& rel)
{

}

void onCondition(const std::string& con)
{

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

	BOOST_AUTO(levelsRule, (int_ % ',')[onlevelLst] );

	qi::rule<std::string::const_iterator, std::string(), ascii::space_type>
		stringRule = *(qi::alnum | '_' | '.');

	qi::rule<std::string::const_iterator, std::string(), ascii::space_type>
		conExpRule = *(qi::alnum | '_' | '.' | '>' | '=' | '<');

	BOOST_AUTO(attrLstRule, lit("([") >> (stringRule % ',')[onAttrLst] >> lit("])") );
	BOOST_AUTO(relationRule, lit("(") >> stringRule[onRelation] >> lit(")"));
	BOOST_AUTO(conditionRule, lit("(") >> conExpRule[onCondition] >> lit(")"));

	BOOST_AUTO(nodeRule, ( 
		(attr_keywords[onType] >> attrLstRule) 
		| (relation_keywords[onType] >> relationRule ) 
		| (condition_keywords[onType] >> conditionRule) 
		| (null_keywords[onType]) 
		) );

	BOOST_AUTO(begRule, nodeRule );
	BOOST_AUTO(expRule, levelsRule >> nodeRule );

	BOOST_AUTO(start, begRule >> * expRule );

	qi::phrase_parse(first, last, start, space);

	return 0;
}
