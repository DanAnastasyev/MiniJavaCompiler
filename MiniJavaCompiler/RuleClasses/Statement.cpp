#include "RuleClasses.h"

CStatementListStatement::CStatementListStatement( IStatementList* _statementList, const CPosition& _position ) :
CPositionStorage( _position ),
statementList( _statementList )
{
}

const IStatementList* CStatementListStatement::GetStatementList() const
{
	return statementList.get();
}

void CStatementListStatement::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CIfStatement::CIfStatement( IExp* _condition, IStatement* _statementIfTrue, IStatement* _statementIfFalse, const CPosition _position ) :
CPositionStorage( _position ),
condition( _condition ),
statementIfTrue( _statementIfTrue ),
statementIfFalse( _statementIfFalse )
{
}

const IExp* CIfStatement::GetCondition() const
{
	return condition.get();
}

const IStatement* CIfStatement::GetIfTrueStatement( ) const
{
	return statementIfTrue.get( );
}

const IStatement* CIfStatement::GetIfFalseStatement() const
{
	return statementIfFalse.get();
}

void CIfStatement::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CWhileStatement::CWhileStatement( IExp* _condition, IStatement* _cycleBody, const CPosition& _position ) :
CPositionStorage( _position ),
condition( _condition ),
cycleBody( _cycleBody )
{
}

const IExp* CWhileStatement::GetCondition() const
{
	return condition.get();
}

const IStatement* CWhileStatement::GetBodyCycle() const
{
	return cycleBody.get();
}

void CWhileStatement::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CPrintStatement::CPrintStatement( IExp* _expression, const CPosition& _position ) :
CPositionStorage( _position ),
expression( _expression )
{
}

const IExp* CPrintStatement::GetExpression() const
{
	return expression.get();
}

void CPrintStatement::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CAssignStatement::CAssignStatement( const std::string& _left, IExp* _right, const CPosition& _position ) :
CPositionStorage( _position ),
left( _left ),
right( _right )
{
}

std::string CAssignStatement::GetLeftPart( ) const
{
	return left;
}

const IExp* CAssignStatement::GetRightPart() const
{
	return right.get();
}

void CAssignStatement::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CArrayAssignStatement::CArrayAssignStatement( const std::string& _arrayId, IExp* _elementNumber, IExp* _rightPart, const CPosition& _position ) :
CPositionStorage( _position ),
arrayId( CSymbol::GetSymbol( _arrayId ) ),
elementNumber( _elementNumber ),
rightPart( _rightPart )
{
}

const CSymbol* CArrayAssignStatement::GetArrayName() const
{
	return arrayId;
}

const IExp* CArrayAssignStatement::GetElementNumber() const
{
	return elementNumber.get();
}

const IExp* CArrayAssignStatement::GetRightPart() const
{
	return rightPart.get();
}

void CArrayAssignStatement::Accept( IVisitor* visitor ) const
{
	return visitor->Visit( this );
}

CStatementList::CStatementList( IStatement* _stmt, IStatementList* _stmtList, const CPosition& pos ) :
CPositionStorage( pos ),
stmt( _stmt ),
stmtList( _stmtList )
{
}

const IStatement* CStatementList::GetStatement() const
{
	return stmt.get();
}

const IStatementList* CStatementList::GetStatementList() const
{
	return stmtList.get();
}

void CStatementList::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}