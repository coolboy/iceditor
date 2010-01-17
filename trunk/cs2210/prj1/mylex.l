%option noyywrap
%{
#include <stdlib.h>
#include <string.h>
#include "token.h"

extern int yycolumn, yylength, yyline, yylval;
%}

/* regular definitions */

%{ /* (f)lex specification */
%}
A [/]
B [*]
C [^/*]

and				&&
assgn			:=
declaration		declarations
dot				[.]
enddeclarations	enddeclarations
equal			=
gt				>
int				int
lbrac			[[]
lparen			[(]
method			method
ne				!=
or				\|\|
program			program
rbrac			[]]
rparen			[)]
semi			[;]
val				val
while			while
class			class
comma			[,]
divide			[/]
else			else
eq				==
ge				>=
iconst			[0-9]+
if				if
lbrace			[{]
le				<=
lt				[<]
minus			[-]
not				[!]
plus			[+]
rbrace			[}]
return			return
sconst			'[^'\n]*'
times			[*]
void			void
eof				<EOF>

delim			[ \t]
whitespace		{delim}+
newline			[\n]
id				[a-zA-Z][a-zA-Z0-9]*

%%

{A}({A}.*|{B}({A}|{C}|{B}+{C})*{B}+{A}) /* skip comment */

{newline}	{yyline++; yycolumn=0;}

{delim}		{++yycolumn;}

{and}		{
				yycolumn += strlen(yytext);
				return (ANDnum);
			}
			
{assgn}		{
				yycolumn += strlen(yytext);
				return (ASSGNnum);
			}
			
{declaration} {
				yycolumn += strlen(yytext);
				return (DECLARATIONSnum);
			}
			
{dot}		{
				yycolumn += strlen(yytext);
				return (DOTnum);
			}
			
{enddeclarations}	{
				yycolumn += strlen(yytext);
				return (ENDDECLARATIONSnum);
			}
			
{equal}		{
				yycolumn += strlen(yytext);
				return (EQUALnum);
			}
			
{gt}		{
				yycolumn += strlen(yytext);
				return (GTnum);
			}
			
{int}		{
				yycolumn += strlen(yytext);
				return (INTnum);
			}
			
{lbrac}		{
				yycolumn += strlen(yytext);
				return (LBRACnum);
			}
			
{lparen}	{
				yycolumn += strlen(yytext);
				return (LPARENnum);
			}
			
{method}	{
				yycolumn += strlen(yytext);
				return (METHODnum);
			}
			
{ne}		{
				yycolumn += strlen(yytext);
				return (NEnum);
			}
			
{or}		{
				yycolumn += strlen(yytext);
				return (ORnum);
			}
			
{program}	{
				yycolumn += strlen(yytext);
				return (PROGRAMnum);
			}
			
{rbrac}		{
				yycolumn += strlen(yytext);
				return (RBRACnum);
			}
			
{rparen}	{
				yycolumn += strlen(yytext);
				return (RPARENnum);
			}
			
{val}		{
				yycolumn += strlen(yytext);
				return (VALnum);
			}
			
{while}		{
				yycolumn += strlen(yytext);
				return (WHILEnum);
			}
						
{class}		{
				yycolumn += strlen(yytext);
				return (CLASSnum);
			}
						
{comma}		{
				yycolumn += strlen(yytext);
				return (COMMAnum);
			}
						
{divide}	{
				yycolumn += strlen(yytext);
				return (DIVIDEnum);
			}
						
{else}		{
				yycolumn += strlen(yytext);
				return (ELSEnum);
			}
						
{eq}		{
				yycolumn += strlen(yytext);
				return (EQnum);
			}
						
{ge}		{
				yycolumn += strlen(yytext);
				return (GEnum);
			}
						
{iconst}	{
				yycolumn += strlen(yytext);
				return (ICONSTnum);
			}
						
{if}		{
				yycolumn += strlen(yytext);
				return (IFnum);
			}
						
{lbrace}	{
				yycolumn += strlen(yytext);
				return (LBRACEnum);
			}
						
{le}		{
				yycolumn += strlen(yytext);
				return (LEnum);
			}
						
{lt}		{
				yycolumn += strlen(yytext);
				return (LTnum);
			}
						
{minus}		{
				yycolumn += strlen(yytext);
				return (MINUSnum);
			}
						
{not}		{
				yycolumn += strlen(yytext);
				return (NOTnum);
			}
						
{plus}		{
				yycolumn += strlen(yytext);
				return (PLUSnum);
			}
						
{rbrace}	{
				yycolumn += strlen(yytext);
				return (RBRACEnum);
			}
						
{return}	{
				yycolumn += strlen(yytext);
				return (RETURNnum);
			}
						
{sconst}	{
				yycolumn += strlen(yytext);
				return (SCONSTnum);
			}
						
{times}		{
				yycolumn += strlen(yytext);
				return (TIMESnum);
			}
						
{void}		{
				yycolumn += strlen(yytext);
				return (VOIDnum);
			}
						
{semi}		{
				++yycolumn;
				return (SEMInum);
			}
			
{eof}		{
				return (EOFnum);
			}
			
{id}		{
				yycolumn += strlen(yytext);
				return (IDnum);
			}

%%

int yyline = 0;
int yycolumn = 0;

main()
{
	printf("Line\tColumn\tToken\tIndex_in_String_table\n");
    while (1)
    {
       int lexReturn = yylex();

       printf("%d\t%d\t%s\t%d\n", yyline, yycolumn, getTokenName(lexReturn), 0);
       
       if (lexReturn == EOFnum)
		break;
    }
}
