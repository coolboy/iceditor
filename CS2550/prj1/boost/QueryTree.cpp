#include "StdAfx.h"
#include "QueryTree.h"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/bind.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace fu = boost::fusion;

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
		conExpRule = *(qi::alnum | '_' | '.' | '>' | '=' | '<' | '\'');

	BOOST_AUTO(attrLstRule, lit("([") >> stringRule % ',' >> lit("])") );
	BOOST_AUTO(relationRule, lit("(") >> stringRule >> lit(")"));
	BOOST_AUTO(conditionRule, lit("(") >> conExpRule >> lit(")"));

	BOOST_AUTO(start,
		begRule >> * expRule 
	);

	bool ret = true;
	while (first != last && ret == true)
	{
		ret = qi::phrase_parse(first, last, start, space);
	}

	return ;
}