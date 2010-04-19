// MiniJava.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>

#include "AsmRefiner.h"
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

// MiniJava src.java out.s
int main(int argc, char* argv[])
{
	if (argc !=3 )
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

	cout<<"Writing out.decaf...\n\n";
	WriteAll("out.decaf", mjd.getDecaf());

	fileBuf = ReadAll(argv[2]);

	AsmRefiner ar;
	ar.setDecafAsm(fileBuf);

	cout<<"Writing out.s...\n\n";
	cout<<ar.getMiniJavaAsm()<<endl;
	WriteAll("out.s", ar.getMiniJavaAsm());

	return EXIT_SUCCESS;
}