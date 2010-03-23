#include "StdAfx.h"
#include "DbCatalog.h"


DbCatalog::DbCatalog(string dbSchema, string dbIndexing, string dbConfig)
{
    
	string delimiters = " \t\n(:->,";
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
			SetPk(CurTabName,token);
		}

		else if(token.compare("FK")==0 || token.compare("fk")==0 )
		{
		    lastPos = dbSchema.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
            pos = dbSchema.find_first_of(delimiters, lastPos);
			token = dbSchema.substr(lastPos, pos - lastPos);
		    AddFks(CurTabName,token);

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
			AddAtr(CurTabName, attr, type, 4);
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
   return 0;
}

/*
DbTable* DbCatalog::SearchTable(string tab_name, )
{
   list<DbTable>::iterator tab_iter;
   for(tab_iter=TableList.begin(); tab_iter!=TableList.end(); ++tab_iter)
   {
       if (tab_iter->GetName() == tab_name)
		   return tab_iter;
   }
   return NULL;
}
*/


long int DbCatalog::GetCardi(string tab_name)
{
   return 0;
}

int DbCatalog::GetBfr(string tab_name)
{
   return 0;
}

string DbCatalog::GetPk(string tab_name)
{
   return " ";
}

int DbCatalog::IsPk(string tab_name, string attr_name)
{
   return 0;
}

double DbCatalog::GetSel(string tab_name, string attr_name)
{
   return 0;
}

Idx_Type DbCatalog::GetIdx(string tab_name, string attr_name)
{
   return NONE_T;
}

int DbCatalog::GetIdxBfr(string tab_name, string attr_name)
{
   return 0;
}


void DbCatalog::SetCardi(string tab_name, long int cardi)
{
   list<DbTable>::iterator tab_iter;
   for(tab_iter=TableList.begin(); tab_iter!=TableList.end(); ++tab_iter)
   {
       if (tab_iter->GetName() == tab_name)
		   tab_iter->SetCardi(cardi);
   }
}

void DbCatalog::SetBfr(string tab_name, int bfr)
{
   list<DbTable>::iterator tab_iter;
   for(tab_iter=TableList.begin(); tab_iter!=TableList.end(); ++tab_iter)
   {
       if (tab_iter->GetName() == tab_name)
		   tab_iter->SetBfr(bfr);
   }
}

void DbCatalog::SetPk(string tab_name, string pk)
{
   list<DbTable>::iterator tab_iter;
   for(tab_iter=TableList.begin(); tab_iter!=TableList.end(); ++tab_iter)
   {
       if (tab_iter->GetName() == tab_name)
		   tab_iter->SetPk(pk);
   }
}

void DbCatalog::SetIdxBfr(string tab_name, string attr_name, int ibfr)
{
   list<DbTable>::iterator tab_iter;
   list<TabAtr>::iterator attr_iter;
   for(tab_iter=TableList.begin(); tab_iter!=TableList.end(); ++tab_iter)
   {
       if (tab_iter->GetName() == tab_name)
	      for(attr_iter=tab_iter->TableAttributes.begin(); attr_iter!=tab_iter->TableAttributes.end(); ++attr_iter)
          {
             if (attr_iter->GetName() == attr_name)
				 attr_iter->SetIdxBfr(ibfr);
          }
		   
   }
}

void DbCatalog::AddFks(string tab_name, string fk)
{
   list<DbTable>::iterator tab_iter;
   for(tab_iter=TableList.begin(); tab_iter!=TableList.end(); ++tab_iter)
   {
       if (tab_iter->GetName() == tab_name)
		   tab_iter->AddFks(fk);
   }
}

void DbCatalog::AddAtr(string tab_name, string n, Atr_Type t,int l)
{
   list<DbTable>::iterator tab_iter;
   for(tab_iter=TableList.begin(); tab_iter!=TableList.end(); ++tab_iter)
   {
       if (tab_iter->GetName() == tab_name)
		   tab_iter->AddAtr(n,t,l);
   }
}

void DbCatalog::AddAtr(string tab_name, string n, Atr_Type t)
{
   list<DbTable>::iterator tab_iter;
   for(tab_iter=TableList.begin(); tab_iter!=TableList.end(); ++tab_iter)
   {
       if (tab_iter->GetName() == tab_name)
		   tab_iter->AddAtr(n,t);
   }
}

void DbCatalog::SetSel(string tab_name, string attr_name,double d)
{
   list<DbTable>::iterator tab_iter;
   list<TabAtr>::iterator attr_iter;
   for(tab_iter=TableList.begin(); tab_iter!=TableList.end(); ++tab_iter)
   {
       if (tab_iter->GetName() == tab_name)
	      for(attr_iter=tab_iter->TableAttributes.begin(); attr_iter!=tab_iter->TableAttributes.end(); ++attr_iter)
          {
             if (attr_iter->GetName() == attr_name)
				 attr_iter->SetSel(d);
          }
		   
   }
}

void DbCatalog::SetIdx(string tab_name, string attr_name, Idx_Type it, int ibfr)
{
   list<DbTable>::iterator tab_iter;
   list<TabAtr>::iterator attr_iter;
   for(tab_iter=TableList.begin(); tab_iter!=TableList.end(); ++tab_iter)
   {
       if (tab_iter->GetName() == tab_name)
	      for(attr_iter=tab_iter->TableAttributes.begin(); attr_iter!=tab_iter->TableAttributes.end(); ++attr_iter)
          {
             if (attr_iter->GetName() == attr_name)
			 {
				attr_iter->SetIdx(it);
		        attr_iter->SetIdxBfr(ibfr);
			 }
          }
		   
   }
}


DbCatalog::~DbCatalog(void)
{
}
