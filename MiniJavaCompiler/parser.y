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

%type <IProgram*> Program
%type <IMainClass*> MainClassDeclaration
%type <IClassDeclList*> ClassDeclarationList
%type <IClassDecl*> ClassDeclaration
%type <IVarDeclList*> VariableDeclarationList
%type <IVarDecl*> VariableDeclaration
%type <IMethodDecl*> MethodDeclaration
%type <IMethodDeclList*> MethodDeclarationList
%type <IFormalList*> FormalList FormalRestList FormalRest
%type <IStatement*> Statement
%type <IType*> Type
%type <IExp*> Expression IDENTIFIER INTEGER_VAL BOOLEAN_VAL
%type <IExpList*> ExpressionList ExpressionRestList ExpressionRest
%type <IStatementList*> StatementList

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
		$$ = nullptr;
	}
	| VariableDeclarationList VariableDeclaration {
		debugRule(@$, "VariableDeclarationList -> VariableDeclarationList VariableDeclaration");
		$$ = new CVarDeclList( $1, $2, CPosition( yylloc ) );
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
		$$ = new CMethodDeclList( $1, $2, CPosition( yylloc ) );
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
		$$ = new CStatementListStatement( $2, CPosition( yylloc ) );
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
		$$ = new CBinOpExpression( $1, AND, $3, CPosition( yylloc ) );
	}
	| Expression '<' Expression {
		debugRule(@$, "Expression -> Expression < Expression");
		$$ = new CBinOpExpression( $1, LESS, $3, CPosition( yylloc ) );
	}
	| Expression '+' Expression {
		debugRule(@$, "Expression -> Expression + Expression");
		$$ = new CBinOpExpression( $1, PLUS, $3, CPosition( yylloc ) );
	} 
	| Expression '-' Expression {
		debugRule(@$, "Expression -> Expression - Expression");
		$$ = new CBinOpExpression( $1, MINUS, $3, CPosition( yylloc ) );
	}
	| '-' Expression %prec UMINUS {
		debugRule(@$, "Expression -> -Expression");
		$$ = new CUnaryOpExpression( MINUS, $2, CPosition( yylloc ) );
	}
	| Expression '*' Expression {
		debugRule(@$, "Expression -> Expression * Expression");
		$$ = new CBinOpExpression( $1, TIMES, $3, CPosition( yylloc ) );
	}
	| Expression '/' Expression {
		debugRule(@$, "Expression -> Expression / Expression");
		$$ = new CBinOpExpression( $1, DIVIDE, $3, CPosition( yylloc ) );
	}
	| Expression '[' Expression ']' {
		debugRule(@$, "Expression -> Expression[Expression]");
		$$ = new CIndexExpression( $1, $3, CPosition( yylloc ) );
	}
	| Expression '.' LENGTH {
		debugRule(@$, "Expression -> Expression.LENGTH");
		$$ = new CLenghtExpression( $1, CPosition( yylloc ) );
	}
	| Expression '.' IDENTIFIER '(' ExpressionList ')' {
		debugRule(@$, "Expression -> Expression.IDENTIFIER(ExpressionList)");
		$$ = new CMethodExpression( $1, $3, $5, CPosition( yylloc ) );
	}
	| INTEGER_VAL {
		debugRule(@$, (std::string("Expression -> INTEGER_VALUE(") + std::string($1) + std::string(")")).c_str() );
		$$ = new CIntLiteralExpression( $1, CPosition( yylloc ) );
	}
	| BOOLEAN_VAL {
		debugRule(@$, (std::string("Expression -> BOOLEAN_VALUE(") + std::string($1) + std::string(")")).c_str() );
		$$ = new CBoolLiteralExpression( $1, CPosition( yylloc ) );
	}
	| IDENTIFIER {
		debugRule(@$, (std::string("Expression -> IDENTIFIER(") + std::string($1) + std::string(")")).c_str() );
		$$ = new CIdentifierExpression( $1, CPosition( yylloc ) );
	}
	| THIS {
		debugRule(@$, "Expression -> THIS");
		$$ = new CThisExpression( CPosition( yylloc ) );
	}
	| NEW INT '[' Expression ']' {
		debugRule(@$, "Expression -> new int[]");
		$$ = new CIdentifierExpression( $4, CPosition( yylloc ) );
	}
	| NEW IDENTIFIER '(' ')' {
		debugRule(@$, "Expression -> new IDENTIFIER()");
		$$ = new CNewExpression( $2, CPosition( yylloc ) );	
	}
	| '!' Expression  {
		debugRule(@$, "Expression -> !Expression");
		$$ = new CUnaryOpExpression( NOT, $2, CPosition( yylloc ) );
	}
	| '(' Expression ')' {
		debugRule(@$, "Expression -> (Expression)");
		$$ = CBracesExpression( $2, CPosition( yylloc ) );
	}

ExpressionList:
	/* empty */ {
		debugRule(@$, "ExpressionList -> empty");
		$$ = nullptr;
	}
	| Expression ExpressionRestList {
		debugRule(@$, "ExpressionList -> Expression ExpressionRestList");
		$$ = new CExpressionList( $1, $2, CPosition( yylloc ) );
	}

ExpressionRestList:
	/* empty */ {
		debugRule(@$, "ExpressionRestList -> empty");
		$$ = nullptr;
	}
	| ExpressionRest ExpressionRestList {
		debugRule(@$, "ExpressionRestList -> Expression ExpressionRestList");
		$$ = new CExpressionList( $1, $2, CPosition( yylloc ) );
	}

ExpressionRest:
	',' Expression {
		debugRule(@$, "ExpressionRest -> , Expression");
		$$ = NULL; // NOT IMPLEMENTED
	}

%%

void yyerror(const char *s) 
{
	std::cout << "(" << yylloc.first_line << ", " << yylloc.first_column << "; "
		<< yylloc.last_line << ", " << yylloc.last_column << ")  ";
	std::cout << s;
}