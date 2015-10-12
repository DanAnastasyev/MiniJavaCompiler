%locations
%{
	#include <iostream>
	#include "parser.tab.hpp"
	extern "C" int yylex();
	void yyerror(const char *);
	int yydebug = 1;

	void debugRule( YYLTYPE yylloc, const char* rule ) 
	{
		std::cout << "(" << yylloc.first_line << ", " << yylloc.first_column << "; "
		<< yylloc.last_line << ", " << yylloc.last_column << ")  " << rule << std::endl;
	}
%}

/*%union {
	char* val;
}*/

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
		debugRule(@$, "Program");
		$$ = NULL;
	}

MainClassDeclaration:
	CLASS IDENTIFIER '{' PUBLIC STATIC VOID MAIN '(' STRING '[' ']' IDENTIFIER ')' '{' Statement '}' '}' {
		debugRule(@$, "MainClassDeclaration");
		$$ = NULL;
	}

ClassDeclarationList:
	/* empty */ {
		debugRule(@$, "ClassDeclarationList empty");
		$$ = NULL;
	}
	| ClassDeclaration ClassDeclarationList {
		debugRule(@$, "ClassDeclarationList");
		$$ = NULL;
	}

ClassDeclaration:
	CLASS IDENTIFIER '{'  VariableDeclarationList MethodDeclarationList '}' {
		debugRule(@$, "ClassDeclaration");
		$$ = NULL;
	}
	| CLASS IDENTIFIER EXTENDS IDENTIFIER '{'  VariableDeclarationList MethodDeclarationList '}' {
		debugRule(@$, "ClassDeclaration extends");
		$$ = NULL;
	}

VariableDeclarationList:
	/* empty */ {
		debugRule(@$, "VariableDeclarationList empty");
		$$ = NULL;
	}
	| VariableDeclarationList VariableDeclaration {
		debugRule(@$, "VariableDeclarationList");
		$$ = NULL;
	}

VariableDeclaration:
	Type IDENTIFIER ';' {
		debugRule(@$, "VariableDeclaration");
		$$ = NULL;
	}

MethodDeclarationList:
	/* empty */ {
		debugRule(@$, "MethodDeclarationList empty");
		$$ = NULL;
	}
	| MethodDeclaration MethodDeclarationList {
		debugRule(@$, "MethodDeclarationList");
		$$ = NULL;
	}

MethodDeclaration:
	PUBLIC Type IDENTIFIER '(' FormalList ')' '{' VariableDeclarationList StatementList RETURN Expression ';' '}' {
		debugRule(@$, "MethodDeclaration");
		$$ = NULL;
	}

FormalList:
	/* empty */ {
		debugRule(@$, "FormalList empty");
		$$ = NULL;
	}
	| Type IDENTIFIER FormalRestList {
		debugRule(@$, "FormalList");
		$$ = NULL;
	}


FormalRestList:
	/* empty */ {
		debugRule(@$, "FormalRestList empty");
		$$ = NULL;
	}
	| FormalRest FormalRestList {
		debugRule(@$, "FormalRestList");
		$$ = NULL;
	}

FormalRest:
	',' Type IDENTIFIER {
		debugRule(@$, "FormalRest");
		$$ = NULL;
	}

Type:
	INT '[' ']' {
		debugRule(@$, "Type INT[]");
		$$ = NULL;
	}
	| BOOL {
		debugRule(@$, "Type BOOL");
		$$ = NULL;
	}
	| INT {
		debugRule(@$, "Type INT");
		$$ = NULL;
	}
	| IDENTIFIER {
		debugRule(@$, "Type CLASS");
		$$ = NULL;
	}

StatementList:
	/* empty */ {
		debugRule(@$, "StatementList empty");
		$$ = NULL;
	}
	| Statement StatementList {
		debugRule(@$, "StatementList");
		$$ = NULL;
	}

Statement:
	'{' StatementList '}' {
		debugRule(@$, "Statement {}");
		$$ = NULL;
	}
	| IF '(' Expression ')' Statement ELSE Statement {
		debugRule(@$, "Statement IF");
		$$ = NULL;
	}
	| WHILE '(' Expression ')' Statement {
		debugRule(@$, "Statement WHILE");
		$$ = NULL;
	}
	| PRINTLN '(' Expression ')' ';' {
		debugRule(@$, "Statement PRINTLN");
		$$ = NULL;
	}
	| IDENTIFIER '=' Expression ';' {
		debugRule(@$, "Statement ASSIGN");
		$$ = NULL;
	}
	| IDENTIFIER '[' Expression ']' '=' Expression ';' {
		debugRule(@$, "Statement ASSIGN[]");
		$$ = NULL;
	}

Expression:
	Expression AND Expression {
		debugRule(@$, "Expression AND");
		$$ = NULL;
	}
	| Expression '<' Expression {
		debugRule(@$, "Expression <");
		$$ = NULL;
	}
	| Expression '+' Expression {
		debugRule(@$, "Expression +");
		$$ = NULL;
	} 
	| Expression '-' Expression {
		debugRule(@$, "Expression -");
		$$ = NULL;
	}
	| '-' Expression %prec UMINUS {
		debugRule(@$, "Expression UNARY -");
		$$ = NULL;
	}
	| Expression '*' Expression {
		debugRule(@$, "Expression *");
		$$ = NULL;
	}
	| Expression '/' Expression {
		debugRule(@$, "Expression /");
		$$ = NULL;
	}
	| Expression '[' Expression ']' {
		debugRule(@$, "Expression[Expression]");
		$$ = NULL;
	}
	| Expression '.' LENGTH {
		debugRule(@$, "Expression.length");
		$$ = NULL;
	}
	| Expression '.' IDENTIFIER '(' ExpressionList ')' {
		debugRule(@$, "Expression.IDENTIFIER(ExpressionList)");
		$$ = NULL;
	}
	| INTEGER_VAL {
		//std::cout << $1 << std::endl;
		debugRule(@$, "Expression INTEGER_VALUE");
		$$ = NULL;
	}
	| BOOLEAN_VAL {
		//std::cout << $1 << std::endl;
		debugRule(@$, "Expression BOOLEAN_VALUE");
		$$ = NULL;
	}
	| IDENTIFIER {
		//std::cout << $1 << std::endl;
		debugRule(@$, "Expression IDENTIFIER");
		$$ = NULL;
	}
	| THIS {
		debugRule(@$, "Expression THIS");
		$$ = NULL;
	}
	| NEW INT '[' Expression ']' {
		debugRule(@$, "Expression new int[]");
		$$ = NULL;
	}
	| NEW IDENTIFIER '(' ')' {
		debugRule(@$, "Expression new class()");
		$$ = NULL;	
	}
	| '!' Expression  {
		debugRule(@$, "Expression !");
		$$ = NULL;
	}
	| '(' Expression ')' {
		debugRule(@$, "(Expression)");
		$$ = NULL;
	}

ExpressionList:
	/* empty */ {
		debugRule(@$, "ExpressionList empty");
		$$ = NULL;
	}
	| Expression ExpressionRestList {
		debugRule(@$, "ExpressionList");
		$$ = NULL;
	}

ExpressionRestList:
	/* empty */ {
		debugRule(@$, "ExpressionRestList empty");
		$$ = NULL;
	}
	| ExpressionRest ExpressionRestList {
		debugRule(@$, "ExpressionRestList");
		$$ = NULL;
	}

ExpressionRest:
	',' Expression {
		debugRule(@$, "ExpressionRest");
		$$ = NULL;
	}

%%

void yyerror(const char *s) 
{
	std::cout << "(" << yylloc.first_line << ", " << yylloc.first_column << "; "
		<< yylloc.last_line << ", " << yylloc.last_column << ")  ";
	std::cout << s;
}