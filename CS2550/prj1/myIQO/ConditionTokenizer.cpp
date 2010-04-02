#include "StdAfx.h"

#include <boost/algorithm/string.hpp> 
#include <boost/foreach.hpp>

#include "DbCatalog.h"
#include "ConditionTokenizer.h"

extern DbCatalog *dbCata;

//! Maintains a collection of substrings that are
//! delimited by a string of one or more characters
class Splitter {
	//! Contains the split tokens
	std::vector<std::string> _tokens;
public:
	//! Subscript type for use with operator[]
	typedef std::vector<std::string>::size_type size_type;
public:
	//! Create and initialize a new Splitter
	//!
	//! \param[in] src The string to split
	//! \param[in] delim The delimiter to split the string around
	Splitter ( const std::string& src, const std::string& delim )
	{
		reset ( src, delim );
	}

	//! Retrieve a split token at the specified index
	//!
	//! \param[in] i The index to search for a token
	//! \return The token at the specified index
	//! \throw std::out_of_range If the index is invalid
	std::string& operator[] ( size_type i )
	{
		return _tokens.at ( i );
	}

	//! Retrieve the number of split tokens
	//!
	//! \return The number of split tokens
	size_type size() const
	{
		return _tokens.size();
	}

	//! Re-initialize with a new source and delimiter
	//!
	//! \param[in] src The string to split
	//! \param[in] delim The delimiter to split the string around
	void reset ( const std::string& src, const std::string& delim )
	{
		std::vector<std::string> tokens;
		std::string::size_type start = 0;
		std::string::size_type end;

		for ( ; ; ) {
			end = src.find ( delim, start );
			tokens.push_back ( src.substr ( start, end - start ) );

			// We just copied the last token
			if ( end == std::string::npos )
				break;

			// Exclude the delimiter in the next search
			start = end + delim.size();
		}

		_tokens.swap ( tokens );
	}
};


ConditionTokenizer::ConditionTokenizer( const std::string& text )
{
	std::string::size_type end;

	end = text.find (" AND ", 0 );
	// If AND
	if ( end != std::string::npos )
	{
		Splitter Split ( text, " AND " );
		for ( Splitter::size_type i = 0; i < Split.size(); i++ )    
			conds.push_back (Condition(Split[i]) );
		ty = AND;
		return;
	}

	end = text.find (" OR ", 0 );
	// If OR
	if ( end != std::string::npos )
	{
		Splitter Split ( text, " OR " );
		for ( Splitter::size_type i = 0; i < Split.size(); i++ )    
			conds.push_back (Condition(Split[i]) );
		ty = OR;
		return;
	}

	end = text.find ("NOT ", 0 );
	// If NOT
	if ( end != std::string::npos )
	{
		Splitter Split ( text, "NOT " );
		for ( Splitter::size_type i = 0; i < Split.size(); i++ )    
			conds.push_back (Condition(Split[i]) );
		ty = NOT;
		return;
	}

	ty = ALONE;
	conds.push_back (Condition(text) );

}

ConditionTokenizer::~ConditionTokenizer(){ }

std::string ConditionTokenizer::getStr()
{
	std::string ret;
	BOOST_FOREACH (const Condition& val, conds){
		ret += val.dbg_str + " ";
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
Condition::Condition( const std::string& text ) :is_equ(false)
{
	std::vector<std::string> fields; 
	boost::split(fields, text, boost::is_any_of("<>=")); 

	is_equ = (text.find ( "=", 0 ) != std::string::npos);

	if (fields[0].find_first_of('.') != std::string::npos)
	{
		std::vector<std::string> names; 
		boost::split(names, fields[0], boost::is_any_of("."));
		ltable_name = names[0];
		lfield_name = names[1];
	}
	else
	{
		lfield_name = fields[0];
		ltable_name = *dbCata->GetTables(lfield_name).begin();
	}

	
	if (fields[1].find_first_of('\'') != std::string::npos)
	{
		rtext.assign(fields[1].begin() + 1, fields[1].begin() + fields[1].size() - 1);
	}
	else if (fields[1].find_first_of('.') != std::string::npos)
	{
		std::vector<std::string> names; 
		boost::split(names, fields[1], boost::is_any_of(".")); 
		rtable_name = names[0];
		rfield_name = names[1];
	}
	else
	{
		rfield_name = fields[1];
		rtable_name = *dbCata->GetTables(rfield_name).begin();
	}

	dbg_str = text;
}