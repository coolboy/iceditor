#include "StdAfx.h"
#include "QueryTree.h"
#include "Opt24.h"
#include "dbCatalog.h"

using namespace client;

int CostCalcTree(QueryTreeNodePtr SubTreeRoot,  DbCatalog* dbCatalog)  //should be recursive
{
    int blocks=0;
    int costs=0;
	std::string delimiters = " ()\t\n.'=<>";
	NodeType nt = SubTreeRoot->getType();
    
    switch(nt)
	{	 
	case	UNDEF:
		{
		//example: setExInfo("Number", 102);
		    SubTreeRoot->setExInfo("Cost", 0); 
			if (SubTreeRoot->hasChild(1))
		    {
				CostCalcTree(SubTreeRoot->getChild(1), dbCatalog);
		    }
			if (SubTreeRoot->hasChild(2))
		    {
				CostCalcTree(SubTreeRoot->getChild(2), dbCatalog);
		    }
		    //QueryTreeNodePtr RightChild;
			break;
		}
	case	SCAN:
		{
	    //SCAN node should have only one child, and its child should not have any child.
		//we need to decide access method and index to be used.

	    //assert(!SubTreeRoot->hasChild());
		//QueryTreeNodePtr LeftChild = SubTreeRoot->getChild(1);
		
		//std::string ScanAttr = boost::get<std::string>(LeftChild->getAttr());
			
		
		    break;
		}
	case	INDEX_SCAN:
		{
		    break;
		}
	case	HASH_SCAN:
		{
		    break;
		}
	case	SELECT:
		{
			//SELECT node should have only one child
            assert(SubTreeRoot->hasChild(1));
			assert(!SubTreeRoot->hasChild(2));
			
			
			std::string Condition = boost::get<std::string>(SubTreeRoot->getAttr());
			std::string TableInCond;
			std::string AttrInCond;
			std::string token;
			int CondType;  //0 equal;  1 not equal

			QueryTreeNodePtr LeftChild = SubTreeRoot->getChild(1);

			if(LeftChild->getType() == SCAN)
			{
		        std::string TableName = boost::get<std::string>(LeftChild->getAttr());
				
				//determine index(change the SCAN type if necessary), cost for this
				//SCAN node based on condition in the SELECT, the index availabe for 
				//this table. In addition to cost, the number of blocks also need to
				//be calculated.
				std::string::size_type lastPos = Condition.find_first_of(".=><", 0);
	
				std::string::size_type pos = Condition.find_first_of(delimiters, lastPos);
		/*		
				while (std::string::npos != pos || std::string::npos != lastPos)
				{
					// Found a token, add it to the vector.
					token = Condition.substr(lastPos, pos - lastPos);

					if (token.compare("table") == 0 || token.compare("TABLE") == 0 || token.compare("Table") == 0)
					{
						// read next token
						lastPos = dbSchema.find_first_not_of(delimiters, pos);
						pos = dbSchema.find_first_of(delimiters, lastPos);
						CurTabName = dbSchema.substr(lastPos, pos - lastPos);
						DbTable NewTable(CurTabName); //???
						TableList.push_back(NewTable);

					}

				     		
				}
				*/
				if(dbCatalog->GetCardi(TableName)/dbCatalog->GetBfr(TableName) <= 10)
				{
				}
				else
				switch(dbCatalog->GetIdx(TableName,AttrInCond))
				{
				case NONE_T:
					{
						if(dbCatalog->IsPk(TableName,AttrInCond))

					break;
					}
				case BTREE_T:
					break;
				case EHASH_T:
					break;
				case LHASH_T:
					break;
				}
				
				blocks = 
				//CondType
			}
			else 
			{
                blocks = CostCalcTree(LeftChild,dbCatalog);
			}
			
		    break;
		}
	case	JOIN:
		{
		    
			break;
		}
	case	UNION:
		break;
	case	PRODUCT:

        //there should be no PRODUCT in the optimized querytree, no need to handle
		break;
	case	PROJECT:
		break;
	}

	return blocks;

}