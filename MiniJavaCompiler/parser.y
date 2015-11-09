%locations
%{
	#include <iostream>
	#include "RuleClasses/RuleClasses.h"
	#include "Visitors/PrettyPrinterVisitor.h"

	extern "C" int yylex();
	void yyerror(std::shared_ptr<IProgram>& root, const char *);
	int yydebug = 1;

//	void debugRule( YYLTYPE yylloc, const char* rule ) 
//	{
		//std::cout << "(" << yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column.first_line << ", " << yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column.first_column << "; "
		//<< yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column.last_line << ", " << yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column.last_column << ")  " << rule << std::endl;
//	}
%}

%code requires {
	#include "RuleClasses/RuleClasses.h"
	#include "Visitors/PrettyPrinterVisitor.h"
}

%parse-param { std::shared_ptr<IProgram>& root }

%union {
	char* val;
	int intVal;
	bool boolVal;
	IProgram* programVal;
	IMainClass* mainClassDeclarationVal;
	IClassDeclList* classDeclarationListVal;
	IClassDecl* classDeclarationVal;
	IVarDeclList* variableDeclarationListVal;
	IVarDecl* variableDeclarationVal;
	IMethodDecl* methodDeclarationVal;
	IMethodDeclList* methodDeclarationListVal;
	IFormalList* formalListVal;
	IStatement* statementVal;
	IType* typeVal;
	IExp* expressionVal;
	IExpList* expressionListVal;
	IStatementList* statementListVal;
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

%type <programVal> Program
%type <mainClassDeclarationVal> MainClassDeclaration
%type <classDeclarationListVal> ClassDeclarationList
%type <classDeclarationVal> ClassDeclaration
%type <variableDeclarationListVal> VariableDeclarationList
%type <variableDeclarationVal> VariableDeclaration
%type <methodDeclarationVal> MethodDeclaration
%type <methodDeclarationListVal> MethodDeclarationList
%type <formalListVal> FormalList FormalRestList FormalRest
%type <statementVal> Statement
%type <typeVal> Type
%type <expressionVal> Expression ExpressionRest
%type <expressionListVal> ExpressionList ExpressionRestList
%type <statementListVal> StatementList
%type <val> IDENTIFIER INTEGER_VAL BOOLEAN_VAL

%%

Program:
	MainClassDeclaration ClassDeclarationList {
		//debugRule(@$, "Program -> MainClassDeclaration ClassDeclarationList");
		root = std::make_shared<CProgram>( $1, $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

MainClassDeclaration:
	CLASS IDENTIFIER '{' PUBLIC STATIC VOID MAIN '(' STRING '[' ']' IDENTIFIER ')' '{' Statement '}' '}' {
		//debugRule(@$, "MainClassDeclaration -> CLASS IDENTIFIER { PUBLIC STATIC VOID MAIN ( STRING [ ] IDENTIFIER ) { Statement } }");
		$$ = new CMainClass( $2, $12, $15, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

ClassDeclarationList:
	/* empty */ {
		//debugRule(@$, "ClassDeclarationList -> empty");
		$$ = nullptr;
	}
	| ClassDeclaration ClassDeclarationList {
		//debugRule(@$, "ClassDeclarationList -> ClassDeclaration ClassDeclarationList");
		$$ = new CClassDeclList( $1, $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

ClassDeclaration:
	CLASS IDENTIFIER '{'  VariableDeclarationList MethodDeclarationList '}' {
		//debugRule(@$, "ClassDeclaration -> CLASS IDENTIFIER {  VariableDeclarationList MethodDeclarationList }");
		$$ = new CClassDecl( $4, $5, $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| CLASS IDENTIFIER EXTENDS IDENTIFIER '{'  VariableDeclarationList MethodDeclarationList '}' {
		//debugRule(@$, "ClassDeclaration -> CLASS IDENTIFIER EXTENDS IDENTIFIER {  VariableDeclarationList MethodDeclarationList }");
		$$ = new CClassDeclDerived( $6, $7, $2, $4, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

VariableDeclarationList:
	/* empty */ {
		//debugRule(@$, "VariableDeclarationList -> empty");
		$$ = nullptr;
	}
	| VariableDeclarationList VariableDeclaration {
		//debugRule(@$, "VariableDeclarationList -> VariableDeclarationList VariableDeclaration");
		$$ = new CVarDeclList( $1, $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

VariableDeclaration:
	Type IDENTIFIER ';' {
		//debugRule(@$, "VariableDeclaration -> Type IDENTIFIER ;");
		$$ = new CVarDecl( $1, $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

MethodDeclarationList:
	/* empty */ {
		//debugRule(@$, "MethodDeclarationList -> empty");
		$$ = nullptr;
	}
	| MethodDeclaration MethodDeclarationList {
		//debugRule(@$, "MethodDeclarationList -> MethodDeclaration MethodDeclarationList");
		$$ = new CMethodDeclList( $1, $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

MethodDeclaration:
	PUBLIC Type IDENTIFIER '(' FormalList ')' '{' VariableDeclarationList StatementList RETURN Expression ';' '}' {
		//debugRule(@$, "MethodDeclaration -> PUBLIC Type IDENTIFIER ( FormalList ) { VariableDeclarationList StatementList RETURN Expression ; }");
		$$ = new CMethodDecl( $2, $3, $5, $8, $9, $11, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

FormalList:
	/* empty */ {
		//debugRule(@$, "FormalList -> empty");
		$$ = nullptr;
	}
	| Type IDENTIFIER FormalRestList {
		//debugRule(@$, "FormalList -> Type IDENTIFIER FormalRestList");
		//$$ = new CFormalList( $1, $2, $3, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
		CFormalList* list = new CFormalList( $3, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
		list->PushParam( new CFormalParam( $1, $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) ) );
		$$ = new CFormalList( list, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

FormalRestList:
	/* empty */ {
		//debugRule(@$, "FormalRestList -> empty");
		$$ = nullptr;
	}
	| FormalRest FormalRestList {
		//debugRule(@$, "FormalRestList -> FormalRest FormalRestList");
		//$$ = new CFormalRestList( $1, $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
		CFormalList* list = new CFormalList( $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
		list->PushParam( $1 );
		$$ = list;
	}

FormalRest:
	',' Type IDENTIFIER {
		//debugRule(@$, "FormalRest -> , Type IDENTIFIER");
		//$$ = new CFormalList( $2, $3, nullptr, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
		$$ = new CFormalParam( $2, $3, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

Type:
	INT '[' ']' {
		//debugRule(@$, "Type -> INT[]");
		$$ = new CStandardType( CStandardType::StandardType::INT_ARRAY, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| BOOL {
		//debugRule(@$, "Type -> BOOL");
		$$ = new CStandardType( CStandardType::StandardType::BOOL, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| INT {
		//debugRule(@$, "Type -> INT");
		$$ = new CStandardType( CStandardType::StandardType::INT, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| IDENTIFIER {
		//debugRule(@$, "Type -> CLASS");
		$$ = new CUserType( $1, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

StatementList:
	/* empty */ {
		//debugRule(@$, "StatementList -> empty");
		$$ = nullptr;
	}
	| Statement StatementList {
		//debugRule(@$, "StatementList -> Statement StatementList");
		$$ = new CStatementList( $1, $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

Statement:
	'{' StatementList '}' {
		//debugRule(@$, "Statement -> { StatementList }");
		$$ = new CStatementListStatement( $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| IF '(' Expression ')' Statement ELSE Statement {
		//debugRule(@$, "Statement -> IF ( Expression ) Statement ELSE Statement");
		$$ = new CIfStatement( $3, $5, $7, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| WHILE '(' Expression ')' Statement {
		//debugRule(@$, "Statement -> WHILE ( Expression ) Statement");
		$$ = new CWhileStatement( $3, $5, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| PRINTLN '(' Expression ')' ';' {
		//debugRule(@$, "Statement -> PRINTLN ( Expression ) ;");
		$$ = new CPrintStatement( $3, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| IDENTIFIER '=' Expression ';' {
		//debugRule(@$, "Statement -> IDENTIFIER = Expression ;");
		$$ = new CAssignStatement( $1, $3, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| IDENTIFIER '[' Expression ']' '=' Expression ';' {
		//debugRule(@$, "Statement -> IDENTIFIER [ Expression ] = Expression ;");
		$$ = new CArrayAssignStatement( $1, $3, $6, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

Expression:
	Expression AND Expression {
		//debugRule(@$, "Expression -> Expression AND Expression");
		$$ = new CBinOpExpression( $1, CBinOpExpression::BinOp::AND, $3, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| Expression '<' Expression {
		//debugRule(@$, "Expression -> Expression < Expression");
		$$ = new CBinOpExpression( $1, CBinOpExpression::BinOp::LESS, $3, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| Expression '+' Expression {
		//debugRule(@$, "Expression -> Expression + Expression");
		$$ = new CBinOpExpression( $1, CBinOpExpression::BinOp::PLUS, $3, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	} 
	| Expression '-' Expression {
		//debugRule(@$, "Expression -> Expression - Expression");
		$$ = new CBinOpExpression( $1, CBinOpExpression::BinOp::MINUS, $3, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| '-' Expression %prec UMINUS {
		//debugRule(@$, "Expression -> -Expression");
		$$ = new CUnaryOpExpression( CUnaryOpExpression::UnaryOp::MINUS, $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| Expression '*' Expression {
		//debugRule(@$, "Expression -> Expression * Expression");
		$$ = new CBinOpExpression( $1, CBinOpExpression::BinOp::TIMES, $3, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| Expression '/' Expression {
		//debugRule(@$, "Expression -> Expression / Expression");
		$$ = new CBinOpExpression( $1, CBinOpExpression::BinOp::DIVIDE, $3, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| Expression '[' Expression ']' {
		//debugRule(@$, "Expression -> Expression[Expression]");
		$$ = new CIndexExpression( $1, $3, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| Expression '.' LENGTH {
		//debugRule(@$, "Expression -> Expression.LENGTH");
		$$ = new CLenghtExpression( $1, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| Expression '.' IDENTIFIER '(' ExpressionList ')' {
		//debugRule(@$, "Expression -> Expression.IDENTIFIER(ExpressionList)");
		$$ = new CMethodExpression( $1, $3, $5, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| INTEGER_VAL {
		//debugRule(@$, (std::string("Expression -> INTEGER_VALUE(") + std::string($1) + std::string(")")).c_str() );
		$$ = new CIntLiteralExpression( $1, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| BOOLEAN_VAL {
		//debugRule(@$, (std::string("Expression -> BOOLEAN_VALUE(") + std::string($1) + std::string(")")).c_str() );
		$$ = new CBoolLiteralExpression( $1, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| IDENTIFIER {
		//debugRule(@$, (std::string("Expression -> IDENTIFIER(") + std::string($1) + std::string(")")).c_str() );
		$$ = new CIdentifierExpression( $1, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| THIS {
		//debugRule(@$, "Expression -> THIS");
		$$ = new CThisExpression( CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| NEW INT '[' Expression ']' {
		//debugRule(@$, "Expression -> new int[]");
		$$ = new CNewIntArrayExpression( $4, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| NEW IDENTIFIER '(' ')' {
		//debugRule(@$, "Expression -> new IDENTIFIER()");
		$$ = new CNewExpression( $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );	
	}
	| '!' Expression  {
		//debugRule(@$, "Expression -> !Expression");
		$$ = new CUnaryOpExpression( CUnaryOpExpression::UnaryOp::NOT, $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}
	| '(' Expression ')' {
		//debugRule(@$, "Expression -> (Expression)");
		$$ = new CBracesExpression( $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

ExpressionList:
	/* empty */ {
		//debugRule(@$, "ExpressionList -> empty");
		$$ = nullptr;
	}
	| Expression ExpressionRestList {
		//debugRule(@$, "ExpressionList -> Expression ExpressionRestList");
		$$ = new CExpressionList( $1, $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

ExpressionRestList:
	/* empty */ {
		//debugRule(@$, "ExpressionRestList -> empty");
		$$ = nullptr;
	}
	| ExpressionRest ExpressionRestList {
		//debugRule(@$, "ExpressionRestList -> Expression ExpressionRestList");
		$$ = new CExpressionList( $1, $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

ExpressionRest:
	',' Expression {
		//debugRule(@$, "ExpressionRest -> , Expression");
		$$ = new CExpressionRest( $2, CPosition( yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column ) );
	}

%%

void yyerror(std::shared_ptr<IProgram>& root, const char *s) 
{
	//std::cout << "(" << yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column.first_line << ", " << yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column.first_column << "; "
	//	<< yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column.last_line << ", " << yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column.last_column << ")  ";
	std::cout << s;
}