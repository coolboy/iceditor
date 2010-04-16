#pragma once

#include <string>

class AsmRefiner
{
public:
	AsmRefiner(void);
	~AsmRefiner(void);

	void setDecafAsm(const std::string& dasm);
	std::string getMiniJavaAsm();

private:
	void transform();

private:
	std::string decafAsm_;
	std::string miniJavaAsm_;
};

