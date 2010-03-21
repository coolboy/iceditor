// myIQO.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

using namespace std;

void FileOpenError(const char* fileName){
	cerr<<"Can't open : "<<fileName<<endl;
}

string ReadAll(const char* fileName){
	string inFileBuf;
	ifstream ifs(fileName);

	if (!ifs){
		return inFileBuf;
	}

	ifs.unsetf(ios::skipws);//unset skip space

	copy(istream_iterator<char>(ifs), istream_iterator<char>(), back_inserter(inFileBuf));

	return inFileBuf
}

int main(int argc, char* argv[])
{
	const char* dbSchemaPath = 0, *dbIndexingPath = 0,
		*dbmsConfigPath = 0, *queryTreesPath = 0;
	if (argc != 5){
		cerr<<"Wrong arguments number!\n";
		return 1;
	}

	dbSchemaPath = argv[1];
	dbIndexingPath = argv[2];
	dbmsConfigPath = argv[3];
	queryTreesPath = argv[4];

	//deal with db schema

	//deal with db indexing

	//deal with dbms config

	//deal with query trees


	return 0;
}
