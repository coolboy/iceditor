#pragma once

#include <string>

class MiniJava2Decaf
{
public:
	MiniJava2Decaf(void);
	~MiniJava2Decaf(void);

	void setMiniJava(const	std::string& miniJava);
	std::string getDecaf();

private:
	void transform();

private:
	std::string	miniJava_;
	std::string decaf_;
};

