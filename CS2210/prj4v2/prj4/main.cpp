#include "proj2.h"

#include <stdio.h>
#include <stdlib.h>

#include "CodeGen.h"

extern "C"{
extern int yycolumn,yyline;
extern FILE* treelst;
int yyparse();

extern tree Root;
};

int main()
{
	yyline = 1;
	yycolumn = 0;

	treelst = stdout;

	yyparse();

	CodeGen cg;
	cg.setAST(Root);

	std::string outAsm = cg.getMIPSCode();

	return 1;
}