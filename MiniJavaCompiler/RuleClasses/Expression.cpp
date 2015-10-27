#include "RuleClasses.h"

CBinOpExpression::CBinOpExpression( IExp* _leftExp, BinOp _binOp, IExp* _rightExp, const CPosition& pos ) :
leftExp( _leftExp ),
rightExp( _rightExp ),
position( pos ),
binOp( _binOp )
{
}

CBinOpExpression::BinOp CBinOpExpression::GetBinOp() const
{
	return binOp;
}

const IExp* CBinOpExpression::GetLeftExp() const
{
	return leftExp.get();
}

const IExp* CBinOpExpression::GetRightExp() const
{
	return rightExp.get();
}

void CBinOpExpression::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CIndexExpression::CIndexExpression( IExp* _exp, IExp* _indexExp, const CPosition& pos ) :
exp( _exp ),
indexExp( _indexExp ),
position( pos )
{
}

const IExp* CIndexExpression::GetExp() const
{
	return exp.get();
}

const IExp* CIndexExpression::GetIndexExp() const
{
	return indexExp.get();
}

void CIndexExpression::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CLenghtExpression::CLenghtExpression( IExp* _exp, const CPosition& pos ) :
exp( _exp ),
position( pos )
{
}

const IExp* CLenghtExpression::GetExp() const
{
	return exp.get();
}

void CLenghtExpression::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CMethodExpression::CMethodExpression( IExp* _exp, const std::string& _identifier, IExpList* _expList, const CPosition& pos ) :
exp( _exp ),
expList( _expList ),
identifier( _identifier ),
position( pos )
{
}

const IExp* CMethodExpression::GetExp() const
{
	return exp.get();
}

const IExpList* CMethodExpression::GetIndexExp() const
{
	return expList.get();
}

std::string CMethodExpression::GetIdentifier() const
{
	return identifier;
}

void CMethodExpression::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CIntLiteralExpression::CIntLiteralExpression( int _val, const CPosition& pos ) :
val( _val ),
position( pos )
{
}

int CIntLiteralExpression::GetValue() const
{
	return val;
}

void CIntLiteralExpression::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CBoolLiteralExpression::CBoolLiteralExpression( bool _val, const CPosition& pos ) :
val( _val ),
position( pos )
{
}

bool CBoolLiteralExpression::GetValue() const
{
	return val;
}

void CBoolLiteralExpression::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CIdentifierExpression::CIdentifierExpression( const std::string& id, const CPosition& pos ) :
identifier( id ),
position( pos )
{
}

std::string CIdentifierExpression::GetIdentifier() const
{
	return identifier;
}

void CIdentifierExpression::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CThisExpression::CThisExpression( const CPosition& pos ) :
position( pos )
{
}

void CThisExpression::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CNewIntArrayExpression::CNewIntArrayExpression( IExp* _exp, const CPosition& pos ) :
exp( _exp ),
position( pos )
{
}

const IExp* CNewIntArrayExpression::GetExp() const
{
	return exp.get();
}

void CNewIntArrayExpression::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CNewExpression::CNewExpression( const std::string& id, const CPosition& pos ) :
identifier( id ),
position( pos )
{
}

std::string CNewExpression::GetIdentifier() const
{
	return identifier;
}

void CNewExpression::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CUnaryOpExpression::CUnaryOpExpression( UnaryOp _op, IExp* _exp, const CPosition& pos ) :
exp( _exp ),
position( pos ),
op( _op )
{
}

const IExp* CUnaryOpExpression::GetLeftExp() const
{
	return exp.get();
}

CUnaryOpExpression::UnaryOp CUnaryOpExpression::GetOperation( ) const
{
	return op;
}

void CUnaryOpExpression::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CBracesExpression::CBracesExpression( IExp* _exp, const CPosition& pos ) :
exp( _exp ),
position( pos )
{
}

const IExp* CBracesExpression::GetExp() const
{
	return exp.get();
}

void CBracesExpression::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CExpressionList::CExpressionList( IExp* _exp, IExpList* _expList, const CPosition& pos ) :
exp( _exp ),
expList( _expList ),
position( pos )
{
}

const IExp* CExpressionList::GetExp() const
{
	return exp.get();
}

const IExpList* CExpressionList::GetExpList( ) const
{
	return expList.get( );
}

void CExpressionList::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CExpressionRest::CExpressionRest( IExp* _exp, const CPosition& pos ) :
exp( _exp ),
position( pos )
{
}

const IExp* CExpressionRest::GetExp() const
{
	return exp.get();
}

void CExpressionRest::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}