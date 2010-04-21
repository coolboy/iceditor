#include "StdAfx.h"

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>

#include "AsmRefiner.h"
#include "MiniJava.h"
#include "MiniJava2Decaf.h"

using namespace std;

string ReadAll(const char* fileName){
	string inFileBuf;
	ifstream ifs(fileName);

	if (!ifs){
		return inFileBuf;
	}

	ifs.unsetf(ios::skipws);//unset skip space

	copy(istream_iterator<char>(ifs), istream_iterator<char>(), back_inserter(inFileBuf));

	return inFileBuf;
}

bool WriteAll(const char* fileName, const std::string& outFilebuf){
	ofstream ofs(fileName);

	if (!ofs){
		return false;
	}

	ofs << outFilebuf;

	return  true;
}

// MiniJava -s src.java
// MiniJava -a out.s
int main(int argc, char* argv[])
{
	if (argc !=3 )
	{
		cerr<<"Wrong arguments!\n";
		return EXIT_FAILURE;
	}

	std::string a1, a2;
	a1 = argv[1];
	a2 = argv[2];

	if (a1 == "-s")
		ConvertToDecaf(argv[2]);
	else if (a1 == "-a")
		ConvertToMips(argv[2]);
	else
		cerr<<"Wrong arguments!\n";

	return EXIT_SUCCESS;
}

void ConvertToDecaf( const char* fileName )
{
	std::string fileBuf = ReadAll(fileName);

	if (fileBuf.empty())
		return;

	MiniJava2Decaf mjd;
	mjd.setMiniJava(fileBuf);

	std::string	decaf = mjd.getDecaf();

	WriteAll("out.decaf", decaf);
}

void ConvertToMips( const char* fileName )
{
	std::string fileBuf = ReadAll(fileName);

	if (fileBuf.empty())
		return;

	AsmRefiner ar;
	ar.setDecafAsm(fileBuf);

	WriteAll("out.s", ar.getMiniJavaAsm());
}
