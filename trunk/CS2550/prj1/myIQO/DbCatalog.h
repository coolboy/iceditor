#pragma once
class DbCatalog
{
private:
	list<DbTable> TableList;   //this include all relevant information: schemas and the informations attached with them: index, cardinality, selectivity, etc

public:
	DbCatalog(const char* dbSchema, const char* dbIndexing, const char* dbConfig); //
	DbCatalog(void);
	int AddTable(string s);

	~DbCatalog(void);
};
