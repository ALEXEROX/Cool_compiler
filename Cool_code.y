%{
#include "ast.h"
#include "ast.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

extern int yylex(void);
void yyerror(const char *s);
%}

%token <str> TYPEID OBJECTID STR_CONST
%token <intval> INT_CONST
%token CLASS INHERITS IF THEN ELSE FI WHILE LOOP POOL LET IN CASE OF ESAC NEW ISVOID
%token TRUE FALSE NOT
%token ASSIGN LE DARROW
%token '+' '-' '*' '/' '<' '=' '.' '@'
%token '{' '}' '(' ')' ',' ':' ';' '~'
%token AND OR

%right ASSIGN
%left AND
%left OR
%nonassoc LE '<' '='
%left '+' '-'
%left '*' '/'
%right ISVOID '~' NOT
%left '.' '@'
%nonassoc LETPREC

%union {
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
}

%type <program> program
%type <class_list> class_list
%type <class_> class
%type <feature_list> feature_list feature_list_e
%type <feature> feature
%type <formal_list> formal_list formal_list_e
%type <formal> formal
%type <expr> expr
%type <expr_list> expr_list expr_list_e
%type <let_list> let_list
%type <case_list> case_list
%type <case_item> case_item
%type <block_expr_list> block_expr_list

6

%start program

%%

program: class_list {$$=make_program($1);}
       ;

class_list: class_list class { $$ = append_class_list($1, $2); }
		  | class { $$ = make_class_list($1); }
		  ;

class: CLASS TYPEID '{' feature_list_e '}' ';' { $$ = make_class($2, NULL, $4); }
	 | CLASS TYPEID INHERITS TYPEID '{' feature_list_e '}' ';' { $$ = make_class($2, $4, $6); }
	 ;

feature_list_e: /* empty */ { $$ = NULL; }
			  | feature_list { $$ = $1; }
			  ;

feature_list: feature_list feature { $$ = append_feature_list($1, $2); }
			| feature { $$ = make_feature_list($1); }
			;

feature: OBJECTID '(' formal_list_e ')' ':' TYPEID '{' block_expr_list '}' ';' { $$ = make_method($1, $3, $6, make_block($8)); }
	   | OBJECTID ':' TYPEID ';' { $$ = make_attr($1, $3, NULL); }
       | OBJECTID ':' TYPEID ASSIGN expr ';' { $$ = make_attr($1, $3, $5); }
       ;

formal_list_e: /* empty */ { $$ = NULL; }
			 | formal_list { $$ = $1; }
			 ;

formal_list: formal_list ',' formal { $$ = append_formal_list($1, $3); }
		   | formal { $$ = make_formal_list($1); }
		   ;

formal: OBJECTID ':' TYPEID { $$ = make_formal($1, $3); }
      ;


expr: expr OR expr { $$ = make_binop(OP_OR, $1, $3); }
	| expr AND expr { $$ = make_binop(OP_AND, $1, $3); }
	| expr '+' expr { $$ = make_binop(OP_PLUS, $1, $3); }
    | expr '-' expr { $$ = make_binop(OP_MINUS, $1, $3); }
    | expr '*' expr { $$ = make_binop(OP_MUL, $1, $3); }
    | expr '/' expr { $$ = make_binop(OP_DIV, $1, $3); }
    | expr '<' expr { $$ = make_binop(OP_LT, $1, $3); }
    | expr LE expr { $$ = make_binop(OP_LE, $1, $3); }
    | expr '=' expr { $$ = make_binop(OP_EQ, $1, $3); }
    | '(' expr ')' { $$ = $2; }
    | OBJECTID ASSIGN expr { $$ = make_assign($1, $3); }
    | OBJECTID { $$ = make_object($1); }
    | INT_CONST { $$ = make_int($1); }
    | STR_CONST { $$ = make_string($1); }
    | TRUE { $$ = make_bool(true); }
    | FALSE { $$ = make_bool(false); }
    | '~' expr { $$ = make_unop(OP_NEG, $2); }
    | ISVOID expr { $$ = make_unop(OP_ISVOID, $2); }
    | NOT expr { $$ = make_unop(OP_NOT, $2); }
    | expr '.' OBJECTID '(' expr_list_e ')' %prec '.' { $$ = make_dispatch($1, $3, $5); }
	| expr '@' TYPEID '.' OBJECTID '(' expr_list_e ')' %prec '.' { $$ = make_static_dispatch($1, $3, $5, $7); }
	| OBJECTID '(' expr_list_e ')' { $$ = make_dispatch(make_object($1), $1, $3); }
    | IF expr THEN expr ELSE expr FI { $$ = make_if($2, $4, $6); }
    | LET let_list IN expr %prec LETPREC { $$ = make_let($2, $4); }
    | '{' block_expr_list '}' { $$ = make_block($2); }
    | CASE expr OF case_list ESAC { $$ = make_case($2, $4); }
    | WHILE expr LOOP expr POOL { $$ = make_while($2, $4); }
    | NEW TYPEID { $$ = make_new($2); }
    ;

let_list: let_list ',' OBJECTID ':' TYPEID { $$ = append_let_list($1, make_let_list(make_let_binding($3, $5, NULL))); }
		| let_list ',' OBJECTID ':' TYPEID ASSIGN expr { $$ = append_let_list($1, make_let_list(make_let_binding($3, $5, $7))); }
		| OBJECTID ':' TYPEID { $$ = make_let_list(make_let_binding($1, $3, NULL)); }
		| OBJECTID ':' TYPEID ASSIGN expr { $$ = make_let_list(make_let_binding($1, $3, $5)); }
		;

expr_list_e: /* empty */ { $$ = NULL; }
		   | expr_list { $$ = $1; }
		   ;

expr_list: expr_list ',' expr { $$ = append_expr_list($1, $3); }
		 | expr { $$ = make_expr_list($1); }
		 ;

block_expr_list: block_expr_list expr ';' { $$ = append_expr_list($1, $2); }
			   | expr ';' { $$ = make_expr_list($1); }
			   ;

case_list: case_list case_item { $$ = append_case_list($1, $2); }
		 | case_item { $$ = make_case_list($1); }
		 ;

case_item: OBJECTID ':' TYPEID DARROW expr ';' { $$ = make_case_item($1, $3, $5); }
		 ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Ошибка: %s\n", s);
}
