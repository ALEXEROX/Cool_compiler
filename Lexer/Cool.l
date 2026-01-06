%option never-interactive
%option noyywrap 

%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Parser/cool_code.tab.h"
%}

%x COMMENT STRING

%%

%{
int line = 1;
char string_buf[8192];
int string_pos = 0;
int comment_depth = 0;
char comment_buf[8192];
int comment_pos = 0;
%}

[ \t\r]+            ;
\n+                 { line+=yyleng; }

"class"             { return CLASS; }
"else"              { return ELSE; }
"fi"                { return FI; }
"if"                { return IF; }
"in"                { return IN; }
"inherits"          { return INHERITS; }
"let"               { return LET; }
"loop"              { return LOOP; }
"pool"              { return POOL; }
"then"              { return THEN; }
"while"             { return WHILE; }
"case"              { return CASE; }
"of"                { return OF; }
"esac"              { return ESAC; }
"new"               { return NEW; }
"isvoid"            { return ISVOID; }
"not"               { return NOT; }

"true"              { return TRUE; }
"false"             { return FALSE; }

[0-9]+              { yylval.intval=atoi(yytext); return INT_CONST; }

[A-Z][A-Za-z0-9_]*  { yylval.str=(char *)malloc(strlen(yytext)+1); strcpy(yylval.str, yytext); return  TYPEID; }
[a-z][A-Za-z0-9_]*  { yylval.str=(char *)malloc(strlen(yytext)+1); strcpy(yylval.str, yytext); return OBJECTID; }

"<-"                { return ASSIGN; }
"=>"                { return DARROW; }
"<="                { return LE; }
"="                 { return '='; }
"<"                 { return '<'; }
"+"                 { return '+'; }
"-"                 { return '-'; }
"*"                 { return '*'; }
"/"                 { return '/'; }
"("                 { return '('; }
")"                 { return ')'; }
"{"                 { return '{'; }
"}"                 { return '}'; }
";"                 { return ';'; }
":"                 { return ':'; }
","                 { return ','; }
"."                 { return '.'; }
"@"                 { return '@'; }
"~"                 { return '~'; }
"&&"				{ return AND; }
"||"				{ return OR; }

"--".*              { printf("Comment: %s\n", yytext); }

\"                  { BEGIN(STRING); string_buf[0] = '\0'; string_pos = 0; }
<STRING>[^\"\\\n]+  { int len = yyleng; if (string_pos + len < sizeof(string_buf)-1) { strncpy(string_buf + string_pos, yytext, len); string_pos += len; string_buf[string_pos] = '\0'; } }
<STRING>\\n         { if (string_pos + 1 < sizeof(string_buf)-1) { string_buf[string_pos++] = '\n'; string_buf[string_pos] = '\0'; } }
<STRING>\\t         { if (string_pos + 1 < sizeof(string_buf)-1) { string_buf[string_pos++] = '\t'; string_buf[string_pos] = '\0'; } }
<STRING>\\b         { if (string_pos + 1 < sizeof(string_buf)-1) { string_buf[string_pos++] = '\b'; string_buf[string_pos] = '\0'; } }
<STRING>\\f         { if (string_pos + 1 < sizeof(string_buf)-1) { string_buf[string_pos++] = '\f'; string_buf[string_pos] = '\0'; } }
<STRING>\\r         { if (string_pos + 1 < sizeof(string_buf)-1) { string_buf[string_pos++] = '\r'; string_buf[string_pos] = '\0'; } }
<STRING>\\\\        { if (string_pos + 1 < sizeof(string_buf)-1) { string_buf[string_pos++] = '\\'; string_buf[string_pos] = '\0'; } }
<STRING>\\\"        { if (string_pos + 1 < sizeof(string_buf)-1) { string_buf[string_pos++] = '\"'; string_buf[string_pos] = '\0'; } }
<STRING>\n          { printf("ERROR unterminated_string line %d\n", line); BEGIN(INITIAL); }
<STRING>\\\n        { line++; }
<STRING>\"          { if (1024 < strlen(string_buf)) {printf("ERROR string_too_long line %d\n", line);} else { BEGIN(INITIAL); yylval.str=(char *)malloc(strlen(string_buf)+1); strcpy(yylval.str, string_buf); return STR_CONST;} }
<STRING><<EOF>>     { 
                        printf("ERROR unterminated_string: %s line %d\n", string_buf, line);
                        BEGIN(INITIAL);
                    }


"(*"                { comment_depth = 1; BEGIN(COMMENT); comment_pos = 0; strncpy(comment_buf, yytext, yyleng); comment_pos += yyleng; }
<COMMENT>"(*"       { comment_depth++; strncpy(comment_buf+comment_pos, yytext, yyleng); comment_pos += yyleng; }
<COMMENT>"*)"       {
                        comment_depth--;
                        strncpy(comment_buf+comment_pos, yytext, yyleng);
                        comment_pos += yyleng;
                        comment_buf[comment_pos] = '\0';
                        if (comment_depth == 0) {
                            BEGIN(INITIAL);
                            printf("MultilineComment: %s\n", comment_buf);
                        }
                    }
<COMMENT>\n         { line++; comment_buf[comment_pos++] = '\n'; }
<COMMENT>.+         { int len = yyleng; strncpy(comment_buf + comment_pos, yytext, len); comment_pos += len;}
<COMMENT><<EOF>>    { 
                        comment_buf[comment_pos] = '\0';
                        printf("ERROR unterminated_comment: %s line %d\n", comment_buf, line);
                        BEGIN(INITIAL);
                    }

<*>.                { printf("ERROR unexpected_symbol %s line %d\n", yytext, line); }

%%
