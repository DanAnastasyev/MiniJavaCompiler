%{
	#include <stdio.h>
	#include <math.h>
	#include <string.h>
	#include <stdarg.h>
	#include <iostream>
	#include <stdio.h>
	#include <set>
	#include <stdlib.h>
	#include <sstream>
	int yylex(void);
	void yyerror(const char *);
	int yydebug = 1;
	extern int yylineno;
%}

%locations
%token INTEGER BOOLEAN VOID STRING DOUBLE
%token VARIABLE
%token WHILE THEN IF RETURN
%token PRIVATE PUBLIC CLASS STATIC MAIN BEGIN_BRACKET END_BRACKET
%nonassoc IF
%nonassoc ELSE

%left OR
%left AND
%left GE LE GT LT EQ NE
%left NOT
%left '+' '-'
%left '*' '/'


%%

class : 
	PUBLIC CLASS VARIABLE BEGIN_BRACKET END_BRACKET {
	} 

    
%%

void yyerror(const char *s) 
{
   fprintf(stderr, "%s line: %d\n", s, yylineno);
}

// инициализация перед парсингом всего
void init() {
    mainProgram = NULL;
}

int main(int argc, char** argv)
{    
    yyparse();
    return 0;
}