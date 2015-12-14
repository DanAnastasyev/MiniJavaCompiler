#pragma once

#include "ErrorStorage.h"

class CExpressionList;
class CBracesExpression;
class CUnaryOpExpression;
class CNewExpression;
class CNewIntArrayExpression;
class CThisExpression;
class CIdentifierExpression;
class CBoolLiteralExpression;
class CIntLiteralExpression;
class CMethodExpression;
class CLenghtExpression;
class CIndexExpression;
class CBinOpExpression;
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
class CVarDeclList;
class CClassDeclDerived;
class CClassDecl;
class CClassDeclList;
class CProgram;
class CMainClass;
class CStatementList;
class CMethodDeclList;
class CExpressionRest;
class CFormalList;
//class CFormalRestList;
class CFormalParam;

class IVisitor {
public:
	virtual ~IVisitor() 
	{}

	virtual void Visit( const CProgram* program ) = 0;
	virtual void Visit( const CMainClass* mainClass ) = 0;
	virtual void Visit( const CClassDeclList* classDeclList ) = 0;
	virtual void Visit( const CClassDecl* classDecl ) = 0;
	virtual void Visit( const CClassDeclDerived* classDeclDerived ) = 0;
	virtual void Visit( const CVarDecl* varDecl ) = 0;
	virtual void Visit( const CVarDeclList* varDeclList ) = 0;
	virtual void Visit( const CMethodDecl* methodDecl ) = 0;
	virtual void Visit( const CStandardType* type ) = 0;
	virtual void Visit( const CUserType* type ) = 0;
	virtual void Visit( const CStatementListStatement* statement ) = 0;
	virtual void Visit( const CArrayAssignStatement* statement ) = 0;
	virtual void Visit( const CAssignStatement* statement ) = 0;
	virtual void Visit( const CIfStatement* statement ) = 0;
	virtual void Visit( const CWhileStatement* statement ) = 0;
	virtual void Visit( const CPrintStatement* statement ) = 0;
	virtual void Visit( const CBinOpExpression* expr ) = 0;
	virtual void Visit( const CIndexExpression* expr ) = 0;
	virtual void Visit( const CLenghtExpression* expr ) = 0;
	virtual void Visit( const CMethodExpression* expr ) = 0;
	virtual void Visit( const CIntLiteralExpression* expr ) = 0;
	virtual void Visit( const CBoolLiteralExpression* expr ) = 0;
	virtual void Visit( const CIdentifierExpression* expr ) = 0;
	virtual void Visit( const CThisExpression* expr ) = 0;
	virtual void Visit( const CNewIntArrayExpression* expr ) = 0;
	virtual void Visit( const CNewExpression* expr ) = 0;
	virtual void Visit( const CUnaryOpExpression* expr ) = 0;
	virtual void Visit( const CBracesExpression* expr ) = 0;
	virtual void Visit( const CExpressionList* exprList ) = 0;
	virtual void Visit( const CStatementList* stmtList ) = 0;
	virtual void Visit( const CExpressionRest* expr ) = 0;
	virtual void Visit( const CMethodDeclList* methodList ) = 0;
	virtual void Visit( const CFormalList* list ) = 0;
	virtual void Visit( const CFormalParam* list ) = 0;
};

namespace IRTree {

	class CMove;
	class CExpr;
	class CJump;
	class CCondJump;
	class CSeq;
	class CLabel;
	class CConst;
	class CName;
	class CTemp;
	class CBinop;
	class CMem;
	class CCall;
	class CESeq;

	class IIRTreeVisitor {
	public:
		virtual ~IIRTreeVisitor()
		{}

		virtual void Visit( const CMove* node ) = 0;
		virtual void Visit( const CExpr* node ) = 0;
		virtual void Visit( const CJump* node ) = 0;
		virtual void Visit( const CCondJump* node ) = 0;
		virtual void Visit( const CSeq* node ) = 0;
		virtual void Visit( const CConst* node ) = 0;
		virtual void Visit( const CName* node ) = 0;
		virtual void Visit( const CTemp* node ) = 0;
		virtual void Visit( const CBinop* node ) = 0;
		virtual void Visit( const CMem* node ) = 0;
		virtual void Visit( const CCall* node ) = 0;
		virtual void Visit( const CESeq* node ) = 0;
		virtual void Visit( const CLabel* node ) = 0;
	};

}