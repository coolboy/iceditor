#include "StdAfx.h"
#include "DbCatalog.h"


DbCatalog::DbCatalog(string dbSchema, string dbIndexing, string dbConfig)
{
    
	string delimiters = " \t\n(:->";
	string token;
	string CurTabName;
	string attr;
	string lengh;
	Atr_Type type;
	/* process the dbSchema string */

	// Skip delimiters at beginning.
    string::size_type lastPos = dbSchema.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos = dbSchema.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        token = dbSchema.substr(lastPos, pos - lastPos);

		if (token.compare("table") == 0 || token.compare("TABLE") == 0 || token.compare("Table") == 0)
		{
		   // read next token
           lastPos = dbSchema.find_first_not_of(delimiters, pos);
           pos = dbSchema.find_first_of(delimiters, lastPos);
		   CurTabName = dbSchema.substr(lastPos, pos - lastPos);
		   DbTable NewTable(CurTabName); //???
		   TableList.push_back(NewTable);
           
		}

		else if(token.compare("constraints") == 0 || token.compare("CONSTRAINTS") == 0 || token.compare("Constraints") == 0)
		{
		   ;
		}

		else if(token.compare("PK")==0 || token.compare("pk")==0 )
		{
		    lastPos = dbSchema.find_first_not_of(delimiters, pos);
            pos = dbSchema.find_first_of("\n", lastPos);
			token = dbSchema.substr(lastPos, pos - lastPos);
			(SearchTable(CurTabName))->SetPk(token);
		}

		else if(token.compare("FK")==0 || token.compare("fk")==0 )
		{
		    lastPos = dbSchema.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
            pos = dbSchema.find_first_of(delimiters, lastPos);
			token = dbSchema.substr(lastPos, pos - lastPos);
			(SearchTable(CurTabName))->AddFks(token);

			lastPos = dbSchema.find_first_of("\n", pos);
            lastPos = dbSchema.find_first_not_of(delimiters, lastPos);  //read next line
            pos = dbSchema.find_first_of(delimiters, lastPos);
		    continue;

		}
		else
		{
		    attr = token;
			lastPos = dbSchema.find_first_not_of(" ,", pos);
            pos = dbSchema.find_first_of(" ,", lastPos);
			token = dbSchema.substr(lastPos, pos - lastPos);
			if(token.compare("char")==0 || token.compare("CHAR")==0)
			{
				type = CHAR_T;
			}
			else if(token.compare("int")==0 || token.compare("INT")==0)
			{
				type = INT_T;
			}
			else if(token.compare("date")==0 || token.compare("DATE")==0)
			{
				type = DATE_T;
			}
			else 
			{
				type = UNKNOWN_T;
			}

			// do not implement the lengh field, do we really need it?

			lastPos = dbSchema.find_first_of("\n", pos);
            lastPos = dbSchema.find_first_not_of(delimiters, lastPos);  //read next line
            pos = dbSchema.find_first_of(delimiters, lastPos);

			//AddAtr(attr, type, 4);  //fixed lengh, do we really need to read the lengh information?
			(SearchTable(CurTabName))->AddAtr(attr, type, 4);
		    continue;
		}
		
        // Skip delimiters.  Note the "not_of"
        lastPos = dbSchema.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = dbSchema.find_first_of(delimiters, lastPos);

    }

	/* process the dbIndexing string */


	/* process the dbConfig string */


}

DbCatalog::DbCatalog(void)
{
    
}

int DbCatalog::AddTable(string s)
{

}

DbTable* DbCatalog::SearchTable(string tab_name)
{

}

long int DbCatalog::GetCardi(string tab_name)
{

}

int DbCatalog::GetBfr(string tab_name)
{

}

string DbCatalog::GetPk(string tab_name)
{

}

int DbCatalog::IsPk(string tab_name, string attr_name)
{

}

double DbCatalog::GetSel(string tab_name, string attr_name)
{

}

Idx_Type DbCatalog::GetIdx(string tab_name, string attr_name)
{

}

int DbCatalog::GetIdxBfr(string tab_name, string attr_name)
{

}


DbCatalog::~DbCatalog(void)
{
}
