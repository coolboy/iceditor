#include "StdAfx.h"

#include <iostream>
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
	 * strip program keyword
	 * 1. program t1; //like this
	*/
	sregex program = sregex::compile( "program\\s+\\w(\\w|\\d)*;");// program\s+\w(\w|\d)*;

	decaf_ = regex_replace( decaf_, program, std::string() );

	/*
	* strip declarations/enddeclarations keyword in class
	* declarations
	* int x=-1;
	* enddeclarations
	*/

	/*
	* strip declarations/enddeclarations keyword in method
	* declarations
	* int x=-1;
	* enddeclarations
	*/

	/*
	* Move main function out and append to the file end.
	*/

	/*
	* Change System.println to Print + Print("\n")
	*/
}
