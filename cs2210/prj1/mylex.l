%option noyywrap
%{
#include <stdlib.h>
#include <string.h>
#include "token.h"

extern int yycolumn, yyline, yylval;

char string_buf[MAX_BUF_LEN];
char *string_buf_ptr;
%}

%x comment
%x str

/* regular definitions */

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
times			"*"
void			"void"
eof				<EOF>

delim			[ \t]
whitespace		{delim}+
newline			[\n]

id				[a-zA-Z][a-zA-Z0-9]*
fid				[0-9][a-zA-Z0-9]*

%%

"/*"         BEGIN(comment);

<comment>[^*\n]*		yycolumn+=yyleng;/* eat anything that's not a '*' */
<comment>"*"[^*/\n]*	yycolumn+=yyleng;/* eat up '*'s not followed by '/'s */
<comment>\n				++yyline;yycolumn = 1;
<comment>"*/"			yycolumn+=yyleng;BEGIN(INITIAL);/*return to init state*/
<comment>.*{eof}		errorHandler(yyline, yycolumn, "Comment without closure!");

\'      string_buf_ptr = string_buf; yycolumn+=yyleng; BEGIN(str);

<str>\'        { /* saw closing quote - all done */
        BEGIN(INITIAL);//TODO : buffer handler
        yylval = putString(string_buf, 1);
        yycolumn+=yyleng;
        *string_buf_ptr = '\0';
		return (SCONSTnum);
        /* return string constant token type and
         * value to parser
         */
        }

<str>\\n  *string_buf_ptr++ = '\n';yycolumn+=yyleng;
<str>\\t  *string_buf_ptr++ = '\t';yycolumn+=yyleng;
<str>\\\\  *string_buf_ptr++ = '\\';yycolumn+=yyleng;
<str>\\\'  *string_buf_ptr++ = '\'';yycolumn+=yyleng;

<str>\\[^nt\\\']  {
	yycolumn+=yyleng;
	errorHandler(yyline, yycolumn, "Esacpe Error!");
	}

<str>[^\\\n\']+		{
		char *yptr = yytext;
		yycolumn += yyleng;
		while ( *yptr )
          *string_buf_ptr++ = *yptr++;
        }


{newline}	{yyline++; yycolumn=1;}

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
			
{fid}		{
				yycolumn += yyleng;
				errorHandler(yyline, yycolumn, "Illegal id\n");
				return (IDnum);
			}
.    {
		yycolumn += yyleng;
		errorHandler(yyline, yycolumn, "Unrecognised character\n");
	 }

%%

int yyline = 1;
int yycolumn = 1;//last+1 of that elem
int yylval = -1;

int main(int argc, char** argv)
{
    ++argv, --argc;  /* skip over program name */
    if (argc > 0)
        yyin = fopen(argv[0], "r");
    else
        yyin = stdin;

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
    return 0;
}
