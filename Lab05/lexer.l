%{
// Reconhecedor de uma linguagem simplificada
#include <iostream>
#include "tokens.h"
#include "token.h"
using std::cout;

typedef union { 
    Token* token; 
} YYSTYPE; 

YYSTYPE yylval;

%}

%option noyywrap

delim    [ \t\n]
brancos {delim}+
letra    [A-Za-z]
digito   [0-9]
id       ({letra}|_){letra}*({letra}|{digito}|_)*
num      {digito}+(\.{digito}+)?(E[+-]?{digito}+)?

%%

{brancos}    ; // nenhuma ação e nenhum retorno
main        { yylval.token = new Token(MAIN, yytext); return MAIN; }

int         { yylval.token = new Token(INTEGER, yytext); return INTEGER; }
float       { yylval.token = new Token(FLOATING, yytext); return FLOATING; }
bool        { yylval.token = new Token(BOOLEAN, yytext); return BOOLEAN; }
true        { yylval.token = new Token(TRUE, yytext); return TRUE; }
false       { yylval.token = new Token(FALSE, yytext); return FALSE; }

while       { yylval.token = new Token(WHILE, yytext); return WHILE; }
if          { yylval.token = new Token(IF, yytext); return IF; }
else        { yylval.token = new Token(ELSE, yytext); return ELSE; }
do          { yylval.token = new Token(DO, yytext); return DO; }

{id}        { yylval.token = new Token(ID, yytext); return ID; }
{num}       { if (strchr(yytext, '.') || strchr(yytext, 'E') || strchr(yytext, 'e')) { yylval.token = new Token(FLOATING, yytext); return FLOATING; } else { yylval.token = new Token(INTEGER, yytext); return INTEGER; } }


";"         { yylval.token = new Token(';', yytext); return ';'; } 
"="         { yylval.token = new Token('=', yytext); return '='; }
"+"         { yylval.token = new Token('+', yytext); return '+'; } 
"-"         { yylval.token = new Token('-', yytext); return '-'; } 
"*"         { yylval.token = new Token('*', yytext); return '*'; } 
"/"         { yylval.token = new Token('/', yytext); return '/'; }

"!"         { yylval.token = new Token('!', yytext); return '!'; }
"=="        { yylval.token = new Token(EQ, yytext); return EQ; }
"!="        { yylval.token = new Token(NEQ, yytext); return NEQ; }
"<"         { yylval.token = new Token(LT, yytext); return LT; }
"<="        { yylval.token = new Token(LTE, yytext); return LTE; }
">"         { yylval.token = new Token(GT, yytext); return GT; }
">="        { yylval.token = new Token(GTE, yytext); return GTE; }
"&&"        { yylval.token = new Token(AND, yytext); return AND; }
"||"        { yylval.token = new Token(OR, yytext); return OR; }
"("         {yylval.token = new Token('(', yytext); return '(';}
")"         {yylval.token = new Token(')', yytext); return ')';}
"["         {yylval.token = new Token('[', yytext); return '[';}
"]"         {yylval.token = new Token(']', yytext); return ']';}
"{"         {yylval.token = new Token('{', yytext); return '{';}
"}"         {yylval.token = new Token('}', yytext); return '}';}

.           { cout << yytext << " eh um token invalido!\n"; return *yytext; }
%%

int yywrap() {
    return 1;
}
