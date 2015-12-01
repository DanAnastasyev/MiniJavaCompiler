#include "Visitors/IRBuilderVisitor.h"
#include "RuleClasses/RuleClasses.h"

void IRBuilderVisitor::Visit( const CProgram* program )
{
	program->GetMainClass()->Accept( this );
	program->GetClassDeclList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CMainClass* mainClass )
{
	mainClass->GetStatement()->Accept( this );
}

void IRBuilderVisitor::Visit( const CClassDeclList* classDeclList )
{
	classDeclList->GetClassDecl()->Accept( this );
	classDeclList->GetClassDeclList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CClassDecl* classDecl )
{
	classDecl->GetVarDeclList()->Accept( this );
	classDecl->GetMethodDeclList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CClassDeclDerived* classDeclDerived )
{
	classDeclDerived->GetVarDeclList()->Accept( this );
	classDeclDerived->GetMethodDeclList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CVarDecl* varDecl )
{
	varDecl->GetType()->Accept( this );
}

void IRBuilderVisitor::Visit( const CVarDeclList* varDeclList )
{
	varDeclList->GetVarDeclList()->Accept( this );
	varDeclList->GetVarDecl()->Accept( this );
}

void IRBuilderVisitor::Visit( const CMethodDecl* methodDecl )
{
	methodDecl->GetType()->Accept( this );
	methodDecl->GetFormalList()->Accept( this );
	methodDecl->GetVarList()->Accept( this );
	methodDecl->GetStatementList()->Accept( this );
	methodDecl->GetReturnExpression()->Accept( this );
}

void IRBuilderVisitor::Visit( const CStandardType* type )
{
}

void IRBuilderVisitor::Visit( const CUserType* type )
{
}

void IRBuilderVisitor::Visit( const CStatementListStatement* statement )
{
	statement->GetStatementList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CArrayAssignStatement* statement )
{

}

void IRBuilderVisitor::Visit( const CAssignStatement* statement )
{
}

void IRBuilderVisitor::Visit( const CIfStatement* statement )
{
}

void IRBuilderVisitor::Visit( const CWhileStatement* statement )
{
}

void IRBuilderVisitor::Visit( const CPrintStatement* statement )
{
}

void IRBuilderVisitor::Visit( const CBinOpExpression* expr )
{
	expr->GetLeftExp->Accept(*this);
	std::shared_ptr<const IRTree::IExpr> left = parsedExpressions.top();
	parsedExpressions.pop();

	expr->GetRightExp->Accept(*this);
	std::shared_ptr<const IRTree::IExpr> right = parsedExpressions.top();
	parsedExpressions.pop();

	IRTree::CExprPtr binOp;
	switch (expr->GetBinOp())
	{
	case CBinOpExpression::AND:
		binOp = IRTree::CExprPtr(new IRTree::CBinop(left, IRTree::IExpr::AND, right));
	case CBinOpExpression::LESS:
		// TODO: Bicycle or not?
		binOp = IRTree::CExprPtr(new IRTree::CBinop(left, IRTree::IExpr::LESS, right));
		break;
	case CBinOpExpression::PLUS:
		binOp = IRTree::CExprPtr(new IRTree::CBinop(left, IRTree::IExpr::PLUS, right));
		break;
	case CBinOpExpression::MINUS:
		binOp = IRTree::CExprPtr(new IRTree::CBinop(left, IRTree::IExpr::MINUS, right));
		break;
	case CBinOpExpression::TIMES:
		binOp = IRTree::CExprPtr(new IRTree::CBinop(left, IRTree::IExpr::MUL, right));
		break;
	case CBinOpExpression::DIVIDE:
		binOp = IRTree::CExprPtr(new IRTree::CBinop(left, IRTree::IExpr::DIV, right));
		break;
	}
	parsedExpressions.emplace(binOp);
}

void IRBuilderVisitor::Visit( const CIndexExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CLenghtExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CMethodExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CIntLiteralExpression* expr )
{
	parsedExpressions.emplace(IRTree::CExprPtr(new IRTree::CConst(expr->GetValue)));
}

void IRBuilderVisitor::Visit( const CBoolLiteralExpression* expr )
{
	parsedExpressions.emplace(IRTree::CExprPtr(new IRTree::CConst(expr->GetValue)));
}

void IRBuilderVisitor::Visit( const CIdentifierExpression* expr )
{
	parsedExpressions.emplace(frames.top().GetFormal(expr->GetIdentifier()->GetString())->GetExp(frames.top().GetFramePtr()));
}

void IRBuilderVisitor::Visit( const CThisExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CNewIntArrayExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CNewExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CUnaryOpExpression* expr )
{
	expr->GetRightExp.Accept(*this);
	std::shared_ptr<const IRTree::IExpr> right = parsedExpressions.top();
	parsedExpressions.pop();

	IRTree::CExprPtr binOp;
	if (expr->GetOperation() == CUnaryOpExpression::UnaryOp::MINUS) {
		binOp = IRTree::CExprPtr(new IRTree::CBinop(IRTree::CExprPtr(new IRTree::CConst(0)), IRTree::IExpr::MINUS, right));
	} else {
		// TODO: Check if it is true
		binOp = IRTree::CExprPtr(new IRTree::CBinop(IRTree::CExprPtr(new IRTree::CConst(1)), IRTree::IExpr::XOR, right));
	}

	parsedExpressions.emplace(binOp);
}

void IRBuilderVisitor::Visit( const CBracesExpression* expr )
{
	expr->GetExp.Accept(*this);
}

void IRBuilderVisitor::Visit( const CExpressionList* exprList )
{
	exprList->GetExp()->Accept( this );
	exprList->GetExpList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CStatementList* stmtList )
{
	stmtList->GetStatement()->Accept( this );
	stmtList->GetStatementList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CExpressionRest* expr )
{
	expr->GetExp()->Accept( this );
}

void IRBuilderVisitor::Visit( const CMethodDeclList* methodList )
{
	methodList->GetMethodDecl()->Accept( this );
	methodList->GetMethodDeclList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CFormalList* list )
{
}

void IRBuilderVisitor::Visit( const CFormalParam* list )
{
	list->GetType()->Accept( this );
}