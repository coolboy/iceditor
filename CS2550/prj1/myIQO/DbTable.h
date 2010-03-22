#pragma once
class DbTable
{
private:
	string name;
	string pk;  //primary key
	list<string> fks;  //foreign keys
	list<TabAtr> TableAttributes;  //attributes list
	long int cardinality;
	int bfr;

public:
	DbTable(string s);

	long int GetCardi(void);
	int GetBfr(void);
	double GetSel(string s,double d);
	Idx_Type GetIdx(string s);
	int GetIdxBfr(string s);

	void SetName(string n);
	void SetPk(string s);
	int AddFks(string s);
	int AddAtr(string n, Atr_Type t,int l);
	int AddAtr(string n, Atr_Type t);
	int SetSel(string s,double d);
	int SetIdx(string s, Idx_Type it, int ibfr);
	
	~DbTable(void);
};

