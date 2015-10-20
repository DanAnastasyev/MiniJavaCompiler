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

%type <val> Program MainClassDeclaration ClassDeclarationList ClassDeclaration VariableDeclarationList VariableDeclaration ExpressionRest 
	MethodDeclaration MethodDeclarationList FormalList FormalRestList FormalRest Type Statement Expression ExpressionList ExpressionRestList
	IDENTIFIER INTEGER_VAL BOOLEAN_VAL StatementList

%%

Program:
	MainClassDeclaration ClassDeclarationList {
		debugRule(@$, "Program -> MainClassDeclaration ClassDeclarationList");
		$$ = new CProgram( $1, $2, CPosition( yylloc ) );
	}

MainClassDeclaration:
	CLASS IDENTIFIER '{' PUBLIC STATIC VOID MAIN '(' STRING '[' ']' IDENTIFIER ')' '{' Statement '}' '}' {
		debugRule(@$, "MainClassDeclaration -> CLASS IDENTIFIER { PUBLIC STATIC VOID MAIN ( STRING [ ] IDENTIFIER ) { Statement } }");
		$$ = new CMainClass( $2, $12, $15, CPosition( yylloc ) );
	}

ClassDeclarationList:
	/* empty */ {
		debugRule(@$, "ClassDeclarationList -> empty");
		$$ = nullptr;
	}
	| ClassDeclaration ClassDeclarationList {
		debugRule(@$, "ClassDeclarationList -> ClassDeclaration ClassDeclarationList");
		$$ = new CClassDeclList( $1, $2, CPosition( yylloc ) );
	}

ClassDeclaration:
	CLASS IDENTIFIER '{'  VariableDeclarationList MethodDeclarationList '}' {
		debugRule(@$, "ClassDeclaration -> CLASS IDENTIFIER {  VariableDeclarationList MethodDeclarationList }");
		$$ = new CClassDecl( $4, $5, $2, CPosition( yylloc ) );
	}
	| CLASS IDENTIFIER EXTENDS IDENTIFIER '{'  VariableDeclarationList MethodDeclarationList '}' {
		debugRule(@$, "ClassDeclaration -> CLASS IDENTIFIER EXTENDS IDENTIFIER {  VariableDeclarationList MethodDeclarationList }");
		$$ = new CClassDeclDerived( $6, $7, $2, $4, CPosition( yylloc ) );
	}

VariableDeclarationList:
	/* empty */ {
		debugRule(@$, "VariableDeclarationList -> empty");
		$$ = NULL; // NOT IMPLEMENTED
	}
	| VariableDeclarationList VariableDeclaration {
		debugRule(@$, "VariableDeclarationList -> VariableDeclarationList VariableDeclaration");
		$$ = NULL; // NOT IMPLEMENTED
	}

VariableDeclaration:
	Type IDENTIFIER ';' {
		debugRule(@$, "VariableDeclaration -> Type IDENTIFIER ;");
		$$ = new CVarDecl( $1, $2, CPosition( yylloc ) );
	}

MethodDeclarationList:
	/* empty */ {
		debugRule(@$, "MethodDeclarationList -> empty");
		$$ = nullptr;
	}
	| MethodDeclaration MethodDeclarationList {
		debugRule(@$, "MethodDeclarationList -> MethodDeclaration MethodDeclarationList");
		$$ = NULL; // NOT IMPLEMENTED
	}

MethodDeclaration:
	PUBLIC Type IDENTIFIER '(' FormalList ')' '{' VariableDeclarationList StatementList RETURN Expression ';' '}' {
		debugRule(@$, "MethodDeclaration -> PUBLIC Type IDENTIFIER ( FormalList ) { VariableDeclarationList StatementList RETURN Expression ; }");
		$$ = new CMethodDecl( $2, $3, $5, $8, $9, $11, CPosition( yylloc ) );
	}

FormalList:
	/* empty */ {
		debugRule(@$, "FormalList -> empty");
		$$ = nullptr;
	}
	| Type IDENTIFIER FormalRestList {
		debugRule(@$, "FormalList -> Type IDENTIFIER FormalRestList");
		$$ = NULL; // NOT IMPLEMENTED
	}


FormalRestList:
	/* empty */ {
		debugRule(@$, "FormalRestList -> empty");
		$$ = nullptr;
	}
	| FormalRest FormalRestList {
		debugRule(@$, "FormalRestList -> FormalRest FormalRestList");
		$$ = NULL; // NOT IMPLEMENTED
	}

FormalRest:
	',' Type IDENTIFIER {
		debugRule(@$, "FormalRest -> , Type IDENTIFIER");
		$$ = NULL; // NOT IMPLEMENTED
	}

Type:
	INT '[' ']' {
		debugRule(@$, "Type -> INT[]");
		$$ = new CStandardType( CStandardType::StandardType::INT_ARRAY, CPosition( yylloc ) );
	}
	| BOOL {
		debugRule(@$, "Type -> BOOL");
		$$ = new CStandardType( CStandardType::StandardType::BOOL, CPosition( yylloc ) );
	}
	| INT {
		debugRule(@$, "Type -> INT");
		$$ = new CStandardType( CStandardType::StandardType::INT, CPosition( yylloc ) );
	}
	| IDENTIFIER {
		debugRule(@$, "Type -> CLASS");
		$$ = new CUserType( $1, CPosition( yylloc ) );
	}

StatementList:
	/* empty */ {
		debugRule(@$, "StatementList -> empty");
		$$ = nullptr;
	}
	| Statement StatementList {
		debugRule(@$, "StatementList -> Statement StatementList");
		$$ = new CStatementList( $1, $2, CPosition( yylloc ) );
	}

Statement:
	'{' StatementList '}' {
		debugRule(@$, "Statement -> { StatementList }");
		$$ = new CStatementListStatement( $1 );
	}
	| IF '(' Expression ')' Statement ELSE Statement {
		debugRule(@$, "Statement -> IF ( Expression ) Statement ELSE Statement");
		$$ = new CIfStatement( $3, $5, $7, CPosition( yylloc ) );
	}
	| WHILE '(' Expression ')' Statement {
		debugRule(@$, "Statement -> WHILE ( Expression ) Statement");
		$$ = new CWhileStatement( $3, $5, CPosition( yylloc ) );
	}
	| PRINTLN '(' Expression ')' ';' {
		debugRule(@$, "Statement -> PRINTLN ( Expression ) ;");
		$$ = new CPrintStatement( $3, CPosition( yylloc ) );
	}
	| IDENTIFIER '=' Expression ';' {
		debugRule(@$, "Statement -> IDENTIFIER = Expression ;");
		$$ = new CAssignStatement( $1, $3, CPosition( yylloc ) );
	}
	| IDENTIFIER '[' Expression ']' '=' Expression ';' {
		debugRule(@$, "Statement -> IDENTIFIER [ Expression ] = Expression ;");
		$$ = new CArrayAssignStatement( $1, $3, $6, CPosition( yylloc ) );
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