#include "proj2.h"

#include <stdio.h>
#include <stdlib.h>

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

	return 1;
}