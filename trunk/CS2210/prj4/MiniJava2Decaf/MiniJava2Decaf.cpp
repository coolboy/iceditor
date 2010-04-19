#include "StdAfx.h"

#include <boost/xpressive/xpressive.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "minijava2decaf.h"

using namespace boost::xpressive;
using namespace std;

MiniJava2Decaf::MiniJava2Decaf(void){}


MiniJava2Decaf::~MiniJava2Decaf(void){}

void MiniJava2Decaf::setMiniJava( const std::string& miniJava )
{
	miniJava_ = miniJava;
}

std::string MiniJava2Decaf::getDecaf()
{
	transform ();

	return decaf_;
}

void MiniJava2Decaf::transform()
{
	decaf_ = miniJava_;
	/*strip the comment
	 * 1. / *...* /
	 * 2. //... <- Don't need this kind of comment now.
	*/
	// essentially the same regex as in the previous example, but using a dynamic regex
	sregex comment = sregex::compile( "\\/\\*(\\s|.)*?\\*\\/");// \/\*(\s|.)*?\*\/ without escape

	decaf_ = regex_replace( miniJava_, comment, std::string() );

	/* 
	* Change multi declaration to multi line // int/int[]
	* int a,b = 5,...;
	* ->
	* int a;
	* int b = 5;
	* int ...
	*/

	sregex valSep = sregex::compile("(\\s*int)\\s+[^;,]*(,\\s+([^;,])*)*;"); // (\s*int)\s+[^;,]*(,\s+([^;,])*)*;

	smatch what;

	std::string::const_iterator beg = decaf_.begin();

	while( regex_search( beg, decaf_.end(), what, valSep ) )
	{
		std::string	obj = ";\n" + what[1];

		std::string target = what[0];

		std::string replaced = boost::algorithm::replace_all_copy(target, ",", obj);

		boost::algorithm::replace_all(decaf_, target, replaced);

		beg = what[0].second;
	}

	/* move declarations to the beginning of the function
	* method void main()
	* int x=4; -> delete
	* {
	* int x=4; <- add
	*/
	sregex bracketBelow = sregex::compile( "(method[^{]*)\\{");// (method[^{]*)\{

	decaf_ = regex_replace( decaf_, bracketBelow, std::string("$1") );//delete bracket below

	sregex bracketNear = sregex::compile( "method[^)]*\\)");// method[^)]*\)

	decaf_ = regex_replace( decaf_, bracketNear, std::string("$& {") );//add bracket near

	/*
	* Separate declaration and initiation
	* int a=1;
	* int b=3;
	* ->
	* int a;
	* int b;
	* a=1;
	* b=1;
	*/

	sregex decl_init = sregex::compile( "\\s*declarations.*?enddeclarations"); // \s*declarations.*?enddeclarations //? for non-greedy

	std::string pattern = 
		"\n_beg_no_init_"
		"$&\n"
		"_end_no_init_\n"
		"_beg_no_type_"
		"$&\n"
		"_end_no_type_\n";

	decaf_ = regex_replace( decaf_, decl_init, pattern );

	sregex no_init = sregex::compile("_beg_no_init_(.*?)_end_no_init_"); // _beg_no_init_.*?_end_no_init_
	sregex init = sregex::compile("=.*?(;)"); // =.*?(;)

	beg = decaf_.begin();

	while( regex_search( beg, decaf_.end(), what, no_init ) )
	{
		std::string	org = what[0];

		std::string target = regex_replace (org, init, std::string	("$1"));

		boost::algorithm::replace_all(decaf_, org, target);

		beg = what[0].second;
	}

	sregex no_type = sregex::compile("_beg_no_type_(.*?)_end_no_type_");
	sregex type = sregex::compile("(\\s*)(\\w|\\d)+\\s+((\\w|\\d)+\\s*=)"); // (\s*)(\w|\d)+\s+((\w|\d)+\s*=)

	beg = decaf_.begin();

	while( regex_search( beg, decaf_.end(), what, no_type ) )
	{
		std::string	org = what[0];

		std::string target = regex_replace (org, type, std::string	("$1$3"));

		boost::algorithm::replace_all(decaf_, org, target);

		beg = what[0].second;
	}

	//rip tags
	sregex tags = sregex::compile("(_beg_no_init_|_end_no_init_|_beg_no_type_|_end_no_type_)");
	decaf_ = regex_replace (decaf_, tags, std::string());

	/*
	* Append a new main function to the file end.
	* like
	* void main() {
	* CLASSNAME main_entry;
	* main_entry.main();
	* }
	*/

	std::string mainFuncStr = "void main() {\n"
		"\tCLASSNAME main_entry;\n"
		"\tmain_entry.main();\n"
	 "}";

	sregex classWithMain = sregex::compile( "class\\s+(\\w+)(.)*main" );// class\s+(\w+)(.|\n|\r)*main

	if( regex_search( decaf_.begin(), decaf_.end(), what, classWithMain ) )
	{
		std::string	obj = what[1];
		boost::algorithm::replace_first(mainFuncStr, "CLASSNAME", obj);
	}

	 decaf_+= mainFuncStr;

	 /* Init class
	 * main_entry=New(Person);
	 */

	////////////////////////////////////Erase keywords/////////////////////////////

	/*
	 * strip program keyword
	 * 1. program t1; //like this
	*/
	sregex program = sregex::compile( "program\\s+\\w(\\w|\\d)*;");// program\s+\w(\w|\d)*;

	decaf_ = regex_replace( decaf_, program, std::string() );

	/*
	* strip declarations/enddeclarations keyword
	* declarations <- delete
	* int x=-1; //how to deal with the = -1? ///TODO!
	* enddeclarations <- delete
	*/

	sregex decl = sregex::compile("(declarations|enddeclarations)");

	decaf_ = regex_replace( decaf_, decl, std::string() );

	/*
	* strip method keyword
	*/

	sregex method = sregex::compile("method");

	decaf_ = regex_replace( decaf_, method, std::string() );

	/*
	* Change System.println(val) to Print(val) + Print("\n")
	*/

	sregex print = sregex::compile("(\\s*)System\\.println\\(([^)]*)\\)\\s*;"); // System\.println\(([^)]*)\)\s*;

	decaf_ = regex_replace( decaf_, print, std::string("$1Print($2);\n$1Print('\\n\');\n") );

	/*
	* Delete useless blank line
	*/

	sregex blankLine = sregex::compile("[\\n|\\r]\\s*[\\n|\\r]"); // [\n|\r]\s*[\n|\r]

	decaf_ = regex_replace( decaf_, blankLine, std::string("\n") );
}
