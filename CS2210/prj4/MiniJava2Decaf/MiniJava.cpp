// MiniJava.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>

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

// MiniJava src.java out.s
int main(int argc, char* argv[])
{
	if (argc !=2 )
	{
		cerr<<"Wrong arguments!\n";
		return EXIT_FAILURE;
	}

	std::string fileBuf;

	fileBuf = ReadAll(argv[1]);

	if (fileBuf.empty())
	{
		cerr<<"Src file read error!\n";
		return EXIT_FAILURE;
	}

	MiniJava2Decaf mjd;
	mjd.setMiniJava(fileBuf);

	cout<<mjd.getDecaf();

	return EXIT_SUCCESS;
}