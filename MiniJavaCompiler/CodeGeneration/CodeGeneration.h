#pragma once
#include <Frame/Frame.h>
#include <Assembler/BaseInstruction.h>

class CCodeGeneration {
public:
	CCodeGeneration( const Frame::CFrame* _frame ) : frame( _frame ) {}

	std::shared_ptr<const Assembler::CBaseInstructionList> GenerateCode( IRTree::IStmPtr stm );
private:
	const Frame::CFrame* frame;
	Assembler::CBaseInstructionList* instructList;
	Assembler::CBaseInstructionList* last;

	void emit( Assembler::CBaseInstruction* instruct );

	void munchStm( IRTree::CSeqPtr stm );
	void munchStm( IRTree::CMovePtr stm );
	void munchStm( IRTree::CLabelPtr stm );
	void munchStm( IRTree::CExprPtr stm );
	void munchStm( IRTree::CJumpPtr stm );
	void munchStm( IRTree::CCondJumpPtr stm );
	void munchStm( IRTree::IStmPtr stm );

	std::shared_ptr<const Temp::CTemp> munchExp( IRTree::CMemPtr expr );
	std::shared_ptr<const Temp::CTemp> munchExp( IRTree::CBinopPtr expr );
	std::shared_ptr<const Temp::CTemp> munchExp( IRTree::CConstPtr expr );
	std::shared_ptr<const Temp::CTemp> munchExp( IRTree::CTempPtr expr );
	std::shared_ptr<const Temp::CTemp> munchExp( IRTree::CNamePtr expr );
	std::shared_ptr<const Temp::CTemp> munchExp( IRTree::CCallPtr expr );
	std::shared_ptr<const Temp::CTemp> munchExp( IRTree::IExprPtr expr );
};
