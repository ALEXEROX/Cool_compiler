
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "./Cool_code.y"

#include "ast.h"
#include "ast.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

extern int yylex(void);
void yyerror(const char *s);


/* Line 189 of yacc.c  */
#line 85 "Cool_code.tab.c"

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

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TYPEID = 258,
     OBJECTID = 259,
     STR_CONST = 260,
     INT_CONST = 261,
     CLASS = 262,
     INHERITS = 263,
     IF = 264,
     THEN = 265,
     ELSE = 266,
     FI = 267,
     WHILE = 268,
     LOOP = 269,
     POOL = 270,
     LET = 271,
     IN = 272,
     CASE = 273,
     OF = 274,
     ESAC = 275,
     NEW = 276,
     ISVOID = 277,
     TRUE = 278,
     FALSE = 279,
     NOT = 280,
     ASSIGN = 281,
     LE = 282,
     DARROW = 283,
     AND = 284,
     OR = 285,
     LETPREC = 286
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 31 "./Cool_code.y"

    char* str;
    int intval;
    struct ProgramNode* program;
    struct ClassNode* class_;
    struct FeatureNode* feature;
    struct FormalNode* formal;
    struct ExprNode* expr;
    struct LetBindingNode* let_binding;
    struct CaseNode* case_item;

    struct ClassList* class_list;
    struct FeatureList* feature_list;
    struct FormalList* formal_list;
    struct ExprList* expr_list;
    struct LetList* let_list;
    struct CaseList* case_list;
    struct BlockExprList* block_expr_list;



/* Line 214 of yacc.c  */
#line 174 "Cool_code.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 186 "Cool_code.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   457

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  17
/* YYNRULES -- Number of rules.  */
#define YYNRULES  59
/* YYNRULES -- Number of states.  */
#define YYNSTATES  146

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   286

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      39,    40,    31,    29,    41,    30,    35,    32,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    42,    43,
      33,    34,     2,     2,    36,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    37,     2,    38,    44,     2,     2,     2,
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
      25,    26,    27,    28,    45,    46,    47
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    17,    26,    27,    29,
      32,    34,    45,    50,    57,    58,    60,    64,    66,    70,
      74,    78,    82,    86,    90,    94,    98,   102,   106,   110,
     114,   116,   118,   120,   122,   124,   127,   130,   133,   140,
     149,   154,   162,   167,   171,   177,   183,   186,   192,   200,
     204,   210,   211,   213,   217,   219,   223,   226,   229,   231
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      49,     0,    -1,    50,    -1,    50,    51,    -1,    51,    -1,
       7,     3,    37,    52,    38,    43,    -1,     7,     3,     8,
       3,    37,    52,    38,    43,    -1,    -1,    53,    -1,    53,
      54,    -1,    54,    -1,     4,    39,    55,    40,    42,     3,
      37,    62,    38,    43,    -1,     4,    42,     3,    43,    -1,
       4,    42,     3,    26,    58,    43,    -1,    -1,    56,    -1,
      56,    41,    57,    -1,    57,    -1,     4,    42,     3,    -1,
      58,    46,    58,    -1,    58,    45,    58,    -1,    58,    29,
      58,    -1,    58,    30,    58,    -1,    58,    31,    58,    -1,
      58,    32,    58,    -1,    58,    33,    58,    -1,    58,    27,
      58,    -1,    58,    34,    58,    -1,    39,    58,    40,    -1,
       4,    26,    58,    -1,     4,    -1,     6,    -1,     5,    -1,
      23,    -1,    24,    -1,    44,    58,    -1,    22,    58,    -1,
      25,    58,    -1,    58,    35,     4,    39,    60,    40,    -1,
      58,    36,     3,    35,     4,    39,    60,    40,    -1,     4,
      39,    60,    40,    -1,     9,    58,    10,    58,    11,    58,
      12,    -1,    16,    59,    17,    58,    -1,    37,    62,    38,
      -1,    18,    58,    19,    63,    20,    -1,    13,    58,    14,
      58,    15,    -1,    21,     3,    -1,    59,    41,     4,    42,
       3,    -1,    59,    41,     4,    42,     3,    26,    58,    -1,
       4,    42,     3,    -1,     4,    42,     3,    26,    58,    -1,
      -1,    61,    -1,    61,    41,    58,    -1,    58,    -1,    62,
      58,    43,    -1,    58,    43,    -1,    63,    64,    -1,    64,
      -1,     4,    42,     3,    28,    58,    43,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    71,    71,    74,    75,    78,    79,    82,    83,    86,
      87,    90,    91,    92,    95,    96,    99,   100,   103,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   137,   138,   139,
     140,   143,   144,   147,   148,   151,   152,   155,   156,   159
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TYPEID", "OBJECTID", "STR_CONST",
  "INT_CONST", "CLASS", "INHERITS", "IF", "THEN", "ELSE", "FI", "WHILE",
  "LOOP", "POOL", "LET", "IN", "CASE", "OF", "ESAC", "NEW", "ISVOID",
  "TRUE", "FALSE", "NOT", "ASSIGN", "LE", "DARROW", "'+'", "'-'", "'*'",
  "'/'", "'<'", "'='", "'.'", "'@'", "'{'", "'}'", "'('", "')'", "','",
  "':'", "';'", "'~'", "AND", "OR", "LETPREC", "$accept", "program",
  "class_list", "class", "feature_list_e", "feature_list", "feature",
  "formal_list_e", "formal_list", "formal", "expr", "let_list",
  "expr_list_e", "expr_list", "block_expr_list", "case_list", "case_item", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,    43,
      45,    42,    47,    60,    61,    46,    64,   123,   125,    40,
      41,    44,    58,    59,   126,   284,   285,   286
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    48,    49,    50,    50,    51,    51,    52,    52,    53,
      53,    54,    54,    54,    55,    55,    56,    56,    57,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    59,    59,    59,
      59,    60,    60,    61,    61,    62,    62,    63,    63,    64
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     6,     8,     0,     1,     2,
       1,    10,     4,     6,     0,     1,     3,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     2,     2,     2,     6,     8,
       4,     7,     4,     3,     5,     5,     2,     5,     7,     3,
       5,     0,     1,     3,     1,     3,     2,     2,     1,     6
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     4,     0,     1,     3,     0,     7,
       0,     0,     0,     8,    10,     7,    14,     0,     0,     9,
       0,     0,     0,    15,    17,     0,     5,     0,     0,     0,
       0,     0,    12,     6,    18,     0,    16,    30,    32,    31,
       0,     0,     0,     0,     0,     0,    33,    34,     0,     0,
       0,     0,     0,     0,     0,    51,     0,     0,     0,     0,
       0,    46,    36,    37,     0,     0,     0,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    13,     0,     0,
       0,    29,    54,     0,    52,     0,     0,     0,     0,     0,
       0,    56,    43,     0,    28,    26,    21,    22,    23,    24,
      25,    27,     0,     0,    20,    19,     0,    40,     0,     0,
       0,    49,    42,     0,     0,     0,    58,    55,    51,     0,
       0,    53,     0,    45,     0,     0,     0,    44,    57,     0,
       0,    11,     0,    50,    47,     0,    38,    51,    41,     0,
       0,     0,    48,     0,    39,    59
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     3,     4,    12,    13,    14,    22,    23,    24,
      82,    59,    83,    84,    65,   115,   116
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -116
static const yytype_int16 yypact[] =
{
      18,     8,     7,    18,  -116,    -7,  -116,  -116,    24,    25,
      -4,   -26,     6,    25,  -116,    25,    41,    43,     9,  -116,
      12,    11,    16,    10,  -116,   -22,  -116,    15,    56,    19,
      41,   121,  -116,  -116,  -116,    57,  -116,   -11,  -116,  -116,
     121,   121,    61,   121,    63,   121,  -116,  -116,   121,   121,
     121,   121,   273,    31,   121,   121,   137,   211,    27,   -15,
     253,  -116,   -30,   -30,   293,    58,   313,   -30,   121,   121,
     121,   121,   121,   121,   121,    66,    69,  -116,   121,   121,
     121,   373,   373,    33,    37,   121,   121,    81,   121,    82,
      83,  -116,  -116,   333,  -116,   411,     0,     0,   -30,   -30,
     411,   411,    46,    53,   393,   421,    94,  -116,   121,   163,
     233,    64,  -116,    47,    50,     4,  -116,  -116,   121,    90,
      62,   373,   121,  -116,   121,    98,   101,  -116,  -116,    71,
      67,  -116,   188,   373,    87,    86,  -116,   121,  -116,   121,
     121,    80,   373,   353,  -116,  -116
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -116,  -116,  -116,   118,   107,  -116,   110,  -116,  -116,    99,
     -31,  -116,  -115,  -116,    44,  -116,    13
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_int16 yytable[] =
{
      52,     8,    88,   129,    31,    75,    76,     6,   114,    56,
      57,     5,    60,    16,    62,    54,    17,    63,    64,    66,
      67,    32,   141,    81,   127,     1,    89,    10,    55,    11,
       9,    71,    72,    15,    93,    75,    76,    95,    96,    97,
      98,    99,   100,   101,    18,    21,    25,   104,   105,    64,
      27,    30,    26,    28,   109,   110,    29,   112,    33,    34,
      53,    35,    37,    38,    39,    58,    61,    40,    80,    87,
     102,    41,   103,   107,    42,    93,    43,   121,   108,    44,
      45,    46,    47,    48,   111,   118,   113,   114,   119,   125,
     124,   132,   126,   133,   130,    49,    92,    50,    37,    38,
      39,   134,    51,    40,   135,   131,   137,    41,   142,   143,
      42,   136,    43,   139,   140,    44,    45,    46,    47,    48,
     144,     7,    20,    19,   106,    37,    38,    39,   128,    36,
      40,    49,   120,    50,    41,     0,     0,    42,    51,    43,
       0,     0,    44,    45,    46,    47,    48,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,     0,
      50,     0,     0,     0,    68,    51,    69,    70,    71,    72,
      73,    74,    75,    76,   122,     0,     0,     0,     0,     0,
       0,     0,    78,    79,     0,     0,     0,     0,     0,     0,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
     138,     0,     0,     0,     0,     0,     0,     0,    78,    79,
       0,     0,     0,     0,     0,    68,     0,    69,    70,    71,
      72,    73,    74,    75,    76,    86,     0,     0,     0,     0,
       0,     0,     0,    78,    79,     0,     0,     0,    68,     0,
      69,    70,    71,    72,    73,    74,    75,    76,   123,     0,
       0,     0,     0,     0,     0,     0,    78,    79,     0,     0,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,     0,    90,     0,     0,     0,     0,     0,    78,    79,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,     0,     0,     0,     0,     0,     0,     0,    78,    79,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,     0,     0,     0,     0,     0,    77,     0,    78,    79,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,     0,     0,     0,     0,     0,    91,     0,    78,    79,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,     0,     0,    94,     0,     0,     0,     0,    78,    79,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,     0,     0,     0,     0,     0,   117,     0,    78,    79,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,     0,     0,     0,     0,     0,   145,     0,    78,    79,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,     0,     0,     0,     0,     0,     0,     0,    78,    79,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,     0,     0,     0,     0,     0,     0,     0,    -1,    79,
      69,    70,    71,    72,    -1,    -1,    75,    76,    68,     0,
      69,    70,    71,    72,    73,    74,    75,    76
};

static const yytype_int16 yycheck[] =
{
      31,     8,    17,   118,    26,    35,    36,     0,     4,    40,
      41,     3,    43,    39,    45,    26,    42,    48,    49,    50,
      51,    43,   137,    54,    20,     7,    41,     3,    39,     4,
      37,    31,    32,    37,    65,    35,    36,    68,    69,    70,
      71,    72,    73,    74,    38,     4,     3,    78,    79,    80,
      38,    41,    43,    42,    85,    86,    40,    88,    43,     3,
       3,    42,     4,     5,     6,     4,     3,     9,    37,    42,
       4,    13,     3,    40,    16,   106,    18,   108,    41,    21,
      22,    23,    24,    25,     3,    39,     4,     4,    35,    42,
      26,   122,    42,   124,     4,    37,    38,    39,     4,     5,
       6,     3,    44,     9,     3,    43,    39,    13,   139,   140,
      16,    40,    18,    26,    28,    21,    22,    23,    24,    25,
      40,     3,    15,    13,    80,     4,     5,     6,   115,    30,
       9,    37,    38,    39,    13,    -1,    -1,    16,    44,    18,
      -1,    -1,    21,    22,    23,    24,    25,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,
      39,    -1,    -1,    -1,    27,    44,    29,    30,    31,    32,
      33,    34,    35,    36,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    29,    30,    31,
      32,    33,    34,    35,    36,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    27,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      27,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    45,    46,
      27,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,
      27,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    45,    46,
      27,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    45,    46,
      27,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      27,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    45,    46,
      27,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    45,    46,
      27,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,
      27,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    46,
      29,    30,    31,    32,    33,    34,    35,    36,    27,    -1,
      29,    30,    31,    32,    33,    34,    35,    36
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     7,    49,    50,    51,     3,     0,    51,     8,    37,
       3,     4,    52,    53,    54,    37,    39,    42,    38,    54,
      52,     4,    55,    56,    57,     3,    43,    38,    42,    40,
      41,    26,    43,    43,     3,    42,    57,     4,     5,     6,
       9,    13,    16,    18,    21,    22,    23,    24,    25,    37,
      39,    44,    58,     3,    26,    39,    58,    58,     4,    59,
      58,     3,    58,    58,    58,    62,    58,    58,    27,    29,
      30,    31,    32,    33,    34,    35,    36,    43,    45,    46,
      37,    58,    58,    60,    61,    10,    14,    42,    17,    41,
      19,    43,    38,    58,    40,    58,    58,    58,    58,    58,
      58,    58,     4,     3,    58,    58,    62,    40,    41,    58,
      58,     3,    58,     4,     4,    63,    64,    43,    39,    35,
      38,    58,    11,    15,    26,    42,    42,    20,    64,    60,
       4,    43,    58,    58,     3,     3,    40,    39,    12,    26,
      28,    60,    58,    58,    40,    43
};

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
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
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
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
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



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

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
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
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

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 71 "./Cool_code.y"
    {program = make_program((yyvsp[(1) - (1)].class_list)); (yyval.program) = program;;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 74 "./Cool_code.y"
    { (yyval.class_list) = append_class_list((yyvsp[(1) - (2)].class_list), (yyvsp[(2) - (2)].class_)); ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 75 "./Cool_code.y"
    { (yyval.class_list) = make_class_list((yyvsp[(1) - (1)].class_)); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 78 "./Cool_code.y"
    { (yyval.class_) = make_class((yyvsp[(2) - (6)].str), NULL, (yyvsp[(4) - (6)].feature_list)); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 79 "./Cool_code.y"
    { (yyval.class_) = make_class((yyvsp[(2) - (8)].str), (yyvsp[(4) - (8)].str), (yyvsp[(6) - (8)].feature_list)); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 82 "./Cool_code.y"
    { (yyval.feature_list) = NULL; ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 83 "./Cool_code.y"
    { (yyval.feature_list) = (yyvsp[(1) - (1)].feature_list); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 86 "./Cool_code.y"
    { (yyval.feature_list) = append_feature_list((yyvsp[(1) - (2)].feature_list), (yyvsp[(2) - (2)].feature)); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 87 "./Cool_code.y"
    { (yyval.feature_list) = make_feature_list((yyvsp[(1) - (1)].feature)); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 90 "./Cool_code.y"
    { (yyval.feature) = make_method((yyvsp[(1) - (10)].str), (yyvsp[(3) - (10)].formal_list), (yyvsp[(6) - (10)].str), make_block((yyvsp[(8) - (10)].block_expr_list))); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 91 "./Cool_code.y"
    { (yyval.feature) = make_attr((yyvsp[(1) - (4)].str), (yyvsp[(3) - (4)].str), NULL); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 92 "./Cool_code.y"
    { (yyval.feature) = make_attr((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].expr)); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 95 "./Cool_code.y"
    { (yyval.formal_list) = NULL; ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 96 "./Cool_code.y"
    { (yyval.formal_list) = (yyvsp[(1) - (1)].formal_list); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 99 "./Cool_code.y"
    { (yyval.formal_list) = append_formal_list((yyvsp[(1) - (3)].formal_list), (yyvsp[(3) - (3)].formal)); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 100 "./Cool_code.y"
    { (yyval.formal_list) = make_formal_list((yyvsp[(1) - (1)].formal)); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 103 "./Cool_code.y"
    { (yyval.formal) = make_formal((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str)); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 107 "./Cool_code.y"
    { (yyval.expr) = make_binop(OP_OR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 108 "./Cool_code.y"
    { (yyval.expr) = make_binop(OP_AND, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 109 "./Cool_code.y"
    { (yyval.expr) = make_binop(OP_PLUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 110 "./Cool_code.y"
    { (yyval.expr) = make_binop(OP_MINUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 111 "./Cool_code.y"
    { (yyval.expr) = make_binop(OP_MUL, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 112 "./Cool_code.y"
    { (yyval.expr) = make_binop(OP_DIV, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 113 "./Cool_code.y"
    { (yyval.expr) = make_binop(OP_LT, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 114 "./Cool_code.y"
    { (yyval.expr) = make_binop(OP_LE, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 115 "./Cool_code.y"
    { (yyval.expr) = make_binop(OP_EQ, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 116 "./Cool_code.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 117 "./Cool_code.y"
    { (yyval.expr) = make_assign((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 118 "./Cool_code.y"
    { (yyval.expr) = make_object((yyvsp[(1) - (1)].str)); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 119 "./Cool_code.y"
    { (yyval.expr) = make_int((yyvsp[(1) - (1)].intval)); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 120 "./Cool_code.y"
    { (yyval.expr) = make_string((yyvsp[(1) - (1)].str)); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 121 "./Cool_code.y"
    { (yyval.expr) = make_bool(true); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 122 "./Cool_code.y"
    { (yyval.expr) = make_bool(false); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 123 "./Cool_code.y"
    { (yyval.expr) = make_unop(OP_NEG, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 124 "./Cool_code.y"
    { (yyval.expr) = make_unop(OP_ISVOID, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 125 "./Cool_code.y"
    { (yyval.expr) = make_unop(OP_NOT, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 126 "./Cool_code.y"
    { (yyval.expr) = make_dispatch((yyvsp[(1) - (6)].expr), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].expr_list)); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 127 "./Cool_code.y"
    { (yyval.expr) = make_static_dispatch((yyvsp[(1) - (8)].expr), (yyvsp[(3) - (8)].str), (yyvsp[(5) - (8)].str), (yyvsp[(7) - (8)].expr_list)); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 128 "./Cool_code.y"
    { (yyval.expr) = make_dispatch(make_object((yyvsp[(1) - (4)].str)), (yyvsp[(1) - (4)].str), (yyvsp[(3) - (4)].expr_list)); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 129 "./Cool_code.y"
    { (yyval.expr) = make_if((yyvsp[(2) - (7)].expr), (yyvsp[(4) - (7)].expr), (yyvsp[(6) - (7)].expr)); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 130 "./Cool_code.y"
    { (yyval.expr) = make_let((yyvsp[(2) - (4)].let_list), (yyvsp[(4) - (4)].expr)); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 131 "./Cool_code.y"
    { (yyval.expr) = make_block((yyvsp[(2) - (3)].block_expr_list)); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 132 "./Cool_code.y"
    { (yyval.expr) = make_case((yyvsp[(2) - (5)].expr), (yyvsp[(4) - (5)].case_list)); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 133 "./Cool_code.y"
    { (yyval.expr) = make_while((yyvsp[(2) - (5)].expr), (yyvsp[(4) - (5)].expr)); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 134 "./Cool_code.y"
    { (yyval.expr) = make_new((yyvsp[(2) - (2)].str)); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 137 "./Cool_code.y"
    { (yyval.let_list) = append_let_list((yyvsp[(1) - (5)].let_list), make_let_list(make_let_binding((yyvsp[(3) - (5)].str), (yyvsp[(5) - (5)].str), NULL))); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 138 "./Cool_code.y"
    { (yyval.let_list) = append_let_list((yyvsp[(1) - (7)].let_list), make_let_list(make_let_binding((yyvsp[(3) - (7)].str), (yyvsp[(5) - (7)].str), (yyvsp[(7) - (7)].expr)))); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 139 "./Cool_code.y"
    { (yyval.let_list) = make_let_list(make_let_binding((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), NULL)); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 140 "./Cool_code.y"
    { (yyval.let_list) = make_let_list(make_let_binding((yyvsp[(1) - (5)].str), (yyvsp[(3) - (5)].str), (yyvsp[(5) - (5)].expr))); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 143 "./Cool_code.y"
    { (yyval.expr_list) = NULL; ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 144 "./Cool_code.y"
    { (yyval.expr_list) = (yyvsp[(1) - (1)].expr_list); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 147 "./Cool_code.y"
    { (yyval.expr_list) = append_expr_list((yyvsp[(1) - (3)].expr_list), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 148 "./Cool_code.y"
    { (yyval.expr_list) = make_expr_list((yyvsp[(1) - (1)].expr)); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 151 "./Cool_code.y"
    { (yyval.block_expr_list) = append_expr_list((yyvsp[(1) - (3)].block_expr_list), (yyvsp[(2) - (3)].expr)); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 152 "./Cool_code.y"
    { (yyval.block_expr_list) = make_expr_list((yyvsp[(1) - (2)].expr)); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 155 "./Cool_code.y"
    { (yyval.case_list) = append_case_list((yyvsp[(1) - (2)].case_list), (yyvsp[(2) - (2)].case_item)); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 156 "./Cool_code.y"
    { (yyval.case_list) = make_case_list((yyvsp[(1) - (1)].case_item)); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 159 "./Cool_code.y"
    { (yyval.case_item) = make_case_item((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].expr)); ;}
    break;



/* Line 1455 of yacc.c  */
#line 1960 "Cool_code.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

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
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
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


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 162 "./Cool_code.y"


void yyerror(const char *s) {
    fprintf(stderr, "Ошибка: %s\n", s);
}

