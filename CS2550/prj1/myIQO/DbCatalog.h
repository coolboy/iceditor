#pragma once
class DbCatalog
{
private:
	list<DbTable> TableList;   //this include all relevant information: schemas and the informations attached with them: index, cardinality, selectivity, etc

public:
	DbCatalog(const char* dbSchema, const char* dbIndexing, const char* dbConfig); //
	DbCatalog(void);
	int AddTable(string s);
	
	DbTable* SearchTable(string tab_name);
	long int GetCardi(string tab_name);
	int GetBfr(string tab_name);
	string GetPk(string tab_name);

	int IsPk(string tab_name, string attr_name);
	double GetSel(string tab_name, string attr_name);
	Idx_Type GetIdx(string tab_name, string attr_name);
	int GetIdxBfr(string tab_name, string attr_name);
	//Idx_Type GetIdx(string tab_name, string attr_name);

	
	/* */

	~DbCatalog(void);
};
