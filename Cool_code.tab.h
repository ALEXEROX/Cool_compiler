
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
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



/* Line 1676 of yacc.c  */
#line 105 "Cool_code.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


