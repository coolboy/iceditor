#pragma once

#include <vector>
#include <string>

class Condition{
public:
	Condition(const std::string& text);

	std::string ltable_name;
	std::string lfield_name;

	std::string rtable_name;
	std::string rfield_name;
	std::string rtext;
	
	bool is_equ;
};

class ConditionTokenizer
{
public:
	enum Type{
		AND,
		OR,
		NOT
	};

	typedef std::vector<Condition> Conds;

public:
	ConditionTokenizer(const std::string& text);
	~ConditionTokenizer();

	Type getType(){return ty;}

	//! Retrieve a split token at the specified index
	//!
	//! \param[in] i The index to search for a token
	//! \return The token at the specified index
	//! \throw std::out_of_range If the index is invalid
	Condition& operator[] ( int i )
	{
		return conds.at ( i );
	}

	//! Retrieve the number of split tokens
	//!
	//! \return The number of split tokens
	int size() const
	{
		return conds.size();
	}

private:
	Type ty;
	Conds conds;
};
