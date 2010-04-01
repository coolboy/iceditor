// myIQO.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "DbCatalog.h"

#include "QueryTree.h"

#include "TreeOptimizer.h"

#include <boost/foreach.hpp>

using namespace std;
using namespace client;

//////////////////////////////////////////////////////////////////////////
DbCatalog *	dbCata = 0;
//////////////////////////////////////////////////////////////////////////

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

//template<typename T>
//std::ostream& operator << (std::ostream& out, const std::vector<T>& vec)
//{
//	out<<'[';
//	for (std::vector<T>::size_type i = 0; i < vec.size() - 1; ++i)
//		out << vec[i] << ", ";
//
//	out<< vec[vec.size() - 1]<<']';
//
//	return out;
//}

int main(int argc, char* argv[])
{
	const char* dbSchemaPath = 0, *dbIndexingPath = 0,
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
	dbSchemaStr = ReadAll(dbSchemaPath);
	dbIndexingStr = ReadAll(dbIndexingPath);
	dbConfigStr = ReadAll(dbmsConfigPath);
	queryTreesStr = ReadAll(queryTreesPath);

	//create catalog module for the database
	dbCata = new DbCatalog(dbSchemaStr, dbIndexingStr, dbConfigStr);

	//deal with db schema

	//deal with db indexing

	//deal with dbms config

	//deal with query trees

	//test cases
	QueryTreeNodePtrs trees = ParseQueryTree(queryTreesStr);

	BOOST_FOREACH(QueryTreeNodePtr pnode, trees)
		PrintTree(pnode);

	QueryTreeNodePtr root = trees[0];

	cout<<"Original tree: \n";
	PrintTree(root);

	//cout<<"Clone tree: \n";
	//QueryTreeNodePtr root_clone = root->clone();
	//PrintTree(root_clone);

	//using namespace boost::assign;
	//IntVec v1, v2;
	//v1 += 1,1;
	//v2 += 1,2;

	//cout<<"After swap"<< v1 <<" "<< v2 << "\n";
	//SwapNode(root, v1, v2);
	//PrintTree( root );

	//QueryTreeNode node;
	//node.setType(client::UNION);
	//QueryTreeNodePtr pnode = QueryTreeNodePtr(new QueryTreeNode(node));

	//cout<<"After append"<< v1<<"\n";
	//AppendNode(root, pnode, v1);
	//PrintTree( root );

	//pnode = QueryTreeNodePtr(new QueryTreeNode(node));
	//cout<<"After insert"<< v2<<"\n";
	//InsertNode(root, pnode, v2);
	//PrintTree( root );

	//cout<<"After remove"<< v1<<"\n";
	//RemoveNode(root, v1);
	//PrintTree( root );

	//verify the clone tree is ok
	//PrintTree(root_clone);

	//select token
	//ConditionTokenizer con("FirstName='Michael' AND PATIENT.FirstName=DOCTOR.FirstName");

	//get tree node path
	//IntLst lvl;
	//GetNodePath(root, root, lvl);

	//Test get nodes by type
	//GetNodesByType(root, SCAN);

	TreeOptimizer to;

	PrintTree(to.optimize(root));

	return 0;
}
