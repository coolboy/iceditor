#include "stdafx.h"

#include <boost/config/warning_disable.hpp>

#include <boost/spirit/include/qi.hpp>
//#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

#include <iostream>
#include <vector>
#include <string>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

//////////////////////////////////////////////////////////////////////////
// Typedefs
//////////////////////////////////////////////////////////////////////////
typedef std::vector<std::string> StringVec;
typedef std::vector<int> IntVec;

//////////////////////////////////////////////////////////////////////////
// enu for node type
//////////////////////////////////////////////////////////////////////////
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

namespace client
{
	namespace qi = boost::spirit::qi;

	// A member function
	struct writer
	{
		void print(int const& i) const
		{
			std::cout << i << std::endl;
		}

		void printStr(std::string const& i) const
		{
			std::cout << i << std::endl;
		}
	};

	void onlevelLst (IntVec& iVec)
	{

	}
}

using namespace client;

int main()
{
	using qi::int_;
	using qi::parse;
	using qi::lit;

	using ascii::space;

	using client::writer;

	std::string text = "UNION"
		"\t1 PROJECT([FirstName, MiddleName, LastName])"
		"\t\t1,1 SCAN(DOCTOR)"
		"\t2 PROJECT([FirstName, MiddleName, LastName])"
		"\t\t2,1 SELECT(PATIENT.SSN=PRESCRIPTION.Patient)";
	BOOST_AUTO (first, text.begin());
	BOOST_AUTO (last, text.end());

	writer w;
	//qi::rule<std::string::const_iterator, IntVec(), ascii::space_type>
		//levelsRule = int_[boost::bind(&writer::print, &w, _1)] % ',';

	BOOST_AUTO(levelsRule, int_[boost::bind(&writer::print, &w, _1)] % ',');

	qi::rule<std::string::const_iterator, std::string(), ascii::space_type>
	 stringRule = *(qi::alnum | '_' | '.');

	qi::rule<std::string::const_iterator, std::string(), ascii::space_type>
		conExpRule = *(qi::alnum | '_' | '.' | '>' | '=' | '<');

	BOOST_AUTO(attrLstRule, lit("([") >> stringRule[boost::bind(&writer::printStr, &w, _1)] % ',' >> lit("])") );
	BOOST_AUTO(relationRule, lit("(") >> stringRule[boost::bind(&writer::printStr, &w, _1)] >> lit(")"));
	BOOST_AUTO(conditionRule, lit("(") >> conExpRule[boost::bind(&writer::printStr, &w, _1)] >> lit(")"));

	BOOST_AUTO(nodeRule, ( 
		  (attr_keywords >> attrLstRule) 
		| (relation_keywords >> relationRule ) 
		| (condition_keywords >> conditionRule) 
		| (null_keywords) 
		) );

	BOOST_AUTO(begRule, nodeRule );
	BOOST_AUTO(expRule, levelsRule >> nodeRule );

	BOOST_AUTO(start, begRule >> * expRule );

	qi::phrase_parse(first, last, start, space);

	return 0;
}
