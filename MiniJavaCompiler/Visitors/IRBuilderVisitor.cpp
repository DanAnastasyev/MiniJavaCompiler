#include "Visitors/IRBuilderVisitor.h"
#include "RuleClasses/RuleClasses.h"

IRBuilderVisitor::IRBuilderVisitor( SymbolsTable::CTable* _table )
{
	symbolsTable = _table;
}

void IRBuilderVisitor::Visit( const CProgram* program )
{
	program->GetMainClass()->Accept( this );
	program->GetClassDeclList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CMainClass* mainClass )
{
	curClass = symbolsTable->GetClass( mainClass->GetClassName()->GetString() );
	curMethod = curClass->GetMethod( "main" );

	frames.emplace_back( curClass, curMethod, symbolsTable );

	mainClass->GetStatement()->Accept( this );

	std::shared_ptr<IRTree::IStm> statements = nullptr;
	if( mainClass->GetStatement() != nullptr ) {
		mainClass->GetStatement()->Accept( this );
		statements = parsedStatements.top();
		parsedStatements.pop();
	}

	frames.back().AddStatements( statements );
}

void IRBuilderVisitor::Visit( const CClassDeclList* classDeclList )
{
	classDeclList->GetClassDecl()->Accept( this );
	classDeclList->GetClassDeclList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CClassDecl* classDecl )
{
	curClass = symbolsTable->GetClass( classDecl->GetName()->GetString() );

	classDecl->GetMethodDeclList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CClassDeclDerived* classDeclDerived )
{
	curClass = symbolsTable->GetClass( classDeclDerived->GetName()->GetString() );

	classDeclDerived->GetMethodDeclList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CVarDecl* varDecl )
{
}

void IRBuilderVisitor::Visit( const CVarDeclList* varDeclList )
{
}

void IRBuilderVisitor::Visit( const CMethodDecl* methodDecl )
{
	curMethod = curClass->GetMethod( methodDecl->GetName()->GetString() );

	frames.emplace_back( curClass, curMethod, symbolsTable );

	std::shared_ptr<const IRTree::IStm> statements;
	if( methodDecl->GetStatementList() != nullptr ) {
		methodDecl->GetStatementList()->Accept( this );
		statements = parsedStatements.top();
		parsedStatements.pop();
	}

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
	expr->GetLeftExp()->Accept( this );
	std::shared_ptr<const IRTree::IExpr> left = parsedExpressions.top();
	parsedExpressions.pop();

	expr->GetRightExp()->Accept( this );
	std::shared_ptr<const IRTree::IExpr> right = parsedExpressions.top();
	parsedExpressions.pop();

	IRTree::CExprPtr binOp;
	switch( expr->GetBinOp() )
	{
	case CBinOpExpression::AND:
		binOp = std::shared_ptr<const IRTree::IExpr>( new IRTree::CBinop( IRTree::IExpr::AND, left, right ) );
	case CBinOpExpression::LESS:
		// TODO: Bicycle or not?
		binOp = std::shared_ptr<const IRTree::IExpr>( new IRTree::CBinop( IRTree::IExpr::LESS, left, right ) );
		break;
	case CBinOpExpression::PLUS:
		binOp = std::shared_ptr<const IRTree::IExpr>( new IRTree::CBinop( IRTree::IExpr::PLUS, left, right ) );
		break;
	case CBinOpExpression::MINUS:
		binOp = std::shared_ptr<const IRTree::IExpr>( new IRTree::CBinop( IRTree::IExpr::MINUS, left, right ) );
		break;
	case CBinOpExpression::TIMES:
		binOp = std::shared_ptr<const IRTree::IExpr>( new IRTree::CBinop( IRTree::IExpr::MUL, left, right ) );
		break;
	case CBinOpExpression::DIVIDE:
		binOp = std::shared_ptr<const IRTree::IExpr>( new IRTree::CBinop( IRTree::IExpr::DIV, left, right ) );
		break;
	}
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
	parsedExpressions.emplace( IRTree::CExprPtr( new IRTree::CConst( expr->GetValue() ) ) );
}

void IRBuilderVisitor::Visit( const CBoolLiteralExpression* expr )
{
	parsedExpressions.emplace( IRTree::CExprPtr( new IRTree::CConst( expr->GetValue() ) ) );
}

void IRBuilderVisitor::Visit( const CIdentifierExpression* expr )
{
	parsedExpressions.emplace( frames.back().GetFormal( expr->GetIdentifier()->GetString() )->GetExp( frames.back().GetFramePtr() ) );
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
	expr->GetRightExp()->Accept( this );
	std::shared_ptr<const IRTree::IExpr> right = parsedExpressions.top();
	parsedExpressions.pop();

	IRTree::CExprPtr binOp;
	if (expr->GetOperation() == CUnaryOpExpression::UnaryOp::MINUS) {
		binOp = std::shared_ptr<const IRTree::IExpr>( 
			new IRTree::CBinop( IRTree::IExpr::MINUS, IRTree::CExprPtr( new IRTree::CConst( 0 ) ), right ) );
	} else {
		// TODO: Check if it is true
		binOp = std::shared_ptr<const IRTree::IExpr>( 
			new IRTree::CBinop( IRTree::IExpr::XOR, IRTree::CExprPtr( new IRTree::CConst( 1 ) ), right ) );
	}

	parsedExpressions.emplace(binOp);
}

void IRBuilderVisitor::Visit( const CBracesExpression* expr )
{
	expr->GetExp()->Accept( this );
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