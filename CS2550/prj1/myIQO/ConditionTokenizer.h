#pragma once

#include <vector>
#include <string>

class Condition{
public:
	Condition(const std::string& text);
	bool isSameTable(const Condition& other){
		return ltable_name == other.ltable_name &&
			rtable_name == other.rtable_name;
	}
	bool operator==(const Condition& other){
		return ltable_name == other.ltable_name &&
			rtable_name == other.rtable_name && 
			lfield_name == other.lfield_name &&
			rfield_name == other.rfield_name &&
			rtext == other.rtext;
	}

	std::string ltable_name;
	std::string lfield_name;

	std::string rtable_name;
	std::string rfield_name;
	std::string rtext;
	//for dbg out
	std::string dbg_str;
	
	bool is_equ;
};

typedef std::vector<Condition> Conds;

class ConditionTokenizer
{
public:
	enum Type{
		AND,
		OR,
		NOT,
		ALONE
	};

public:
	ConditionTokenizer();
	ConditionTokenizer(const std::string& text);
	~ConditionTokenizer();

	Type getType(){return ty;}
	void setType(Type val){ty = val;}

	Conds getCons(){return conds;}

	void RemoveCon(Condition con);
	void AppendCon(Condition con);

	//void setCons(const Conds& val);

	std::string getStr();

	//! Retrieve a split token at the specified index
	//!
	//! \param[in] i The index to search for a token
	//! \return The token at the specified index
	//! \throw std::out_of_range If the index is invalid
	//Condition& operator[] ( int i )
	//{
		//return conds.at ( i );
	//}

	//! Retrieve the number of split tokens
	//!
	//! \return The number of split tokens
	//int size() const
	//{
		//return conds.size();
	//}

private:
	Type ty;
	Conds conds;
};
