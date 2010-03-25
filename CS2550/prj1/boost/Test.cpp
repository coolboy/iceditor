#include "stdafx.h"

#include <boost/config/warning_disable.hpp>

#include <boost/spirit/include/qi.hpp>
//#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

#include <iostream>
#include <vector>
#include <string>

//////////////////////////////////////////////////////////////////////////
// Typedefs
//////////////////////////////////////////////////////////////////////////
typedef std::vector<std::string> StringVec;
typedef std::vector<int> IntVec;

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
	};
}

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

int main()
{
	using qi::int_;
	using qi::parse;
	using qi::lit;

	using ascii::space;


	using client::writer;


	std::string text = "([44,43])";
	BOOST_AUTO (first, text.begin());
	BOOST_AUTO (last, text.end());

	writer w;
	//qi::rule<std::string::const_iterator, IntVec(), ascii::space_type>
		//levelsRule = int_[boost::bind(&writer::print, &w, _1)] % ',';

	BOOST_AUTO(levelsRule, int_[boost::bind(&writer::print, &w, _1)] % ',');

	IntVec v;
	qi::phrase_parse(first, last, 
		lit("([") >> levelsRule >> lit("(["),
		space, v);

	return 0;
}
