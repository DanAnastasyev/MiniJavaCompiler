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
	//void Visit( const IVarDeclList* program );
	void Visit( const CVarDecl* program );
	//void Visit( const IMethodDeclList* program );
	void Visit( const CMethodDecl* program );
	//void Visit( const IFormalList* program );
	void Visit( const CStandardType* program );
	void Visit( const CUserType* program );
	//void Visit( const IStatementList* program );
	void Visit( const CStatementListStatement* statement );
	void Visit( const CArrayAssignStatement* statement );
	void Visit( const CAssignStatement* statement );
	void Visit( const CIfStatement* statement );
	void Visit( const CWhileStatement* statement );
	void Visit( const CPrintStatement* statement );
//	void Visit( const IExpList* program );
//	void Visit( const IExp* program );
};