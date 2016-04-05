#pragma once

#include "IRTree/Expressions.h"
#include "IRTree/Statements.h"

class CMoveCall : public IRTree::IStm {
public:
	CMoveCall( std::shared_ptr<const IRTree::CTemp> dst, std::shared_ptr<const IRTree::CCall> src );

	std::shared_ptr<const IRTree::CTemp> Dst() const
	{
		return dst;
	}
	std::shared_ptr<const IRTree::CCall> Src() const
	{
		return src;
	}

	IRTree::CExprList* Kids() const override;
	IRTree::IStm* Build(const IRTree::CExprList* kids) const override;
	void Accept( IRTree::IIRTreeVisitor* visitor ) const override {}

private:
	std::shared_ptr<const IRTree::CTemp> dst;
	std::shared_ptr<const IRTree::CCall> src;
};
typedef std::shared_ptr<CMoveCall> CMoveCallPtr;

class CExprCall : public IRTree::IStm {
public:
	explicit CExprCall( std::shared_ptr<const IRTree::CCall> call );

	std::shared_ptr<const IRTree::CCall> GetCall() const
	{
		return call;
	}

	IRTree::CExprList* Kids() const override;
	IRTree::IStm* Build(const IRTree::CExprList* kids) const override;
	void Accept( IRTree::IIRTreeVisitor* visitor ) const override {}

private:
	std::shared_ptr<const IRTree::CCall> call;
};
typedef std::shared_ptr<const CExprCall> CExprCallPtr;

class CStmExprList {
public:
	CStmExprList( std::shared_ptr<const IRTree::IStm> stm, std::shared_ptr<const IRTree::CExprList> exprs );

	std::shared_ptr<const IRTree::IStm> Stm() const
	{
		return stm;
	}
	std::shared_ptr<const IRTree::CExprList> Exprs() const
	{
		return exprs;
	}

private:
	std::shared_ptr<const IRTree::IStm> stm;
	std::shared_ptr<const IRTree::CExprList> exprs;
};

class CCanon {
public:
	static IRTree::CStmListPtr Linearize( IRTree::IStmPtr stm );

private:
	static CStmExprList nopNull;

	// ќпределение коммутативности узлов
	static bool isCommute( IRTree::IStmPtr a, IRTree::IExprPtr b );
	// явл€етс€ ли это экспрешеном с константным значением
	static bool isNop( IRTree::IStmPtr a );

	static IRTree::IStmPtr createSeq( IRTree::IStmPtr firstStm, IRTree::IStmPtr secondStm );

	static IRTree::IStmPtr doStm( IRTree::CSeqPtr seq );
	static IRTree::IStmPtr doStm( IRTree::CMovePtr  move );
	static IRTree::IStmPtr doStm( IRTree::CExprPtr expr );
	static IRTree::IStmPtr doStm( IRTree::IStmPtr stm );

	static IRTree::IStmPtr reorderStm( IRTree::IStmPtr stm );

	static IRTree::CESeqPtr doExp( IRTree::CESeqPtr eseq );
	static IRTree::CESeqPtr doExp( IRTree::IExprPtr eseq );

	static IRTree::CESeqPtr reorderExp( IRTree::IExprPtr expr );

	static std::shared_ptr<CStmExprList> reorder( const IRTree::CExprList* exprs );

	static IRTree::CStmListPtr linear( IRTree::CSeqPtr seq, IRTree::CStmListPtr list );
	static IRTree::CStmListPtr linear( IRTree::IStmPtr stm, IRTree::CStmListPtr list );
};