/* A Bison parser, made by GNU Bison 1.875c.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_Void = 258,
     T_Bool = 259,
     T_Int = 260,
     T_Double = 261,
     T_String = 262,
     T_Class = 263,
     T_LessEqual = 264,
     T_GreaterEqual = 265,
     T_Equal = 266,
     T_NotEqual = 267,
     T_Dims = 268,
     T_And = 269,
     T_Or = 270,
     T_Null = 271,
     T_Extends = 272,
     T_This = 273,
     T_Interface = 274,
     T_Implements = 275,
     T_While = 276,
     T_For = 277,
     T_If = 278,
     T_Else = 279,
     T_Return = 280,
     T_Break = 281,
     T_New = 282,
     T_NewArray = 283,
     T_Print = 284,
     T_ReadInteger = 285,
     T_ReadLine = 286,
     T_Identifier = 287,
     T_StringConstant = 288,
     T_IntConstant = 289,
     T_DoubleConstant = 290,
     T_BoolConstant = 291,
     T_UnaryMinus = 292,
     T_Lower_Than_Else = 293
   };
#endif
#define T_Void 258
#define T_Bool 259
#define T_Int 260
#define T_Double 261
#define T_String 262
#define T_Class 263
#define T_LessEqual 264
#define T_GreaterEqual 265
#define T_Equal 266
#define T_NotEqual 267
#define T_Dims 268
#define T_And 269
#define T_Or 270
#define T_Null 271
#define T_Extends 272
#define T_This 273
#define T_Interface 274
#define T_Implements 275
#define T_While 276
#define T_For 277
#define T_If 278
#define T_Else 279
#define T_Return 280
#define T_Break 281
#define T_New 282
#define T_NewArray 283
#define T_Print 284
#define T_ReadInteger 285
#define T_ReadLine 286
#define T_Identifier 287
#define T_StringConstant 288
#define T_IntConstant 289
#define T_DoubleConstant 290
#define T_BoolConstant 291
#define T_UnaryMinus 292
#define T_Lower_Than_Else 293




/* Copy the first part of user declarations.  */
#line 6 "parser.y"


#include "scanner.h" // for yylex
#include "parser.h"
#include "errors.h"

void yyerror(const char *msg); // standard error-handling routine



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 20 "parser.y"
typedef union YYSTYPE {
    int integerConstant;
    bool boolConstant;
    char *stringConstant;
    double doubleConstant;
    char identifier[MaxIdentLen+1]; // +1 for terminating null
    Decl *decl;
    List<Decl*> *declList;
    Type *type;
    NamedType *cType;
    List<NamedType*> *cTypeList;
    FnDecl *fDecl;
    VarDecl *var;
    List<VarDecl*> *varList;
    Expr *expr;
    List<Expr*> *exprList;
    Stmt *stmt;
    List<Stmt*> *stmtList;
    LValue *lvalue;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 183 "y.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined (YYLTYPE) && ! defined (YYLTYPE_IS_DECLARED)
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 207 "y.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   define YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYLTYPE_IS_TRIVIAL) && YYLTYPE_IS_TRIVIAL \
             && defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  22
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   534

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  31
/* YYNRULES -- Number of rules. */
#define YYNRULES  94
/* YYNRULES -- Number of states. */
#define YYNSTATES  183

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   293

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    45,     2,     2,     2,    44,     2,     2,
      54,    55,    42,    40,    53,    41,    47,    43,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    50,
      38,    37,    39,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    48,     2,    56,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    51,     2,    52,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    46,    49
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    18,
      21,    24,    26,    28,    30,    32,    34,    37,    43,    47,
      48,    56,    59,    60,    63,    64,    68,    70,    73,    74,
      76,    78,    84,    90,    92,    93,    97,    99,   102,   107,
     110,   111,   114,   115,   118,   120,   127,   133,   143,   147,
     150,   156,   159,   161,   165,   170,   175,   182,   184,   185,
     187,   189,   191,   195,   199,   203,   207,   211,   215,   219,
     223,   227,   231,   235,   239,   243,   247,   251,   254,   257,
     261,   265,   270,   277,   279,   281,   283,   285,   287,   289,
     291,   292,   296,   298,   301
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      58,     0,    -1,    59,    -1,    59,    60,    -1,    60,    -1,
      66,    -1,    75,    -1,    61,    -1,    64,    -1,    62,    50,
      -1,    63,    32,    -1,     5,    -1,     4,    -1,     7,    -1,
       6,    -1,    32,    -1,    63,    13,    -1,    19,    32,    51,
      65,    52,    -1,    65,    72,    50,    -1,    -1,     8,    32,
      67,    68,    51,    70,    52,    -1,    17,    32,    -1,    -1,
      20,    69,    -1,    -1,    69,    53,    32,    -1,    32,    -1,
      70,    71,    -1,    -1,    61,    -1,    75,    -1,    63,    32,
      54,    73,    55,    -1,     3,    32,    54,    73,    55,    -1,
      74,    -1,    -1,    74,    53,    62,    -1,    62,    -1,    72,
      76,    -1,    51,    77,    78,    52,    -1,    77,    61,    -1,
      -1,    79,    78,    -1,    -1,    82,    50,    -1,    76,    -1,
      23,    54,    83,    55,    79,    87,    -1,    21,    54,    83,
      55,    79,    -1,    22,    54,    82,    50,    83,    50,    82,
      55,    79,    -1,    25,    83,    50,    -1,    25,    50,    -1,
      29,    54,    86,    55,    50,    -1,    26,    50,    -1,    32,
      -1,    83,    47,    32,    -1,    83,    48,    83,    56,    -1,
      32,    54,    85,    55,    -1,    83,    47,    32,    54,    85,
      55,    -1,    83,    -1,    -1,    80,    -1,    81,    -1,    84,
      -1,    80,    37,    83,    -1,    83,    40,    83,    -1,    83,
      41,    83,    -1,    83,    43,    83,    -1,    83,    42,    83,
      -1,    83,    44,    83,    -1,    83,    11,    83,    -1,    83,
      12,    83,    -1,    83,    38,    83,    -1,    83,    39,    83,
      -1,    83,     9,    83,    -1,    83,    10,    83,    -1,    83,
      14,    83,    -1,    83,    15,    83,    -1,    54,    83,    55,
      -1,    41,    83,    -1,    45,    83,    -1,    30,    54,    55,
      -1,    31,    54,    55,    -1,    27,    54,    32,    55,    -1,
      28,    54,    83,    53,    63,    55,    -1,    18,    -1,    34,
      -1,    36,    -1,    35,    -1,    33,    -1,    16,    -1,    86,
      -1,    -1,    86,    53,    83,    -1,    83,    -1,    24,    79,
      -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,    98,    98,   111,   112,   115,   116,   117,   118,   121,
     124,   127,   128,   129,   130,   131,   132,   135,   139,   141,
     144,   148,   150,   153,   155,   158,   160,   163,   164,   167,
     168,   172,   174,   178,   179,   182,   184,   187,   190,   194,
     195,   198,   199,   202,   203,   204,   206,   208,   210,   212,
     214,   216,   219,   220,   221,   224,   226,   230,   231,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   249,   251,   252,   253,   254,   256,   257,
     259,   260,   262,   264,   267,   268,   269,   270,   271,   274,
     275,   278,   279,   282,   283
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_Void", "T_Bool", "T_Int", "T_Double",
  "T_String", "T_Class", "T_LessEqual", "T_GreaterEqual", "T_Equal",
  "T_NotEqual", "T_Dims", "T_And", "T_Or", "T_Null", "T_Extends", "T_This",
  "T_Interface", "T_Implements", "T_While", "T_For", "T_If", "T_Else",
  "T_Return", "T_Break", "T_New", "T_NewArray", "T_Print", "T_ReadInteger",
  "T_ReadLine", "T_Identifier", "T_StringConstant", "T_IntConstant",
  "T_DoubleConstant", "T_BoolConstant", "'='", "'<'", "'>'", "'+'", "'-'",
  "'*'", "'/'", "'%'", "'!'", "T_UnaryMinus", "'.'", "'['",
  "T_Lower_Than_Else", "';'", "'{'", "'}'", "','", "'('", "')'", "']'",
  "$accept", "Program", "DeclList", "Decl", "VarDecl", "Variable", "Type",
  "IntfDecl", "IntfList", "ClassDecl", "OptExt", "OptImpl", "ImpList",
  "FieldList", "Field", "FnHeader", "Formals", "FormalList", "FnDecl",
  "StmtBlock", "VarDecls", "StmtList", "Stmt", "LValue", "Call", "OptExpr",
  "Expr", "Constant", "Actuals", "ExprList", "OptElse", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,    61,    60,    62,
      43,    45,    42,    47,    37,    33,   292,    46,    91,   293,
      59,   123,   125,    44,    40,    41,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    57,    58,    59,    59,    60,    60,    60,    60,    61,
      62,    63,    63,    63,    63,    63,    63,    64,    65,    65,
      66,    67,    67,    68,    68,    69,    69,    70,    70,    71,
      71,    72,    72,    73,    73,    74,    74,    75,    76,    77,
      77,    78,    78,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    80,    80,    80,    81,    81,    82,    82,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    84,    84,    84,    84,    84,    85,
      85,    86,    86,    87,    87
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     2,     5,     3,     0,
       7,     2,     0,     2,     0,     3,     1,     2,     0,     1,
       1,     5,     5,     1,     0,     3,     1,     2,     4,     2,
       0,     2,     0,     2,     1,     6,     5,     9,     3,     2,
       5,     2,     1,     3,     4,     4,     6,     1,     0,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     3,
       3,     4,     6,     1,     1,     1,     1,     1,     1,     1,
       0,     3,     1,     2,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,    12,    11,    14,    13,     0,     0,    15,     0,
       2,     4,     7,     0,     0,     8,     5,     0,     6,     0,
      22,     0,     1,     3,     9,    16,    10,    40,    37,    34,
       0,    24,    19,    34,    42,    36,     0,     0,    33,    21,
       0,     0,     0,     0,    88,    83,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,    87,    84,    86,
      85,     0,     0,     0,    39,    44,     0,    42,    59,    60,
       0,    57,    61,    10,    32,     0,    26,    23,    28,    17,
       0,     0,    31,     0,    58,     0,    52,    49,     0,    51,
       0,     0,     0,     0,     0,    90,    77,    78,     0,    38,
      41,     0,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    35,     0,
       0,     0,    18,     0,     0,     0,    48,     0,     0,    92,
       0,    79,    80,     0,    89,    76,    62,    72,    73,    68,
      69,    74,    75,    70,    71,    63,    64,    66,    65,    67,
      53,     0,    25,    20,    29,    27,    30,    58,     0,    58,
      81,     0,     0,     0,    55,    90,    54,    46,     0,    94,
       0,    91,    50,     0,    58,    58,    45,    82,    56,     0,
      93,    58,    47
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,     9,    10,    11,    12,    13,    36,    15,    42,    16,
      31,    41,    77,   120,   155,    17,    37,    38,    18,    65,
      34,    66,    67,    68,    69,    70,    71,    72,   133,   134,
     176
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -131
static const short yypact[] =
{
      92,     1,  -131,  -131,  -131,  -131,     6,     7,  -131,    32,
      92,  -131,  -131,   -10,    -1,  -131,  -131,    21,  -131,   -13,
      60,    34,  -131,  -131,  -131,  -131,    37,  -131,  -131,    69,
      54,    73,  -131,    69,   100,  -131,     4,    39,    49,  -131,
      71,    57,     2,    64,  -131,  -131,    55,    59,    61,   452,
      62,    90,   101,   102,   110,   111,    -2,  -131,  -131,  -131,
    -131,   480,   480,   480,  -131,  -131,    94,   387,   136,  -131,
     124,   292,  -131,  -131,  -131,    69,  -131,   132,  -131,  -131,
      58,   131,  -131,   480,   480,   480,   139,  -131,   232,  -131,
     162,   480,   480,   142,   143,   480,   -22,   -22,   148,  -131,
    -131,   480,  -131,   480,   480,   480,   480,   480,   480,   480,
     480,   480,   480,   480,   480,   480,   181,   480,  -131,   182,
      16,    37,  -131,   168,   167,   190,  -131,   163,   210,   292,
     -37,  -131,  -131,   171,   174,  -131,   292,    40,    40,   314,
     314,   354,   303,    40,    40,   105,   105,   -22,   -22,   -22,
     185,   128,  -131,  -131,  -131,  -131,  -131,   424,   480,   424,
    -131,    69,   480,   186,  -131,   480,  -131,  -131,   250,   211,
      -9,   292,  -131,   200,   480,   424,  -131,  -131,  -131,   201,
    -131,   424,  -131
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -131,  -131,  -131,   230,   -31,    -5,     0,  -131,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,   224,   234,  -131,   149,   251,
    -131,   214,  -130,  -131,  -131,   -82,   -48,  -131,   112,   191,
    -131
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -59
static const short yytable[] =
{
      14,    88,   124,    64,    25,     1,     2,     3,     4,     5,
      14,   -15,    25,    96,    97,    98,   162,    25,   163,     1,
       2,     3,     4,     5,    35,   116,   117,   167,    35,   169,
     -15,    26,    22,    19,     8,   123,    73,   125,    20,    21,
      24,    29,    80,   128,   129,   180,   177,   129,     8,   -59,
     -59,   182,    95,   136,    79,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   153,   151,
     118,    25,    27,     2,     3,     4,     5,    30,   -59,   -59,
     111,   112,   113,   114,   115,    32,    39,   116,   117,   154,
     121,    33,   179,    40,    74,     1,     2,     3,     4,     5,
       6,     8,    75,    76,     2,     3,     4,     5,    78,    83,
     168,     7,    89,    84,   171,    85,    44,   129,    45,    82,
      14,    46,    47,    48,     8,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,   103,   104,   105,
     106,    61,   107,   108,    90,    62,    99,   113,   114,   115,
     -58,    27,   116,   117,    63,    91,    92,   103,   104,   105,
     106,   170,   107,   108,    93,    94,   109,   110,   111,   112,
     113,   114,   115,   101,   102,   116,   117,   103,   104,   105,
     106,   122,   107,   108,   166,   119,   109,   110,   111,   112,
     113,   114,   115,    95,   127,   116,   117,   131,   132,   103,
     104,   105,   106,   135,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   150,   152,   116,   117,   158,   160,   103,
     104,   105,   106,   157,   107,   108,   164,   162,   109,   110,
     111,   112,   113,   114,   115,   175,   172,   116,   117,   165,
      23,   103,   104,   105,   106,   159,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   178,   181,   116,   117,   103,
     104,   105,   106,   161,   107,   108,    81,    43,    28,   156,
     109,   110,   111,   112,   113,   114,   115,   173,     0,   116,
     117,   100,   126,   130,     0,     0,     0,     0,   109,   110,
     111,   112,   113,   114,   115,     0,     0,   116,   117,     0,
     174,   103,   104,   105,   106,     0,   107,   108,     0,     0,
       0,     0,   103,   104,   105,   106,     0,   107,     0,     0,
       0,     0,     0,   103,   104,   -59,   -59,     0,     0,     0,
     109,   110,   111,   112,   113,   114,   115,     0,     0,   116,
     117,   109,   110,   111,   112,   113,   114,   115,     0,     0,
     116,   117,   109,   110,   111,   112,   113,   114,   115,     0,
       0,   116,   117,   103,   104,   105,   106,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   109,   110,   111,   112,   113,   114,   115,     0,
       0,   116,   117,    44,     0,    45,     0,     0,    46,    47,
      48,     0,    49,    50,    51,    52,    53,    54,    55,    86,
      57,    58,    59,    60,     0,     0,     0,     0,    61,     0,
       0,     0,    62,     0,     0,     0,     0,   -58,    27,     0,
      44,    63,    45,     0,     0,    46,    47,    48,     0,    49,
      50,    51,    52,    53,    54,    55,    86,    57,    58,    59,
      60,     0,     0,     0,     0,    61,     0,     0,    44,    62,
      45,     0,     0,     0,     0,    27,     0,     0,    63,    51,
      52,     0,    54,    55,    86,    57,    58,    59,    60,     0,
       0,     0,     0,    61,     0,     0,    44,    62,    45,     0,
       0,     0,    87,     0,     0,     0,    63,    51,    52,     0,
      54,    55,    86,    57,    58,    59,    60,     0,     0,     0,
       0,    61,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63
};

static const short yycheck[] =
{
       0,    49,    84,    34,    13,     3,     4,     5,     6,     7,
      10,    13,    13,    61,    62,    63,    53,    13,    55,     3,
       4,     5,     6,     7,    29,    47,    48,   157,    33,   159,
      32,    32,     0,    32,    32,    83,    32,    85,    32,    32,
      50,    54,    42,    91,    92,   175,    55,    95,    32,     9,
      10,   181,    54,   101,    52,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    52,   117,
      75,    13,    51,     4,     5,     6,     7,    17,    38,    39,
      40,    41,    42,    43,    44,    51,    32,    47,    48,   120,
      32,    54,   174,    20,    55,     3,     4,     5,     6,     7,
       8,    32,    53,    32,     4,     5,     6,     7,    51,    54,
     158,    19,    50,    54,   162,    54,    16,   165,    18,    55,
     120,    21,    22,    23,    32,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,     9,    10,    11,
      12,    41,    14,    15,    54,    45,    52,    42,    43,    44,
      50,    51,    47,    48,    54,    54,    54,     9,    10,    11,
      12,   161,    14,    15,    54,    54,    38,    39,    40,    41,
      42,    43,    44,    37,    50,    47,    48,     9,    10,    11,
      12,    50,    14,    15,    56,    53,    38,    39,    40,    41,
      42,    43,    44,    54,    32,    47,    48,    55,    55,     9,
      10,    11,    12,    55,    14,    15,    38,    39,    40,    41,
      42,    43,    44,    32,    32,    47,    48,    50,    55,     9,
      10,    11,    12,    55,    14,    15,    55,    53,    38,    39,
      40,    41,    42,    43,    44,    24,    50,    47,    48,    54,
      10,     9,    10,    11,    12,    55,    14,    15,    38,    39,
      40,    41,    42,    43,    44,    55,    55,    47,    48,     9,
      10,    11,    12,    53,    14,    15,    42,    33,    17,   120,
      38,    39,    40,    41,    42,    43,    44,   165,    -1,    47,
      48,    67,    50,    92,    -1,    -1,    -1,    -1,    38,    39,
      40,    41,    42,    43,    44,    -1,    -1,    47,    48,    -1,
      50,     9,    10,    11,    12,    -1,    14,    15,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    -1,    14,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    -1,    -1,    -1,
      38,    39,    40,    41,    42,    43,    44,    -1,    -1,    47,
      48,    38,    39,    40,    41,    42,    43,    44,    -1,    -1,
      47,    48,    38,    39,    40,    41,    42,    43,    44,    -1,
      -1,    47,    48,     9,    10,    11,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    39,    40,    41,    42,    43,    44,    -1,
      -1,    47,    48,    16,    -1,    18,    -1,    -1,    21,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    -1,    50,    51,    -1,
      16,    54,    18,    -1,    -1,    21,    22,    23,    -1,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    -1,    -1,    41,    -1,    -1,    16,    45,
      18,    -1,    -1,    -1,    -1,    51,    -1,    -1,    54,    27,
      28,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    16,    45,    18,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    54,    27,    28,    -1,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,    19,    32,    58,
      59,    60,    61,    62,    63,    64,    66,    72,    75,    32,
      32,    32,     0,    60,    50,    13,    32,    51,    76,    54,
      17,    67,    51,    54,    77,    62,    63,    73,    74,    32,
      20,    68,    65,    73,    16,    18,    21,    22,    23,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    41,    45,    54,    61,    76,    78,    79,    80,    81,
      82,    83,    84,    32,    55,    53,    32,    69,    51,    52,
      63,    72,    55,    54,    54,    54,    32,    50,    83,    50,
      54,    54,    54,    54,    54,    54,    83,    83,    83,    52,
      78,    37,    50,     9,    10,    11,    12,    14,    15,    38,
      39,    40,    41,    42,    43,    44,    47,    48,    62,    53,
      70,    32,    50,    83,    82,    83,    50,    32,    83,    83,
      86,    55,    55,    85,    86,    55,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      32,    83,    32,    52,    61,    71,    75,    55,    50,    55,
      55,    53,    53,    55,    55,    54,    56,    79,    83,    79,
      63,    83,    50,    85,    50,    24,    87,    55,    55,    82,
      79,    55,    79
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value, Location);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if defined (YYMAXDEPTH) && YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;
  (void) yylocationp;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yytype, yyvaluep, yylocationp)
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;
  (void) yylocationp;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  YYLTYPE *yylerrsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
  *++yylsp = yylloc;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, yylsp - yylen, yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 98 "parser.y"
    { 
                                      yylsp[0]; 
                                      Program *program = new Program(yyvsp[0].declList);
                                      // if no errors, advance to next phase
                                      if (ReportError::NumErrors() == 0) 
                                          program->CheckSemantics(); 
                                      // comment out prev line to skip semantic analysis
                                      if (ReportError::NumErrors() == 0) 
                                          program->Emit();
                                    }
    break;

  case 3:
#line 111 "parser.y"
    { (yyval.declList=yyvsp[-1].declList)->Append(yyvsp[0].decl); }
    break;

  case 4:
#line 112 "parser.y"
    { (yyval.declList = new List<Decl*>)->Append(yyvsp[0].decl); }
    break;

  case 6:
#line 116 "parser.y"
    { yyval.decl=yyvsp[0].fDecl; }
    break;

  case 7:
#line 117 "parser.y"
    { yyval.decl=yyvsp[0].var; }
    break;

  case 10:
#line 124 "parser.y"
    { yyval.var = new VarDecl(new Identifier(yylsp[0], yyvsp[0].identifier), yyvsp[-1].type); }
    break;

  case 11:
#line 127 "parser.y"
    { yyval.type = Type::intType; }
    break;

  case 12:
#line 128 "parser.y"
    { yyval.type = Type::boolType; }
    break;

  case 13:
#line 129 "parser.y"
    { yyval.type = Type::stringType; }
    break;

  case 14:
#line 130 "parser.y"
    { yyval.type = Type::doubleType; }
    break;

  case 15:
#line 131 "parser.y"
    { yyval.type = new NamedType(new Identifier(yylsp[0],yyvsp[0].identifier)); }
    break;

  case 16:
#line 132 "parser.y"
    { yyval.type = new ArrayType(Join(yylsp[-1], yylsp[0]), yyvsp[-1].type); }
    break;

  case 17:
#line 136 "parser.y"
    { yyval.decl = new InterfaceDecl(new Identifier(yylsp[-3], yyvsp[-3].identifier), yyvsp[-1].declList); }
    break;

  case 18:
#line 140 "parser.y"
    { (yyval.declList=yyvsp[-2].declList)->Append(yyvsp[-1].fDecl); }
    break;

  case 19:
#line 141 "parser.y"
    { yyval.declList = new List<Decl*>(); }
    break;

  case 20:
#line 145 "parser.y"
    { yyval.decl = new ClassDecl(new Identifier(yylsp[-5], yyvsp[-5].identifier), yyvsp[-4].cType, yyvsp[-3].cTypeList, yyvsp[-1].declList); }
    break;

  case 21:
#line 149 "parser.y"
    { yyval.cType = new NamedType(new Identifier(yylsp[0], yyvsp[0].identifier)); }
    break;

  case 22:
#line 150 "parser.y"
    { yyval.cType = NULL; }
    break;

  case 23:
#line 154 "parser.y"
    { yyval.cTypeList = yyvsp[0].cTypeList; }
    break;

  case 24:
#line 155 "parser.y"
    { yyval.cTypeList = new List<NamedType*>; }
    break;

  case 25:
#line 159 "parser.y"
    { (yyval.cTypeList=yyvsp[-2].cTypeList)->Append(new NamedType(new Identifier(yylsp[0], yyvsp[0].identifier))); }
    break;

  case 26:
#line 160 "parser.y"
    { (yyval.cTypeList=new List<NamedType*>)->Append(new NamedType(new Identifier(yylsp[0], yyvsp[0].identifier))); }
    break;

  case 27:
#line 163 "parser.y"
    { (yyval.declList=yyvsp[-1].declList)->Append(yyvsp[0].decl); }
    break;

  case 28:
#line 164 "parser.y"
    { yyval.declList = new List<Decl*>(); }
    break;

  case 29:
#line 167 "parser.y"
    { yyval.decl = yyvsp[0].var; }
    break;

  case 30:
#line 168 "parser.y"
    { yyval.decl = yyvsp[0].fDecl; }
    break;

  case 31:
#line 173 "parser.y"
    { yyval.fDecl = new FnDecl(new Identifier(yylsp[-3], yyvsp[-3].identifier), yyvsp[-4].type, yyvsp[-1].varList); }
    break;

  case 32:
#line 175 "parser.y"
    { yyval.fDecl = new FnDecl(new Identifier(yylsp[-3], yyvsp[-3].identifier), Type::voidType, yyvsp[-1].varList); }
    break;

  case 33:
#line 178 "parser.y"
    { yyval.varList = yyvsp[0].varList; }
    break;

  case 34:
#line 179 "parser.y"
    { yyval.varList = new List<VarDecl*>; }
    break;

  case 35:
#line 183 "parser.y"
    { (yyval.varList=yyvsp[-2].varList)->Append(yyvsp[0].var); }
    break;

  case 36:
#line 184 "parser.y"
    { (yyval.varList = new List<VarDecl*>)->Append(yyvsp[0].var); }
    break;

  case 37:
#line 187 "parser.y"
    { (yyval.fDecl=yyvsp[-1].fDecl)->SetFunctionBody(yyvsp[0].stmt); }
    break;

  case 38:
#line 191 "parser.y"
    { yyval.stmt = new StmtBlock(yyvsp[-2].varList, yyvsp[-1].stmtList); }
    break;

  case 39:
#line 194 "parser.y"
    { (yyval.varList=yyvsp[-1].varList)->Append(yyvsp[0].var); }
    break;

  case 40:
#line 195 "parser.y"
    { yyval.varList = new List<VarDecl*>; }
    break;

  case 41:
#line 198 "parser.y"
    { yyval.stmtList = yyvsp[0].stmtList; yyval.stmtList->InsertAt(yyvsp[-1].stmt, 0); }
    break;

  case 42:
#line 199 "parser.y"
    { yyval.stmtList = new List<Stmt*>; }
    break;

  case 43:
#line 202 "parser.y"
    { yyval.stmt = yyvsp[-1].expr; }
    break;

  case 45:
#line 205 "parser.y"
    { yyval.stmt = new IfStmt(yyvsp[-3].expr, yyvsp[-1].stmt, yyvsp[0].stmt); }
    break;

  case 46:
#line 207 "parser.y"
    { yyval.stmt = new WhileStmt(yyvsp[-2].expr, yyvsp[0].stmt); }
    break;

  case 47:
#line 209 "parser.y"
    { yyval.stmt = new ForStmt(yyvsp[-6].expr, yyvsp[-4].expr, yyvsp[-2].expr, yyvsp[0].stmt); }
    break;

  case 48:
#line 211 "parser.y"
    { yyval.stmt = new ReturnStmt(yylsp[-1], yyvsp[-1].expr); }
    break;

  case 49:
#line 213 "parser.y"
    { yyval.stmt = new ReturnStmt(yylsp[-1], new EmptyExpr()); }
    break;

  case 50:
#line 215 "parser.y"
    { yyval.stmt = new PrintStmt(yyvsp[-2].exprList); }
    break;

  case 51:
#line 216 "parser.y"
    { yyval.stmt = new BreakStmt(yylsp[-1]); }
    break;

  case 52:
#line 219 "parser.y"
    { yyval.lvalue = new FieldAccess(NULL, new Identifier(yylsp[0], yyvsp[0].identifier)); }
    break;

  case 53:
#line 220 "parser.y"
    { yyval.lvalue = new FieldAccess(yyvsp[-2].expr, new Identifier(yylsp[0], yyvsp[0].identifier)); }
    break;

  case 54:
#line 221 "parser.y"
    { yyval.lvalue = new ArrayAccess(Join(yylsp[-3], yylsp[0]), yyvsp[-3].expr, yyvsp[-1].expr); }
    break;

  case 55:
#line 225 "parser.y"
    { yyval.expr = new Call(Join(yylsp[-3],yylsp[0]), NULL, new Identifier(yylsp[-3],yyvsp[-3].identifier), yyvsp[-1].exprList); }
    break;

  case 56:
#line 227 "parser.y"
    { yyval.expr = new Call(Join(yylsp[-5],yylsp[0]), yyvsp[-5].expr, new Identifier(yylsp[-3],yyvsp[-3].identifier), yyvsp[-1].exprList); }
    break;

  case 57:
#line 230 "parser.y"
    { yyval.expr = yyvsp[0].expr; }
    break;

  case 58:
#line 231 "parser.y"
    { yyval.expr = new EmptyExpr(); }
    break;

  case 59:
#line 234 "parser.y"
    { yyval.expr = yyvsp[0].lvalue; }
    break;

  case 62:
#line 237 "parser.y"
    { yyval.expr = new AssignExpr(yyvsp[-2].lvalue, new Operator(yylsp[-1],"="), yyvsp[0].expr); }
    break;

  case 63:
#line 238 "parser.y"
    { yyval.expr = new ArithmeticExpr(yyvsp[-2].expr, new Operator(yylsp[-1], "+"), yyvsp[0].expr); }
    break;

  case 64:
#line 239 "parser.y"
    { yyval.expr = new ArithmeticExpr(yyvsp[-2].expr, new Operator(yylsp[-1], "-"), yyvsp[0].expr); }
    break;

  case 65:
#line 240 "parser.y"
    { yyval.expr = new ArithmeticExpr(yyvsp[-2].expr, new Operator(yylsp[-1],"/"), yyvsp[0].expr); }
    break;

  case 66:
#line 241 "parser.y"
    { yyval.expr = new ArithmeticExpr(yyvsp[-2].expr, new Operator(yylsp[-1],"*"), yyvsp[0].expr); }
    break;

  case 67:
#line 242 "parser.y"
    { yyval.expr = new ArithmeticExpr(yyvsp[-2].expr, new Operator(yylsp[-1],"%"), yyvsp[0].expr); }
    break;

  case 68:
#line 243 "parser.y"
    { yyval.expr = new EqualityExpr(yyvsp[-2].expr, new Operator(yylsp[-1],"=="), yyvsp[0].expr); }
    break;

  case 69:
#line 244 "parser.y"
    { yyval.expr = new EqualityExpr(yyvsp[-2].expr, new Operator(yylsp[-1],"!="), yyvsp[0].expr); }
    break;

  case 70:
#line 245 "parser.y"
    { yyval.expr = new RelationalExpr(yyvsp[-2].expr, new Operator(yylsp[-1],"<"), yyvsp[0].expr); }
    break;

  case 71:
#line 246 "parser.y"
    { yyval.expr = new RelationalExpr(yyvsp[-2].expr, new Operator(yylsp[-1],">"), yyvsp[0].expr); }
    break;

  case 72:
#line 248 "parser.y"
    { yyval.expr = new RelationalExpr(yyvsp[-2].expr, new Operator(yylsp[-1],"<="), yyvsp[0].expr); }
    break;

  case 73:
#line 250 "parser.y"
    { yyval.expr = new RelationalExpr(yyvsp[-2].expr, new Operator(yylsp[-1],">="), yyvsp[0].expr); }
    break;

  case 74:
#line 251 "parser.y"
    { yyval.expr = new LogicalExpr(yyvsp[-2].expr, new Operator(yylsp[-1],"&&"), yyvsp[0].expr); }
    break;

  case 75:
#line 252 "parser.y"
    { yyval.expr = new LogicalExpr(yyvsp[-2].expr, new Operator(yylsp[-1],"||"), yyvsp[0].expr); }
    break;

  case 76:
#line 253 "parser.y"
    { yyval.expr = yyvsp[-1].expr; }
    break;

  case 77:
#line 255 "parser.y"
    { yyval.expr = new ArithmeticExpr(new Operator(yylsp[-1],"-"), yyvsp[0].expr); }
    break;

  case 78:
#line 256 "parser.y"
    { yyval.expr = new LogicalExpr(new Operator(yylsp[-1],"!"), yyvsp[0].expr); }
    break;

  case 79:
#line 258 "parser.y"
    { yyval.expr = new ReadIntegerExpr(Join(yylsp[-2],yylsp[0])); }
    break;

  case 80:
#line 259 "parser.y"
    { yyval.expr = new ReadLineExpr(Join(yylsp[-2],yylsp[0])); }
    break;

  case 81:
#line 261 "parser.y"
    { yyval.expr = new NewExpr(Join(yylsp[-3],yylsp[0]),new NamedType(new Identifier(yylsp[-1],yyvsp[-1].identifier))); }
    break;

  case 82:
#line 263 "parser.y"
    { yyval.expr = new NewArrayExpr(Join(yylsp[-5],yylsp[0]),yyvsp[-3].expr, yyvsp[-1].type); }
    break;

  case 83:
#line 264 "parser.y"
    { yyval.expr = new This(yylsp[0]); }
    break;

  case 84:
#line 267 "parser.y"
    { yyval.expr = new IntConstant(yylsp[0],yyvsp[0].integerConstant); }
    break;

  case 85:
#line 268 "parser.y"
    { yyval.expr = new BoolConstant(yylsp[0],yyvsp[0].boolConstant); }
    break;

  case 86:
#line 269 "parser.y"
    { yyval.expr = new DoubleConstant(yylsp[0],yyvsp[0].doubleConstant); }
    break;

  case 87:
#line 270 "parser.y"
    { yyval.expr = new StringConstant(yylsp[0],yyvsp[0].stringConstant); }
    break;

  case 88:
#line 271 "parser.y"
    { yyval.expr = new NullConstant(yylsp[0]); }
    break;

  case 89:
#line 274 "parser.y"
    { yyval.exprList = yyvsp[0].exprList; }
    break;

  case 90:
#line 275 "parser.y"
    { yyval.exprList = new List<Expr*>; }
    break;

  case 91:
#line 278 "parser.y"
    { (yyval.exprList=yyvsp[-2].exprList)->Append(yyvsp[0].expr); }
    break;

  case 92:
#line 279 "parser.y"
    { (yyval.exprList = new List<Expr*>)->Append(yyvsp[0].expr); }
    break;

  case 93:
#line 282 "parser.y"
    { yyval.stmt = yyvsp[0].stmt; }
    break;

  case 94:
#line 284 "parser.y"
    { yyval.stmt = NULL; }
    break;


    }

/* Line 1000 of yacc.c.  */
#line 1785 "y.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;
  yylsp -= yylen;

  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }

  yylerrsp = yylsp;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {
		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
		 yydestruct (yystos[*yyssp], yyvsp, yylsp);
	       }
        }
      else
	{
	  YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
	  yydestruct (yytoken, &yylval, &yylloc);
	  yychar = YYEMPTY;
	  *++yylerrsp = yylloc;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

  yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  yylerrsp = yylsp;
  *++yylerrsp = yyloc;
  yylsp -= yylen;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
  YYLLOC_DEFAULT (yyloc, yylsp, yylerrsp - yylsp);
  *++yylsp = yyloc;

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 287 "parser.y"



/* Function: InitParser
 * --------------------
 * This function will be called before any calls to yyparse().  It is designed
 * to give you an opportunity to do anything that must be done to initialize
 * the parser (set global variables, configure starting state, etc.). One
 * thing it already does for you is assign the value of the global variable
 * yydebug that controls whether yacc prints debugging information about
 * parser actions (shift/reduce) and contents of state stack during parser.
 * If set to false, no information is printed. Setting it to true will give
 * you a running trail that might be helpful when debugging your parser.
 * Please be sure the variable is set to false when submitting your final
 * version.
 */
void InitParser()
{
   PrintDebug("parser", "Initializing parser");
   yydebug = false;
}

