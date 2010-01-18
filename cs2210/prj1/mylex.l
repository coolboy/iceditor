%option noyywrap
%{
#include <stdlib.h>
#include <string.h>
#include "token.h"

extern int yycolumn, yylength, yyline, yylval;
%}

/* regular definitions */

%{ /* (f)lex specification for comment*/
%}
A				[/]
B				[*]
C				[^/*]

and				"&&"
assgn			":="
declaration		"declarations"
dot				"."
enddeclarations	"enddeclarations"
equal			"="
gt				">"
int				"int"
lbrac			"["
lparen			"("
method			"method"
ne				"!="
or				"||"
program			"program"
rbrac			"]"
rparen			")"
semi			";"
val				"val"
while			"while"
class			"class"
comma			","
divide			"/"
else			"else"
eq				"=="
ge				">="
iconst			[0-9]+
if				"if"
lbrace			"{"
le				"<="
lt				"<"
minus			"-"
not				"!"
plus			"+"
rbrace			"}"
return			"return"
sconst			'[^']*'
times			"*"
void			"void"
eof				<EOF>

delim			[ \t]
whitespace		{delim}+
newline			[\n]
id				[a-zA-Z][a-zA-Z0-9]*

%%

{A}({A}.*|{B}({A}|{C}|{B}+{C})*{B}+{A}) /* skip comment */

{newline}	{yyline++; yycolumn=0;}

{delim}		{yycolumn+=yyleng;}

{and}		{
				yycolumn += yyleng;
				return (ANDnum);
			}
			
{assgn}		{
				yycolumn += yyleng;
				return (ASSGNnum);
			}
			
{declaration} {
				yycolumn += yyleng;
				return (DECLARATIONSnum);
			}
			
{dot}		{
				yycolumn += yyleng;
				return (DOTnum);
			}
			
{enddeclarations}	{
				yycolumn += yyleng;
				return (ENDDECLARATIONSnum);
			}
			
{equal}		{
				yycolumn += yyleng;
				return (EQUALnum);
			}
			
{gt}		{
				yycolumn += yyleng;
				return (GTnum);
			}
			
{int}		{
				yycolumn += yyleng;
				return (INTnum);
			}
			
{lbrac}		{
				yycolumn += yyleng;
				return (LBRACnum);
			}
			
{lparen}	{
				yycolumn += yyleng;
				return (LPARENnum);
			}
			
{method}	{
				yycolumn += yyleng;
				return (METHODnum);
			}
			
{ne}		{
				yycolumn += yyleng;
				return (NEnum);
			}
			
{or}		{
				yycolumn += yyleng;
				return (ORnum);
			}
			
{program}	{
				yycolumn += yyleng;
				return (PROGRAMnum);
			}
			
{rbrac}		{
				yycolumn += yyleng;
				return (RBRACnum);
			}
			
{rparen}	{
				yycolumn += yyleng;
				return (RPARENnum);
			}
			
{val}		{
				yycolumn += yyleng;
				return (VALnum);
			}
			
{while}		{
				yycolumn += yyleng;
				return (WHILEnum);
			}
						
{class}		{
				yycolumn += yyleng;
				return (CLASSnum);
			}
						
{comma}		{
				yycolumn += yyleng;
				return (COMMAnum);
			}
						
{divide}	{
				yycolumn += yyleng;
				return (DIVIDEnum);
			}
						
{else}		{
				yycolumn += yyleng;
				return (ELSEnum);
			}
						
{eq}		{
				yycolumn += yyleng;
				return (EQnum);
			}
						
{ge}		{
				yycolumn += yyleng;
				return (GEnum);
			}
						
{iconst}	{
				yycolumn += yyleng;
				return (ICONSTnum);
			}
						
{if}		{
				yycolumn += yyleng;
				return (IFnum);
			}
						
{lbrace}	{
				yycolumn += yyleng;
				return (LBRACEnum);
			}
						
{le}		{
				yycolumn += yyleng;
				return (LEnum);
			}
						
{lt}		{
				yycolumn += yyleng;
				return (LTnum);
			}
						
{minus}		{
				yycolumn += yyleng;
				return (MINUSnum);
			}
						
{not}		{
				yycolumn += yyleng;
				return (NOTnum);
			}
						
{plus}		{
				yycolumn += yyleng;
				return (PLUSnum);
			}
						
{rbrace}	{
				yycolumn += yyleng;
				return (RBRACEnum);
			}
						
{return}	{
				yycolumn += yyleng;
				return (RETURNnum);
			}
						
{sconst}	{
				yylval = putString(yytext, 1);
				yycolumn += yyleng;
				return (SCONSTnum);
			}
						
{times}		{
				yycolumn += yyleng;
				return (TIMESnum);
			}
						
{void}		{
				yycolumn += yyleng;
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
				yylval = putString(yytext, 0);
				yycolumn += yyleng;
				return (IDnum);
			}

%%

int yyline = 0;
int yycolumn = 0;
int yylval = -1;

main()
{
	printf("Line\tColumn\tToken\tIndex_in_String_table\n");
    while (1)
    {
       int lexReturn = yylex();
		       
       if (lexReturn == EOFnum)
       {	
		printf("\t\t%s\t\n", getTokenName(lexReturn));
		break;
	   }

	   if (yylval == -1)
		printf("%d\t%d\t%s\n", yyline, yycolumn, getTokenName(lexReturn));
       else
		printf("%d\t%d\t%s\t%d\n", yyline, yycolumn, getTokenName(lexReturn), yylval);
       
       yylval = -1;//reset
    }
    
    printTable();
}
