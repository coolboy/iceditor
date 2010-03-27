// myIQO.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DbCatalog.h"
#include "QueryTree.h"

using namespace std;

DbCatalog *dbCata;

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

	return inFileBuf;
}

int main(int argc, char* argv[])
{
	const char* dbSchemaPath = "dbSchema.txt", *dbIndexingPath = 0,
		*dbmsConfigPath = 0, *queryTreesPath = 0;

	string dbSchemaStr, dbIndexingStr, dbConfigStr, queryTreesStr;  // strings used to store input files

	if (argc != 5){
		cerr<<"Wrong arguments number!\n";
		return 1;
	}

	dbSchemaPath = argv[1];
	dbIndexingPath = argv[2];
	dbmsConfigPath = argv[3];
	queryTreesPath = argv[4];

	// read input files to strings
	//dbSchemaStr = ReadAll(dbSchemaPath);
	//	dbIndexingStr = ReadAll(dbIndexingPath);
	//	dbConfigStr = ReadAll(dbmsConfigPath);
	queryTreesStr = ReadAll(queryTreesPath);

	//create catalog module for the database
	//dbCata = new DbCatalog(dbSchemaStr, "s", "s");

	//deal with db schema

	//deal with db indexing

	//deal with dbms config

	//deal with query trees
	using namespace client;
	QueryTreeNodePtr root = ParseQueryTree(queryTreesStr);

	return 0;
}
