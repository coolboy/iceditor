#include "StdAfx.h"
#include "minijava2decaf.h"


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
	/*strip the comment
	 * 1. / *...* /
	 * 2. //...
	*/

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
