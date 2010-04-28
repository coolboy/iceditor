#include "StdAfx.h"

#include <vector>

#include <assert.h>

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/algorithm/string.hpp>

#include "MiniJava2Decaf.h"

using namespace std;
using namespace boost::xpressive;
using namespace boost::algorithm;

typedef boost::function<void()> PendingWork;
typedef std::vector<PendingWork> PendingWorks;
typedef vector< string > StrVec;

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

/*
* Array Init list transform
* ia = {3, 5, 7};
* ->
* ia = NewArray(3, int);
* ia[0] = 3;
* ia[1] = 5;
* ia[2] = 7;
*/
std::string transformArrayInitListInternal(const std::string& src){
	if (src.find('{') == std::string::npos)
		return src;

	sregex extractArray = sregex::compile( "\\s*((\\w|\\d)+).*?=\\s*?\\{(.+?)}");// \s*((\w|\d)+).*?=\s*?\{(.+?)}

	string AryNameAndLst = regex_replace( src, extractArray, std::string("$1;$3") );//delete bracket below

	StrVec srcLines;
	boost::algorithm::split( srcLines, AryNameAndLst, is_any_of(";") );

	std::string arrayName = srcLines[0];

	StrVec argVec;
	boost::algorithm::split( argVec, srcLines[1], is_any_of(",") );

	assert (argVec.size() >= 1);

	std::stringstream ss;

	ss << arrayName <<" = NewArray("<< argVec.size() <<", int);\n";

	for (int i = 0; i != argVec.size(); ++i){
		ss << arrayName << '[' << i << "] = " << argVec[i] <<";\n";
	}

	return ss.str();
}

std::string transformArrayInitList(const std::string& src){
	std::string ret;
	smatch what; //matching results

	StrVec srcLines;
	boost::algorithm::split( srcLines, src, is_any_of(";") );//why will have empty elem?

	for (int i = 0; i != srcLines.size(); ++i){
		srcLines[i] = transformArrayInitListInternal(srcLines[i]);
	}

	ret.clear();
	for (int i = 0; i != srcLines.size(); ++i){
		if (srcLines[i].size() != 1)
			ret += srcLines[i] + ";\n";
	}

	return ret;
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
		std::string needDelTypeStr = eraseLineWithoutEQ(org);

		/*
		* delete type in _no_type_
		*/
		sregex delType = sregex::compile("^(\\s*?)(\\w|\\[|\\])+(.*?;)"); //^(\s*?)(\w|\[|\])+(.*?;)
		std::string typeDeletedStr = regex_replace(needDelTypeStr,  delType, std::string("$1$3"));

		/*
		* Create Array
		* arr = int [10]
		* ->
		* arr = NewArray (10, int)
		*/
		sregex arrayNew = sregex::compile("int\\s*\\[(\\d+)\\]"); // int\s*\[(\d+)\]
		std::string arrayNewStr = regex_replace(typeDeletedStr,  arrayNew, std::string("NewArray($1, int)"));

		/*
		* Array Init list transform
		* ia = {3, 5, 7};
		* ->
		* ia[0] = 3;
		* ia[1] = 5;
		* ia[2] = 7;
		*/

		std::string arrayInitListStr = transformArrayInitList(arrayNewStr);

		org = what[1] + org + what[3];
		arrayInitListStr = what[1] + arrayInitListStr + what[3];

		if (arrayInitListStr != org)
			pws.push_back(boost::bind(&peningWork, boost::ref(decaf_), org, arrayInitListStr));

		beg = what[0].second;
	}

	BOOST_FOREACH (PendingWork pw, pws)
		pw();
	pws.clear();

	/*
	* Add Init method for class init
	*/
	sregex classInit = sregex::compile( "(class[^}()]+?)(_beg_no_type_.+?_end_no_type_.*?)?(\\s*?method)"); // (class[^}()]+?)(_beg_no_type_.+?_end_no_type_.*?)?(\s*?method)

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
		int mainPos = mainIter - decaf_.begin();
		int classPos = decaf_.rfind("class", mainPos);

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
	* Tested //we dont need that
	*/
}
