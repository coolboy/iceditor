#pragma once
class DbTable
{
private:
	string name;
	string pk;  //primary key(in case of compound key, I store all keys as a whole string)
	list<string> fks;  //foreign keys
	list<TabAtr> TableAttributes;  //attributes list
	long int cardinality;
	int bfr;

public:
	DbTable(string s);

	string GetName(void);
	string GetPk(void);
	long int GetCardi(void);
	int GetBfr(void);
	double GetSel(string s);
	Idx_Type GetIdx(string s);
	int GetIdxBfr(string s);
	int IsFk(string s);  // 1 mean fk, 0 mean not

	void SetName(string n);
	void SetPk(string s);
	void AddFks(string s);
	void AddAtr(string n, Atr_Type t,int l);
    void AddAtr(string n, Atr_Type t);
	int SetSel(string s,double d);
	int SetIdx(string s, Idx_Type it, int ibfr);
	
	~DbTable(void);
};

