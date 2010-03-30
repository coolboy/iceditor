#pragma once
#include "DbTable.h"

using namespace std;
class DbCatalog
{
private:
	list<DbTable> TableList;   //this include all relevant information: schemas and the informations attached with them: index, cardinality, selectivity, etc

public:
	DbCatalog(string dbSchema, string dbIndexing, string dbConfig); //
	DbCatalog(void);
	int AddTable(string s);
	
	//DbTable* SearchTable(string tab_name);
    void SetCardi(string tab_name, long int cardi);
	void SetBfr(string tab_name, int bfr);
	void SetPk(string tab_name, string pk);
	void SetIdxBfr(string tab_name, string attr_name, int ibfr);
	void AddFks(string tab_name, string fk);
	void AddAtr(string tab_name, string n, Atr_Type t,int l);
    void AddAtr(string tab_name, string n, Atr_Type t);
	void SetSel(string tab_name, string attr_name,double d);
	void SetIdx(string tab_name, string attr_name, Idx_Type it, int ibfr);

	long int GetCardi(string tab_name);
	int GetBfr(string tab_name);
	string GetPk(string tab_name);

	int IsPk(string tab_name, string attr_name);
	double GetSel(string tab_name, string attr_name);
	Idx_Type GetIdx(string tab_name, string attr_name);
	int GetIdxBfr(string tab_name, string attr_name);
	std::vector<std::string> GetTables(string attr_name);
	//Idx_Type GetIdx(string tab_name, string attr_name);

	
	/* */

	~DbCatalog(void);
};
