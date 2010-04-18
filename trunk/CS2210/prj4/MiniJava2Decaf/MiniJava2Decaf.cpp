#include "StdAfx.h"

#include <boost/xpressive/xpressive.hpp>

#include "minijava2decaf.h"

using namespace boost::xpressive;

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
	sregex date = sregex::compile( "\\/\\*(\\s|.)*?\\*\\/" );// (\/\*(\s|.)*?\*\/) without escape

	// As in Perl, $& is a reference to the sub-string that matched the regex
	std::string format( "" );

	decaf_ = regex_replace( miniJava_, date, format );

	/*
	 * strip program keyword
	 * 
	*/

	/*
	* strip declarations/enddeclarations keyword
	* declarations
	* int x=-1;
	* enddeclarations
	*/

	/*
	* strip declarations/enddeclarations keyword
	* declarations
	* int x=-1;
	* enddeclarations
	*/

	/*
	* Move main function out and append to the file end.
	*/

	/*
	* Change Print to System.println
	*/
}
