#pragma once

class CArrayAssignStatement;
class CAssignStatement;
class CStatementListStatement;
class CIfStatement;
class CPrintStatement;
class CWhileStatement;
class CStandardType;
class CUserType;
class CMethodDecl;
class CVarDecl;
class CClassDeclDerived;
class CClassDecl;
class CClassDeclList;
class CProgram;
class CMainClass;

class IVisitor {
public:
	void Visit( const CProgram* program );
	void Visit( const CMainClass* program );
	void Visit( const CClassDeclList* program );
	void Visit( const CClassDecl* program );
	void Visit( const CClassDeclDerived* program );
	void Visit( const CVarDecl* program );
	void Visit( const CMethodDecl* program );
	void Visit( const CStandardType* program );
	void Visit( const CUserType* program );
	void Visit( const CStatementListStatement* statement );
	void Visit( const CArrayAssignStatement* statement );
	void Visit( const CAssignStatement* statement );
	void Visit( const CIfStatement* statement );
	void Visit( const CWhileStatement* statement );
	void Visit( const CPrintStatement* statement );
	void Visit( const CBinOpExpression* expr );
	void Visit( const CIndexExpression* expr );
	void Visit( const CLenghtExpression* expr );
	void Visit( const CMethodExpression* expr );
	void Visit( const CIntLiteralExpression* expr );
	void Visit( const CBoolLiteralExpression* expr );
	void Visit( const CIdentifierExpression* expr );
	void Visit( const CThisExpression* expr );
	void Visit( const CNewIntArrayExpression* expr );
	void Visit( const CNewExpression* expr );
	void Visit( const CIntLiteralExpression* expr );
	void Visit( const CUnaryOpExpression* expr );
	void Visit( const CBracesExpression* expr );
	void Visit( const CExpressionList* exprList );
};