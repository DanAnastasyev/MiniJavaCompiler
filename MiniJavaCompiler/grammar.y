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
%token INTEGER BOOLEAN VOID STRING DOUBLE INT BOOL
%token IDENTIFIER VOID PRINTLN BINARY_AND BINARY_OR
%token WHILE THEN IF RETURN EXTENDS LENGTH
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

Program:
	MainClassDeclaration ClassDeclarationList {
		$$ = NULL;
	}

MainClassDeclaration:
	CLASS IDENTIFIER BEGIN_BRACKET PUBLIC STATIC VOID MAIN '(' STRING_KEYWORD '[' ']' IDENTIFIER ')' BEGIN_BRACKET Statement END_BRACKET END_BRACKET {
		$$ = NULL;
	}

ClassDeclarationList:
	/* empty */ {
		$$ = NULL;
	}
	| ClassDeclaration ClassDeclarationList {
		$$ = NULL;
	}

ClassDeclaration:
	CLASS IDENTIFIER BEGIN_BRACKET  VariableDeclarationList MethodDeclarationList END_BRACKET {
		$$ = NULL;
	}
	| CLASS IDENTIFIER EXTENDS IDENTIFIER BEGIN_BRACKET  VariableDeclarationList MethodDeclarationList END_BRACKET {
		$$ = NULL;
	}

VariableDeclarationList:
	/* empty */ {
		$$ = NULL;
	}
	| VariableDeclarationList VariableDeclaration {
		$$ = NULL;
	}

VariableDeclaration:
	Type IDENTIFIER ';' {
		$$ = NULL;
	}

MethodDeclarationList:
	/* empty */ {
		$$ = NULL;
	}
	| MethodDeclaration MethodDeclarationList {
		$$ = NULL;
	}

MethodDeclaration:
	PUBLIC Type IDENTIFIER '(' FormalList ')' BEGIN_BRACKET VariableDeclarationList StatementList RETURN Expression ';' END_BRACKET {
		$$ = NULL;
	}

FormalList:
	/* empty */ {
		$$ = NULL;
	}
	| Type IDENTIFIER FormalRestList {
		$$ = NULL;
	}


FormalRestList:
	/* empty */ {
		$$ = NULL;
	}
	| FormalRest FormalRestList {
		$$ = NULL;
	}

FormalRest:
	',' Type IDENTIFIER {
		$$ = NULL;
	}

Type:
	INT '[' ']' {
		$$ = NULL;
	}
	| BOOL {
		$$ = NULL;
	}
	| INT {
		$$ = NULL;
	}
	| IDENTIFIER {
		$$ = NULL;
	}

StatementList:
	/* empty */ {
		$$ = NULL;
	}
	| Statement StatementList {
		$$ = NULL;
	}

Statement:
	BEGIN_BRACKET StatementList END_BRACKET {
		$$ = NULL;
	}
	| IF '(' Expression ')' Statement ELSE Statement {
		$$ = NULL;
	}
	| WHILE '(' Expression ')' Statement {
		$$ = NULL;
	}
	| PRINTLN '(' Expression ')' ';' {
		$$ = NULL;
	}
	| IDENTIFIER '=' Expression ';' {
		$$ = NULL;
	}
	| IDENTIFIER '[' Expression ']' '=' Expression ';' {
		$$ = NULL;
	}

Expression:
	Expression BINARY_AND Expression {
		$$ = NULL;
	}
	| Expression '<' Expression {
		$$ = NULL;
	}
	| Expression '+' Expression {
		$$ = NULL;
	} 
	| Expression '-' Expression {
		$$ = NULL;
	}
	| Expression '*' Expression {
		$$ = NULL;
	}
	| Expression '.' LENGTH {
		$$ = NULL;
	}
	| Expression '.' IDENTIFIER '(' ExpressionList ')' {
		$$ = NULL;
	}
	| INTEGER {
		$$ = NULL;
	}
	| BOOLEAN {
		$$ = NULL;
	}
	| IDENTIFIER {
		$$ = NULL;
	}
	| THIS {
		$$ = NULL;
	}
	| NEW INT '[' Expression ']' {
		$$ = NULL;
	}
	| NEW IDENTIFIER '(' ')' {
		$$ = NULL;	
	}
	| '!' Expression  {
		$$ = NULL;
	}
	| '(' Expression ')' {
		$$ = NULL;
	}

ExpressionList:
	/* empty */ {
		$$ = NULL;
	}
	| Expression ExpressionRestList {
		$$ = NULL;
	}

ExpressionRestList:
	/* empty */ {
		$$ = NULL;
	}
	| ExpressionRest ExpressionRestList {
		$$ = NULL;
	}

ExpressionRest:
	',' Expression {
		$$ = NULL;
	}
;

    
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