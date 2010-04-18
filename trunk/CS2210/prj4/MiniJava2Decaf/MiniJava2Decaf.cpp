#include "StdAfx.h"

#include <boost/xpressive/xpressive.hpp>

#include "minijava2decaf.h"

using namespace boost::xpressive;
using namespace std;

MiniJava2Decaf::MiniJava2Decaf(void)
{
}


MiniJava2Decaf::~MiniJava2Decaf(void)
{
}

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
	* Change multi declaration to multi line //only int and int[]
	* int a,b = 5,...;
	* ->
	* int a;
	* int b = 5;
	* int ...
	* 
	* move declarations to the beginning of the function
	* method void main()
	* int x=4; -> delete
	* {
	* int x=4; <- add
	*/

	/*
	* Move main function out and append to the file end.
	*/

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

	sregex print = sregex::compile("System\\.println\\(([^)]*)\\)\\s*;"); // System\.println\(([^)]*)\)\s*;

	decaf_ = regex_replace( decaf_, print, std::string("Print($1);\nPrint('\\n\');\n") );
	//decaf_ = regex_replace( decaf_, print, std::string("[R]") );
}
