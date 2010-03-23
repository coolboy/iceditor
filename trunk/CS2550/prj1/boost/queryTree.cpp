#include "stdafx.h"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace client
{
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;

	//struct hundreds_ : qi::symbols<char, char>
	//{
	//	hundreds_()
	//	{
	//		add
	//			("C"    , 100)
	//			("CM"   , 900)
	//			;
	//	}

	//} hundreds;

	///////////////////////////////////////////////////////////////////////////
	//  Our number list compiler
	///////////////////////////////////////////////////////////////////////////
	typedef std::vector<int> NumberVec;

	template <typename Iterator>
	struct querytree : qi::grammar<Iterator, NumberVec()>
	{
		querytree() : querytree::base_type(start)
		{
			using qi::double_;

			start = double_ % ',';
		}

		qi::rule<Iterator, NumberVec()> start;
	};
}

////////////////////////////////////////////////////////////////////////////
//  Main program
////////////////////////////////////////////////////////////////////////////
int
main()
{
	std::cout << "/////////////////////////////////////////////////////////\n\n";
	std::cout << "\t\tA comma separated list parser for Spirit...\n\n";
	std::cout << "/////////////////////////////////////////////////////////\n\n";

	std::cout << "Give me a comma separated list of numbers.\n";
	std::cout << "The numbers will be inserted in a vector of numbers\n";
	std::cout << "Type [q or Q] to quit\n\n";

	std::string str;
	typedef std::string::const_iterator iterator_type;
	typedef client::querytree<iterator_type> querytree_parser;
	querytree_parser tree_parser; // Our grammar

	while (getline(std::cin, str))
	{
		if (str.empty() || str[0] == 'q' || str[0] == 'Q')
			break;

		client::NumberVec v;

		std::string::const_iterator iter = str.begin();
		std::string::const_iterator end = str.end();

		bool r = parse(iter, end, tree_parser, v);

		if (r && iter == end)
		{
			std::cout << "-------------------------\n";
			std::cout << "Parsing succeeded\n";
			std::cout << str << " Parses OK: " << std::endl;

			for (client::NumberVec::size_type i = 0; i < v.size(); ++i)
				std::cout << i << ": " << v[i] << std::endl;

			std::cout << "\n-------------------------\n";
		}
		else
		{
			std::cout << "-------------------------\n";
			std::cout << "Parsing failed\n";
			std::cout << "-------------------------\n";
		}
	}

	std::cout << "Bye... :-) \n\n";
	return 0;
}
