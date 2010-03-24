#include "stdafx.h"

//ref:http://www.gamedev.net/community/forums/topic.asp?topic_id=557809

#include <boost/foreach.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

//////////////////////////////////////////////////////////////////////////
// Typedefs
//////////////////////////////////////////////////////////////////////////
typedef std::vector<std::string> StringVec;
typedef std::vector<int> IntVec;

///////////////////////////////////////////////////////////////////////////
//  Query Tree Node struct
///////////////////////////////////////////////////////////////////////////
enum NodeType{
	SCAN,
	INDEX_SCAN,
	HASH_SCAN,
	SELECT,
	JOIN,
	UNION,
	PRODUCT,
	PROJECT
};

struct TreeNode
{
	IntVec Levels;
	NodeType Type;
	StringVec AttrLst;
};

// We need to tell fusion about our employee struct
// to make it a first-class fusion citizen. This has to
// be in global scope.

BOOST_FUSION_ADAPT_STRUCT(
													TreeNode,
													(IntVec, Levels)
													(NodeType, Type)
													(StringVec, AttrLst)
													)

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
			("SELECT"   , SELECT)
			("JOIN"   , JOIN);
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
//grammar parser to parse query tree
//////////////////////////////////////////////////////////////////////////
template<typename Iterator>
struct my_grammar : qi::grammar<Iterator, TreeNode(), ascii::space_type>
{
	my_grammar() : my_grammar::base_type(start){
		using qi::lit;
		using qi::int_;

		start = levelsRule >> 
			(attr_keywords | relation_keywords) >> attrLstRule
			;

		attrLstRule = lit("([") >> stringRule % ',' >> lit("])");

		relationRule = lit("(") >> stringRule >> lit(")");

		stringRule = *(qi::alnum | '_' | '.');//stringRule = *(char_("_.a-zA-Z0-9"));

		levelsRule = int_ % ',';
	}

	qi::rule<Iterator, TreeNode(), ascii::space_type> start;

	qi::rule<Iterator, StringVec(), ascii::space_type> attrLstRule;

	qi::rule<Iterator, std::string(), ascii::space_type> relationRule;

	qi::rule<Iterator, std::string(), ascii::space_type> stringRule;

	qi::rule<Iterator, IntVec(), ascii::space_type> levelsRule;
};


int main(int argc, char *argv[])
{
	my_grammar<std::string::const_iterator> gar;

	TreeNode output;

	std::string text = "1,2 SCAN ([test,text])";
	BOOST_AUTO (iter, text.begin());
	BOOST_AUTO (end, text.end());

	bool result = phrase_parse(iter, end, gar, ascii::space, output);

	if (result && iter == end){
		std::cout << "Parsing OK!\n";
	}
	else{
		std::cout << "Parsing failed.\n";
	}

	return 0;
}
