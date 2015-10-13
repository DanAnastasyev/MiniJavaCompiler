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

%union {
	char* val;
}

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

%type <val> Program MainClassDeclaration ClassDeclarationList ClassDeclaration VariableDeclarationList VariableDeclaration ExpressionRest StatementList
	MethodDeclaration MethodDeclarationList FormalList FormalRestList FormalRest Type Statement Expression ExpressionList ExpressionRestList
	IDENTIFIER INTEGER_VAL BOOLEAN_VAL

%%

Program:
	MainClassDeclaration ClassDeclarationList {
		debugRule(@$, "Program -> MainClassDeclaration ClassDeclarationList");
		$$ = NULL;
	}

MainClassDeclaration:
	CLASS IDENTIFIER '{' PUBLIC STATIC VOID MAIN '(' STRING '[' ']' IDENTIFIER ')' '{' Statement '}' '}' {
		debugRule(@$, "MainClassDeclaration -> CLASS IDENTIFIER { PUBLIC STATIC VOID MAIN ( STRING [ ] IDENTIFIER ) { Statement } }");
		$$ = NULL;
	}

ClassDeclarationList:
	/* empty */ {
		debugRule(@$, "ClassDeclarationList -> empty");
		$$ = NULL;
	}
	| ClassDeclaration ClassDeclarationList {
		debugRule(@$, "ClassDeclarationList -> ClassDeclaration ClassDeclarationList");
		$$ = NULL;
	}

ClassDeclaration:
	CLASS IDENTIFIER '{'  VariableDeclarationList MethodDeclarationList '}' {
		debugRule(@$, "ClassDeclaration -> CLASS IDENTIFIER {  VariableDeclarationList MethodDeclarationList }");
		$$ = NULL;
	}
	| CLASS IDENTIFIER EXTENDS IDENTIFIER '{'  VariableDeclarationList MethodDeclarationList '}' {
		debugRule(@$, "ClassDeclaration -> CLASS IDENTIFIER EXTENDS IDENTIFIER {  VariableDeclarationList MethodDeclarationList }");
		$$ = NULL;
	}

VariableDeclarationList:
	/* empty */ {
		debugRule(@$, "VariableDeclarationList -> empty");
		$$ = NULL;
	}
	| VariableDeclarationList VariableDeclaration {
		debugRule(@$, "VariableDeclarationList -> VariableDeclarationList VariableDeclaration");
		$$ = NULL;
	}

VariableDeclaration:
	Type IDENTIFIER ';' {
		debugRule(@$, "VariableDeclaration -> Type IDENTIFIER ;");
		$$ = NULL;
	}

MethodDeclarationList:
	/* empty */ {
		debugRule(@$, "MethodDeclarationList -> empty");
		$$ = NULL;
	}
	| MethodDeclaration MethodDeclarationList {
		debugRule(@$, "MethodDeclarationList -> MethodDeclaration MethodDeclarationList");
		$$ = NULL;
	}

MethodDeclaration:
	PUBLIC Type IDENTIFIER '(' FormalList ')' '{' VariableDeclarationList StatementList RETURN Expression ';' '}' {
		debugRule(@$, "MethodDeclaration -> PUBLIC Type IDENTIFIER ( FormalList ) { VariableDeclarationList StatementList RETURN Expression ; }");
		$$ = NULL;
	}

FormalList:
	/* empty */ {
		debugRule(@$, "FormalList -> empty");
		$$ = NULL;
	}
	| Type IDENTIFIER FormalRestList {
		debugRule(@$, "FormalList -> Type IDENTIFIER FormalRestList");
		$$ = NULL;
	}


FormalRestList:
	/* empty */ {
		debugRule(@$, "FormalRestList -> empty");
		$$ = NULL;
	}
	| FormalRest FormalRestList {
		debugRule(@$, "FormalRestList -> FormalRest FormalRestList");
		$$ = NULL;
	}

FormalRest:
	',' Type IDENTIFIER {
		debugRule(@$, "FormalRest -> , Type IDENTIFIER");
		$$ = NULL;
	}

Type:
	INT '[' ']' {
		debugRule(@$, "Type -> INT[]");
		$$ = NULL;
	}
	| BOOL {
		debugRule(@$, "Type -> BOOL");
		$$ = NULL;
	}
	| INT {
		debugRule(@$, "Type -> INT");
		$$ = NULL;
	}
	| IDENTIFIER {
		debugRule(@$, "Type -> CLASS");
		$$ = NULL;
	}

StatementList:
	/* empty */ {
		debugRule(@$, "StatementList -> empty");
		$$ = NULL;
	}
	| Statement StatementList {
		debugRule(@$, "StatementList -> Statement StatementList");
		$$ = NULL;
	}

Statement:
	'{' StatementList '}' {
		debugRule(@$, "Statement -> { StatementList }");
		$$ = NULL;
	}
	| IF '(' Expression ')' Statement ELSE Statement {
		debugRule(@$, "Statement -> IF ( Expression ) Statement ELSE Statement");
		$$ = NULL;
	}
	| WHILE '(' Expression ')' Statement {
		debugRule(@$, "Statement -> WHILE ( Expression ) Statement");
		$$ = NULL;
	}
	| PRINTLN '(' Expression ')' ';' {
		debugRule(@$, "Statement -> PRINTLN ( Expression ) ;");
		$$ = NULL;
	}
	| IDENTIFIER '=' Expression ';' {
		debugRule(@$, "Statement -> IDENTIFIER = Expression ;");
		$$ = NULL;
	}
	| IDENTIFIER '[' Expression ']' '=' Expression ';' {
		debugRule(@$, "Statement -> IDENTIFIER [ Expression ] = Expression ;");
		$$ = NULL;
	}

Expression:
	Expression AND Expression {
		debugRule(@$, "Expression -> Expression AND Expression");
		$$ = NULL;
	}
	| Expression '<' Expression {
		debugRule(@$, "Expression -> Expression < Expression");
		$$ = NULL;
	}
	| Expression '+' Expression {
		debugRule(@$, "Expression -> Expression + Expression");
		$$ = NULL;
	} 
	| Expression '-' Expression {
		debugRule(@$, "Expression -> Expression - Expression");
		$$ = NULL;
	}
	| '-' Expression %prec UMINUS {
		debugRule(@$, "Expression -> -Expression");
		$$ = NULL;
	}
	| Expression '*' Expression {
		debugRule(@$, "Expression -> Expression * Expression");
		$$ = NULL;
	}
	| Expression '/' Expression {
		debugRule(@$, "Expression -> Expression / Expression");
		$$ = NULL;
	}
	| Expression '[' Expression ']' {
		debugRule(@$, "Expression -> Expression[Expression]");
		$$ = NULL;
	}
	| Expression '.' LENGTH {
		debugRule(@$, "Expression -> Expression.LENGTH");
		$$ = NULL;
	}
	| Expression '.' IDENTIFIER '(' ExpressionList ')' {
		debugRule(@$, "Expression -> Expression.IDENTIFIER(ExpressionList)");
		$$ = NULL;
	}
	| INTEGER_VAL {
		debugRule(@$, (std::string("Expression -> INTEGER_VALUE(") + std::string($1) + std::string(")")).c_str() );
		$$ = NULL;
	}
	| BOOLEAN_VAL {
		debugRule(@$, (std::string("Expression -> BOOLEAN_VALUE(") + std::string($1) + std::string(")")).c_str() );
		$$ = NULL;
	}
	| IDENTIFIER {
		debugRule(@$, (std::string("Expression -> IDENTIFIER(") + std::string($1) + std::string(")")).c_str() );
		$$ = NULL;
	}
	| THIS {
		debugRule(@$, "Expression -> THIS");
		$$ = NULL;
	}
	| NEW INT '[' Expression ']' {
		debugRule(@$, "Expression -> new int[]");
		$$ = NULL;
	}
	| NEW IDENTIFIER '(' ')' {
		debugRule(@$, "Expression -> new IDENTIFIER()");
		$$ = NULL;	
	}
	| '!' Expression  {
		debugRule(@$, "Expression -> !Expression");
		$$ = NULL;
	}
	| '(' Expression ')' {
		debugRule(@$, "Expression -> (Expression)");
		$$ = NULL;
	}

ExpressionList:
	/* empty */ {
		debugRule(@$, "ExpressionList -> empty");
		$$ = NULL;
	}
	| Expression ExpressionRestList {
		debugRule(@$, "ExpressionList -> Expression ExpressionRestList");
		$$ = NULL;
	}

ExpressionRestList:
	/* empty */ {
		debugRule(@$, "ExpressionRestList -> empty");
		$$ = NULL;
	}
	| ExpressionRest ExpressionRestList {
		debugRule(@$, "ExpressionRestList -> Expression ExpressionRestList");
		$$ = NULL;
	}

ExpressionRest:
	',' Expression {
		debugRule(@$, "ExpressionRest -> , Expression");
		$$ = NULL;
	}

%%

void yyerror(const char *s) 
{
	std::cout << "(" << yylloc.first_line << ", " << yylloc.first_column << "; "
		<< yylloc.last_line << ", " << yylloc.last_column << ")  ";
	std::cout << s;
}