#include "stdafx.h"

//ref:http://www.gamedev.net/community/forums/topic.asp?topic_id=557809
//ref:http://stackoverflow.com/questions/2469861/parsing-string-with-boost-spirit-2-to-fill-data-in-user-defined-struct

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

template<typename Iterator>
struct my_grammar : qi::grammar<Iterator, std::vector<std::string>(), ascii::space_type>
{
	my_grammar() : my_grammar::base_type(start){
		using qi::lit;
		using qi::int_;

		start = lit("([") >> stringRule % ',' >> lit("])");

		//stringRule = *(char_("_.a-zA-Z0-9"));
		stringRule = *(qi::alnum | '_' | '.');

		uintsRule = int_ % ',';
	}

	qi::rule<Iterator, std::vector<std::string>(), ascii::space_type> start;
	qi::rule<Iterator, std::string(), ascii::space_type> stringRule;
	qi::rule<Iterator, std::vector<int>(), ascii::space_type> uintsRule;
};


int main(int argc, char *argv[])
{
	my_grammar<std::string::const_iterator> g;

	std::vector<std::string> output;

	std::string text = "([test,text])";
	std::string::const_iterator iter = text.begin();
	std::string::const_iterator end = text.end();

	bool result = phrase_parse(iter, end, g, ascii::space, output);

	if (result && iter == end){
		std::cout << "Parsing OK!\n";
		BOOST_FOREACH(std::string s, output){
			std::cout << s << std::endl;
		}
	}
	else{
		std::cout << "Parsing failed.\n";
	}

	return 0;
}
