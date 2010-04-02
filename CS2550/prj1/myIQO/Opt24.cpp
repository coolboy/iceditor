#include "StdAfx.h"
#include "QueryTree.h"
#include "Opt24.h"
#include "dbCatalog.h"
//#include <math.h>

using namespace client;

int CostCalcTree(QueryTreeNodePtr SubTreeRoot,  DbCatalog* dbCatalog)  //should be recursive
{
	std::string Linear = "LINEAR_SEARCH";
	std::string Binary = "BINARY_SEARCH";
	std::string Btree = "BTREE_SEARCH";
	std::string Extensible = "EXTENSIBLE_HASH";
	std::string Lhash = "LINEAR_HASH";
	std::string Njoin = "NESTED_LOOP_JOIN";
	std::string Sjoin = "SINGLE_LOOP_JOIN";
	std::string Pjoin = "PARTITION_HASH_JOIN";
	std::string Msort = "MERGE_SORT";
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
					if(dbCatalog->IsPk(TableName, AttrInCond) && CondType)
					{
					    costs =  dbCatalog->GetCardi(TableName)/(dbCatalog->GetBfr(TableName)*2);
						blocks = 1;
					}
					else
					{
					    costs =  dbCatalog->GetCardi(TableName)/dbCatalog->GetBfr(TableName);
						if(dbCatalog->GetSel(TableName,AttrInCond)>0 && dbCatalog->GetCardi(TableName)>0 && CondType)
						{
						   blocks = (dbCatalog->GetSel(TableName,AttrInCond)*dbCatalog->GetCardi(TableName))/
							   dbCatalog->GetBfr(TableName);
						}
						else
						   blocks = (int) costs/2;
					}

					LeftChild->setExInfo("Cost",costs);
				    LeftChild->setExInfo("Algorithm",Linear);

				}
				else
				switch(dbCatalog->GetIdx(TableName,AttrInCond))
				{
				case NONE_T:
					{
					if(dbCatalog->IsPk(TableName, AttrInCond) && CondType)
					{
					    costs =  dbCatalog->GetCardi(TableName)/(dbCatalog->GetBfr(TableName)*2);
						blocks = 1;
					}
					else
					{
						costs =  dbCatalog->GetCardi(TableName)/dbCatalog->GetBfr(TableName);
						if(dbCatalog->GetSel(TableName,AttrInCond)>0 && dbCatalog->GetCardi(TableName)>0 && CondType)
						{
						   blocks = (dbCatalog->GetSel(TableName,AttrInCond)*dbCatalog->GetCardi(TableName)
							   /dbCatalog->GetBfr(TableName));
						}
						else
						   blocks = (int) costs/2;
					}

					LeftChild->setExInfo("Cost",costs);
				    LeftChild->setExInfo("Algorithm",Linear);

					break;
					}
				case BTREE_T:
					{
					int btreeBft = dbCatalog->GetIdxBfr(TableName,AttrInCond);
					int level = (int) log((float) dbCatalog->GetCardi(TableName)/dbCatalog->GetBfr(TableName));
					
					if(dbCatalog->IsPk(TableName, AttrInCond) && CondType)
					{
					    costs =  level+1;
						blocks = 1;
					}
					else if(!(dbCatalog->IsPk(TableName, AttrInCond)) && CondType)
					{
                        if(dbCatalog->GetSel(TableName,AttrInCond)>0 && dbCatalog->GetCardi(TableName)>0)
						{
						   blocks = (dbCatalog->GetSel(TableName,AttrInCond)*dbCatalog->GetCardi(TableName)
							   /dbCatalog->GetBfr(TableName));
						   costs = blocks + level;
						}
						else
						{
							blocks =  dbCatalog->GetCardi(TableName)/(dbCatalog->GetBfr(TableName)*2);
							costs = blocks + level;
						}
				      
					}
					else if(!CondType)
					{
							blocks =  dbCatalog->GetCardi(TableName)/(dbCatalog->GetBfr(TableName)*2);
						    costs = blocks + level;
				          
					}
					LeftChild->setExInfo("Cost",costs);
					LeftChild->setExInfo("Algorithm",Btree);
					break;
					}
				case EHASH_T:
					{
					
					if(CondType)
					{
					    costs =  2;
						blocks = 1;
						LeftChild->setExInfo("Algorithm",Extensible);
					}

					else if(!CondType)
					{
					blocks =  dbCatalog->GetCardi(TableName)/(dbCatalog->GetBfr(TableName)*2);
					costs = 2*blocks;
					LeftChild->setExInfo("Algorithm",Linear);
				          
					}
					LeftChild->setExInfo("Cost",costs);
					
					break;
					}
				case LHASH_T:
					{
					if(CondType)
					{
					    costs =  1;
						blocks = 1;
						LeftChild->setExInfo("Algorithm",Lhash);
					}

					else if(!CondType)
					{
					blocks =  dbCatalog->GetCardi(TableName)/(dbCatalog->GetBfr(TableName)*2);
					costs = 2*blocks;
					LeftChild->setExInfo("Algorithm",Linear);
				          
					}
					LeftChild->setExInfo("Cost",costs);
					break;
					}
				}
				
				
				//CondType
			}
			else 
			{
                blocks = CostCalcTree(LeftChild,dbCatalog);
				
			}
			SubTreeRoot->setExInfo("Cost",costs);
			SubTreeRoot->setExInfo("Algorithm",LeftChild->getExInfo("Algorithm"));
		    break;
		}
	case	JOIN:
		{
		    assert(SubTreeRoot->hasChild(1));
			assert(SubTreeRoot->hasChild(2));
			costs = 0;
			int BuffSizeBlock = dbCatalog->GetBuffSize()/dbCatalog->GetPageSize();
			int CostsSingle, CostsNest, CostsHash;
			int BlocksLeft, BlocksRight, BlocksResult;
			Idx_Type LeftType; //only test if left attribute has an index for single loop join
			                   //during the join order optimization, left and right child will
			                   //be switched and their costs will be compared
			int IdxLev;
			std::string alg;
			std::string Condition = boost::get<std::string>(SubTreeRoot->getAttr());
			std::string TableInCondL,TableInCondR;
			std::string AttrInCondL,AttrInCondR;
			std::string token;

			QueryTreeNodePtr LeftChild = SubTreeRoot->getChild(1);
			LeftType = dbCatalog->GetIdx(TableInCondL, AttrInCondL);
			QueryTreeNodePtr RightChild = SubTreeRoot->getChild(2);

			BlocksLeft = CostCalcTree(LeftChild, dbCatalog);
			BlocksRight = CostCalcTree(RightChild, dbCatalog);
			
			if(dbCatalog->IsPk(TableInCondL,AttrInCondL))
			BlocksResult = BlocksRight;
			else if(dbCatalog->IsPk(TableInCondR,AttrInCondR))
			BlocksResult = BlocksLeft;
			else
            BlocksResult = BlocksLeft * BlocksRight/2;
			
			switch(LeftType)
			{
			case BTREE_T:
				{
				  IdxLev = log((float) dbCatalog->GetCardi(TableInCondL)/dbCatalog->GetBfr(TableInCondL));
				  CostsSingle = BlocksRight + BlocksRight*dbCatalog->GetBfr(TableInCondR)*(IdxLev-1);
				break;
				}
			case EHASH_T:
				{
				 IdxLev = 2;
				 CostsSingle = BlocksRight + BlocksRight*dbCatalog->GetBfr(TableInCondR)*(IdxLev-1);
				break;
				}
			case LHASH_T:
				{
				 IdxLev = 1;
				 CostsSingle = BlocksRight + BlocksRight*dbCatalog->GetBfr(TableInCondR)*(IdxLev-1);
				break;
				}
			case NONE_T:
				{
                 CostsSingle = 10000000;  //make it impossible to be used 
				break;
				}
			}
			

			CostsNest = BlocksRight + BlocksLeft*(BlocksRight/(BuffSizeBlock-2));
			
			CostsHash = 3*(BlocksLeft + BlocksRight);

			if(CostsNest >= CostsHash)
			{
				costs = CostsHash;
				alg = "PARTITION_HASH_JOIN";
			}
			else
			{
			   costs = CostsNest;
			   alg = "NESTED_LOOP_JOIN";
			}

			if(costs >= CostsSingle)
			{
			   costs = CostsSingle;
			   alg = "SINGLE_LOOP_JOIN";
			}
            
			blocks = BlocksResult;

			costs += blocks;

			SubTreeRoot->setExInfo("Cost",costs);
			SubTreeRoot->setExInfo("Algorithm",alg);

			break;
		}
	case	UNION:
		{
		    assert(SubTreeRoot->hasChild(1));
			assert(SubTreeRoot->hasChild(2));
						
			int BlocksLeft, BlocksRight, BlocksResult;

			QueryTreeNodePtr LeftChild = SubTreeRoot->getChild(1);
			QueryTreeNodePtr RightChild = SubTreeRoot->getChild(2);

			BlocksLeft = CostCalcTree(LeftChild, dbCatalog);
			BlocksRight = CostCalcTree(RightChild, dbCatalog);
			
			BlocksResult=BlocksLeft + BlocksRight;
			
			blocks = BlocksResult;
			costs = blocks;
		break;
		}
	case	PRODUCT:
		{
		    assert(SubTreeRoot->hasChild(1));
			assert(SubTreeRoot->hasChild(2));
			costs = 0;
			
			int BlocksLeft, BlocksRight, BlocksResult;

			QueryTreeNodePtr LeftChild = SubTreeRoot->getChild(1);
			QueryTreeNodePtr RightChild = SubTreeRoot->getChild(2);

			BlocksLeft = CostCalcTree(LeftChild, dbCatalog);
			BlocksRight = CostCalcTree(RightChild, dbCatalog);
			
			BlocksResult=BlocksLeft * BlocksRight;
			
			blocks = BlocksResult;
			costs = blocks;
        //there should be no PRODUCT in the optimized querytree, no need to handle
		break;
		}
	case	PROJECT:
		{
		    assert(SubTreeRoot->hasChild(1));
			assert(!SubTreeRoot->hasChild(2));
						
			std::string Condition = boost::get<std::string>(SubTreeRoot->getAttr());
			std::string TableInCond;
			std::string AttrInCond;
			std::string token;
			int CondType;  //0 equal;  1 not equal
			float ProjFrac =0.25; //The fraction of blokcs filtered by the project.

			// calculate the project fraction here:

			

			QueryTreeNodePtr LeftChild = SubTreeRoot->getChild(1);

			if(LeftChild->getType() != SCAN)
			{
			   blocks = CostCalcTree(SubTreeRoot->getChild(1), dbCatalog);

			   costs = 0;

	           blocks = blocks * ProjFrac;

			   SubTreeRoot->setExInfo("Algorithm",Msort);
			   SubTreeRoot->setExInfo("Cost",costs);
			}

			else if(LeftChild->getType() == SCAN)
			{
               int HasKey = 1;
			   std::string TableName = boost::get<std::string>(LeftChild->getAttr());

			   blocks = dbCatalog->GetCardi(TableName)/dbCatalog->GetBfr(TableName);
			   
			   
			   if(HasKey)
			   {
			      costs = blocks;
				  LeftChild->setExInfo("Algorithm",Linear);
			   }
			   else
			   {
				  costs = 4*blocks;
				  LeftChild->setExInfo("Algorithm",Msort);

			   }

			   blocks = blocks * ProjFrac;

			   LeftChild->setExInfo("Cost",costs);
			   
			   SubTreeRoot->setExInfo("Algorithm",LeftChild->getExInfo("Algorithm"));
			   SubTreeRoot->setExInfo("Cost",costs);
			}



			

		break;
		}
	}

	return blocks;

}