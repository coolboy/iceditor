#include "StdAfx.h"

#include <vector>

#include <assert.h>

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "minijava2decaf.h"

using namespace std;
using namespace boost::xpressive;

typedef boost::function<void()> PendingWork;
typedef std::vector<PendingWork> PendingWorks;

MiniJava2Decaf::MiniJava2Decaf(void){}

MiniJava2Decaf::~MiniJava2Decaf(void){}

void MiniJava2Decaf::setMiniJava( const std::string& miniJava ){
	miniJava_ = miniJava;
}

std::string MiniJava2Decaf::getDecaf(){
	transform ();
	return decaf_;
}

//////////////////////////////////////////////////////////////////////////
void peningWork(std::string& decaf_, std::string target, std::string replaced)
{
	boost::algorithm::replace_all(decaf_, target, replaced);
}

std::string eraseLineWithoutEQ(const std::string& input)
{
	smatch what; //matching results

	sregex lineWithoutEQ = sregex::compile("^[^=]*$");

	return regex_replace(input, lineWithoutEQ, std::string());
}

//////////////////////////////////////////////////////////////////////////

void MiniJava2Decaf::transform()
{
	decaf_ = miniJava_;

	smatch what; //matching results
	PendingWorks pws; //replace work after regexp

	/*strip the comment
	* 1. / *...* /
	* 2. //... <- Don't need this kind of comment now.
	*/
	// essentially the same regex as in the previous example, but using a dynamic regex
	sregex comment = sregex::compile( "\\/\\*(\\s|.)*?\\*\\/");// \/\*(\s|.)*?\*\/ without escape

	decaf_ = regex_replace( miniJava_, comment, std::string() );

	/* 
	* Change multi declaration to multi line
	* int a,b = 5,...;
	* ->
	* int a;
	* int b = 5;
	* int ...
	*/

	sregex valSep = sregex::compile("(\\s*(\\d|\\w)+?)\\s((\\d|\\w|=|-| )+?),(.+?);"); // (\s*(\d|\w)+?)\s((\d|\w|=|-| )+?),(.+?);

	std::string::const_iterator beg = decaf_.begin();

	while( regex_search( beg, decaf_.end(), what, valSep ) )
	{
		std::string	obj = ";\n" + what[1];

		std::string target = what[0];

		std::string replaced = boost::algorithm::replace_all_copy(target, ",", obj);

		pws.push_back(boost::bind(&peningWork, boost::ref(decaf_), target, replaced));

		beg = what[0].second;
	}

	BOOST_FOREACH (PendingWork pw, pws)
		pw();
	pws.clear();

	/* Change argument list
	* method int product(int p; val int x, y)
	* ->
	* method int product(int p, int x, int y)
	* ,->, int
	* ;->,
	* val->
	*/
	sregex paras = sregex::compile("\\s*method[^(]+\\(([^)]+)\\)"); // \s*method[^(]+\(([^)]+)\)

	beg = decaf_.begin();

	while( regex_search( beg, decaf_.end(), what, paras ) )
	{
		std::string target = what[1];

		std::string  replaced = boost::algorithm::replace_all_copy(target, ",", ", int");
		boost::algorithm::replace_all(replaced, ";", ",");
		boost::algorithm::replace_all(replaced, "val", "");

		pws.push_back(boost::bind(&peningWork, boost::ref(decaf_), target, replaced));

		beg = what[0].second;
	}

	BOOST_FOREACH (PendingWork pw, pws)
		pw();
	pws.clear();

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

	//delete init
	sregex no_init = sregex::compile("_beg_no_init_(.*?)_end_no_init_"); // _beg_no_init_.*?_end_no_init_
	sregex init = sregex::compile("=.*?(;)"); // =.*?(;)

	beg = decaf_.begin();

	while( regex_search( beg, decaf_.end(), what, no_init ) )
	{
		std::string	org = what[0];

		std::string target = regex_replace (org, init, std::string	("$1"));

		pws.push_back(boost::bind(&peningWork, boost::ref(decaf_), org, target));

		beg = what[0].second;
	}

	BOOST_FOREACH (PendingWork pw, pws)
		pw();
	pws.clear();

	//delete type
	sregex no_type = sregex::compile("(_beg_no_type_.*?declarations)(.*?)(enddeclarations.*?_end_no_type_)");
	sregex type = sregex::compile("^(\\s*)(\\w|\\d)+\\s+((\\w|\\d)+\\s*=)"); // \n(\s*)(\w|\d)+\s+((\w|\d)+\s*=)
	sregex new_class = sregex::compile("^(\\s*)\\b(?!int)((\\w|\\d)+)\\s+((\\w|\\d)+);"); //\n(\s*)\b(?!int)((\w|\d)+)\s+((\w|\d)+);

	/* Init class append to the end of the no-init
	* main_entry=New(Person);
	*/
	beg = decaf_.begin();
	while( regex_search( beg, decaf_.end(), what, no_type ) )
	{
		std::string	org = what[0];

		std::string classWithNewString = regex_replace (org, new_class, std::string	("$1$4=New($2);"));
		std::string target = regex_replace (classWithNewString, 	type, std::string	("$1$3"));

		if (target != org)
			pws.push_back(boost::bind(&peningWork, boost::ref(decaf_), org, target));

		beg = what[0].second;
	}

	BOOST_FOREACH (PendingWork pw, pws)
		pw();
	pws.clear();

	beg = decaf_.begin();

	while( regex_search( beg, decaf_.end(), what, no_type ) )
	{
		std::string	org = what[2];

		/*
		* delete int line in _no_type_ without a '='
		*/
		std::string needDelType = eraseLineWithoutEQ(org);

		/*
		* delete type in _no_type_
		*/
		sregex delType = sregex::compile("(.*?)((\\w|\\d)+\\s*?=.*?$)"); //(.*?)((\w|\d)+\s*?=.*?$)
		std::string dupDeleted = regex_replace(needDelType,  delType, std::string("\n$2\n"));

		/*
		* Create Array
		* arr = int [10]
		* ->
		* arr = NewArray (10, int)
		*/
		sregex arrayInit = sregex::compile("int\\s+\\[(\\d+)\\]"); // int\s+\[(\d+)\]
		std::string arrayInitStr = regex_replace(dupDeleted,  arrayInit, std::string("NewArray($1, int)"));

		org = what[1] + org + what[3];
		arrayInitStr = what[1] + arrayInitStr + what[3];

		if (arrayInitStr != org)
			pws.push_back(boost::bind(&peningWork, boost::ref(decaf_), org, arrayInitStr));

		beg = what[0].second;
	}

	BOOST_FOREACH (PendingWork pw, pws)
		pw();
	pws.clear();

	/*
	* Add Init method for class init
	*/
	sregex classInit = sregex::compile( "(class[^}()]+?)(_beg_no_type_[^}]+?_end_no_type_.*?)?(method)"); // (class[^}()]+?)(_beg_no_type_[^}]+?_end_no_type_.*?)?(method)

	decaf_ = regex_replace( decaf_, classInit, std::string("$1void Init()\n{\n$2}\n$3") );//add init method to the class

	//rip init and type tags
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
		"\tmain_entry=New(CLASSNAME);\n"
		"\tmain_entry.main();\n"
		"}\n";

	sregex classWithMain = sregex::compile( "method\\s+void\\s+main" );// method\s+void\s+main //get main pos

	if( regex_search( decaf_.begin(), decaf_.end(), what, classWithMain ) )
	{
		std::string::const_iterator mainIter = what[0].second;
		auto mainPos = mainIter - decaf_.begin();
		auto classPos = decaf_.rfind("class", mainPos);

		sregex classNameRxp = sregex::compile("class\\s+((\\w|\\d)+)"); //class\s+((\w|\d)+)

		std::string	mainClassName;

		if( regex_search( decaf_.begin() + classPos, decaf_.end(), what, classNameRxp ) )
		{
			mainClassName = what[1];
		}
		else
			assert(0);

		boost::algorithm::replace_all(mainFuncStr, "CLASSNAME", mainClassName);

		decaf_+= mainFuncStr;
	}
	else
		assert(0);

	/*
	* Call Init member function 
	*/
	sregex classNew = sregex::compile( "(\\s*?)((\\w|\\d)+)\\s*?=\\s*?New\\((\\w|\\d)+\\)\\s*?;"); // (\s*?)((\w|\d)+)\s*?=\s*?New\((\w|\d)+\)\s*?;

	decaf_ = regex_replace( decaf_, classNew, std::string("$&$1$2.Init();") );//call init method after val=New(CLASSNAME)

	////////////////////////////////////Erase keywords/////////////////////////////

	/*
	* strip program keyword
	* 1. program t1; //like this
	*/
	sregex program = sregex::compile( "program\\s+\\w(\\w|\\d)*;");// program\s+\w(\w|\d)*;

	decaf_ = regex_replace( decaf_, program, "" );

	/*
	* strip declarations/enddeclarations keyword
	* declarations <- delete
	* int x=-1;
	* enddeclarations <- delete
	*/

	sregex decl = sregex::compile("(declarations|enddeclarations)");

	decaf_ = regex_replace( decaf_, decl, "" );

	/*
	* strip method keyword
	*/

	sregex method = sregex::compile("method");

	decaf_ = regex_replace( decaf_, method, std::string() );

	/*
	* Change System.println(val) to Print(val) + Print("\n")
	*/

	sregex print = sregex::compile("(\\s*)System\\.println(\\(.*?);", icase); // (\s*)System\.println(\(.*?);

	decaf_ = regex_replace( decaf_, print, std::string("$1Print$2;\n$1Print('\\n');\n") );

	/*
	* Change System.readln(x) to x = ReadInteger()
	*/

	sregex readInt = sregex::compile("System\\.readln\\(([^)]+)\\)", icase); // System\.readln\(([^)]+)\)

	decaf_ = regex_replace( decaf_, readInt, std::string("$1 = ReadInteger()") );

	/*
	* Delete useless blank line
	*/

	sregex blankLine = sregex::compile("[\\n|\\r]\\s*[\\n|\\r]"); // [\n|\r]\s*[\n|\r]

	decaf_ = regex_replace( decaf_, blankLine, std::string("\n") );

	/*
	* Change ' in Print to "
	*/
	sregex singleQuote = sregex::compile("'");

	decaf_ = regex_replace( decaf_, singleQuote, std::string("\"") );

	/*
	* Change := -> =
	*/

	boost::algorithm::replace_all(decaf_, ":=", "=");

	/*
	* Change EOL to Unix style //don't need it under unix
	*/
}
