#include "stdafx.h"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace client
{
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;

	struct relation_keywords_ : qi::symbols<char, char>
	{
		relation_keywords_()
		{
			add
				("SCAN", "SCAN")
				("INDEX_SCAN", "INDEX_SCAN")
				("HASH_SCAN", "HASH_SCAN");
		}

	} relation_keywords;

	struct condition_keywords_ : qi::symbols<char, char>
	{
		condition_keywords_()
		{
			add
				("SELECT"   , "SELECT")
				("JOIN"   , "JOIN");
		}

	} condition_keywords;

	struct attr_keywords_ : qi::symbols<char, char>
	{
		attr_keywords_()
		{
			add
				("PROJECT"   , "PROJECT");
		}

	} attr_keywords;

	struct null_keywords_ : qi::symbols<char, char>
	{
		null_keywords_()
		{
			add
				("UNION"   , "UNION")
				("PRODUCT"   , "PRODUCT");
		}

	} null_keywords;

	///////////////////////////////////////////////////////////////////////////
	//  Our number list compiler
	///////////////////////////////////////////////////////////////////////////
	typedef std::vector<int> NumberVec;
	typedef std::vector<std::string> StringVec;
	typedef std::vector<std::pair<NumberVec, StringVec>> OutVec;

	template <typename Iterator>
	struct querytree : qi::grammar<Iterator, StringVec()>
	{
		querytree() : querytree::base_type(stringList)
		{
			using qi::int_;
			using qi::char_;

			using qi::lit;

			//start = stringList >> *(numberList >> stringList);
			//numberList = int_ % ',';
			stringList = lit("([") >> (char_("a-zA-Z_.") - ',') % ',' >> lit("])");
		}

		//qi::rule<Iterator, OutVec()> start;
		//qi::rule<Iterator, NumberVec()> numberList;
		qi::rule<Iterator, StringVec()> stringList;
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

		client::StringVec v;

		std::string::const_iterator iter = str.begin();
		std::string::const_iterator end = str.end();

		bool r = parse(iter, end, tree_parser, v);

		if (r && iter == end)
		{
			std::cout << "-------------------------\n";
			std::cout << "Parsing succeeded\n";
			std::cout << str << " Parses OK: " << std::endl;

			//for (client::NumberVec::size_type i = 0; i < v.size(); ++i)
				//std::cout << i << ": " << v[i] << std::endl;

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
