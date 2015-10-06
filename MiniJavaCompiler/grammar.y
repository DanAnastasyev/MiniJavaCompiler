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
	extern "C" int yylex();
	void yyerror(const char *);
	int yydebug = 1;
	// extern int yylineno;
%}

%locations
%token INTEGER_VAL BOOLEAN_VAL VOID STRING INT BOOL
%token IDENTIFIER PRINTLN THIS NEW
%token WHILE IF RETURN EXTENDS LENGTH
%token PUBLIC CLASS STATIC MAIN
%nonassoc IF
%nonassoc ELSE

%left AND
%left '<' '='
%left '+' '-'
%left '*' '/'
%left UMINUS
%left '!'
%left '.' '[' ']'

%%

Program:
	MainClassDeclaration ClassDeclarationList {
		$$ = NULL;
	}

MainClassDeclaration:
	CLASS IDENTIFIER '{' PUBLIC STATIC VOID MAIN '(' STRING '[' ']' IDENTIFIER ')' '{' Statement '}' '}' {
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
	CLASS IDENTIFIER '{'  VariableDeclarationList MethodDeclarationList '}' {
		$$ = NULL;
	}
	| CLASS IDENTIFIER EXTENDS IDENTIFIER '{'  VariableDeclarationList MethodDeclarationList '}' {
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
	PUBLIC Type IDENTIFIER '(' FormalList ')' '{' VariableDeclarationList StatementList RETURN Expression ';' '}' {
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
	'{' StatementList '}' {
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
	Expression '<' Expression {
		$$ = NULL;
	}
	| Expression '+' Expression {
		$$ = NULL;
	} 
	| Expression '-' Expression {
		$$ = NULL;
	}
	| '-' Expression %prec UMINUS {
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
	| INTEGER_VAL {
		$$ = NULL;
	}
	| BOOLEAN_VAL {
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

%%

void yyerror(const char *s) 
{
//    fprintf(stderr, "%s line: %d\n", s, yylineno);
}