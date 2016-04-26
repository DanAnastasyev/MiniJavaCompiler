#include <CodeGeneration/CodeGeneration.h>

using namespace IRTree;

#define INSTANCEOF(instance, clazz) (std::dynamic_pointer_cast<const clazz>( instance ) != nullptr)
#define CAST(instance, clazz) (std::dynamic_pointer_cast<const clazz>(instance))

std::shared_ptr<const Assembler::CBaseInstructionList> CCodeGeneration::GenerateCode(IRTree::IStmPtr stm)
{
	Assembler::CBaseInstructionList* list;
	munchStm( stm );
	list = instructList;
	instructList = last = nullptr;
	return std::shared_ptr<const Assembler::CBaseInstructionList>( list );
}

void CCodeGeneration::emit( Assembler::CBaseInstruction* instruct )
{
	if( last != nullptr ) {
		last = last->tail = new Assembler::CBaseInstructionList( instruct, nullptr );
	} else {
		last = instructList = new Assembler::CBaseInstructionList( instruct, nullptr );
	}
}

void CCodeGeneration::munchStm(CSeqPtr stm)
{
	munchStm( stm->GetLeftStm() );
	munchStm( stm->GetRightStm() );
}

void CCodeGeneration::munchStm( CMovePtr stm )
{
	// MOVE( MEM(...) )
	if( INSTANCEOF( stm->GetDestExpr(), CMem ) ) {
		auto destMem = std::dynamic_pointer_cast<const CMem>( stm->GetDestExpr() );
		
		if( INSTANCEOF( destMem->GetMem(), CBinop ) ) {
			// MOVE( MEM( BINOP(PLUS, e1, CONST(i)), e2 ) )
			auto binOp = std::dynamic_pointer_cast<const CBinop>( destMem->GetMem() );
			IExprPtr binOpExpr;
			CConstPtr constantExpr;
			if( INSTANCEOF( binOp->GetLeft(), CConst ) ) {
				binOpExpr = binOp->GetRight();
				constantExpr = std::dynamic_pointer_cast<const CConst>( binOp->GetLeft() );
			} else {
				binOpExpr = binOp->GetLeft();
				constantExpr = std::dynamic_pointer_cast<const CConst>( binOp->GetRight() );
			}
			emit( new Assembler::COper( std::string( "STORE M['s0+" ) +
				std::to_string( constantExpr->GetValue() ) +
				std::string( "] <- 's1\n" ),
				nullptr,
				std::make_shared<const Temp::CTempList>(
				munchExp( binOpExpr ),
				std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) ) );
		} else if( INSTANCEOF( destMem->GetMem(), CConst ) ) {
			// MOVE( MEM( CONST(i) ), e2 )
			CConstPtr constantExpr = std::dynamic_pointer_cast<const CConst>( destMem->GetMem() );
			emit( new Assembler::COper( std::string( "STORE M['s0+" ) +
				std::to_string( constantExpr->GetValue( ) ) +
				std::string( "] <- 's1\n" ),
				nullptr,
				std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) );
		} else if( INSTANCEOF( destMem->GetMem(), CMem ) ) {
			if( INSTANCEOF( stm->GetSrcExpr(), CMem ) ) {
				// MOVE( MEM(e1), MEM(e2) )
				emit( new Assembler::COper( std::string( "STORE M['s0] <- M['s1]\n" ),
					nullptr,
					std::make_shared<const Temp::CTempList>( 
					munchExp( stm->GetSrcExpr() ),
					std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) ) );
			} else {
				// MOVE( MEM(e1), e2 )
				emit( new Assembler::COper( std::string( "STORE M['s0] <- 's1\n" ),
					nullptr,
					std::make_shared<const Temp::CTempList>(
					munchExp( stm->GetSrcExpr() ),
					std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) ) );
			}
		}
	} else if( INSTANCEOF( stm->GetDestExpr(), CTemp ) ) { 
		// MOVE(TEMP(i), e2)
		CTempPtr temp = std::dynamic_pointer_cast<const CTemp>( stm->GetDestExpr() );
		emit( new Assembler::COper( "ADD 'd0 <- 's0 + r0\n",
			std::make_shared<const Temp::CTempList>( temp->GetTemp(), nullptr ),
			std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) );
	}
}

void CCodeGeneration::munchStm(IRTree::CLabelPtr stm)
{
	emit( new Assembler::CLabel( stm->GetLabel()->GetName()->GetString() + std::string(":\n"), stm->GetLabel() ) );
}

void CCodeGeneration::munchStm( IRTree::CExprPtr stm )
{
	munchExp( stm->GetExp() );
}

void CCodeGeneration::munchStm( IRTree::CJumpPtr stm )
{
	emit( new Assembler::COper( "JMP 'j0\n", nullptr, nullptr, stm->GetTargets() ) );
}

void CCodeGeneration::munchStm( IRTree::CCondJumpPtr stm )
{
	//if( !INSTANCEOF( stm->GetExpr(), CBinop ) ) {
	//	
	//}
	//emit( new Assembler::COper( "CMP 's0, 's1\n", nullptr,
	//	std::shared_ptr<const Temp::CTempList>( munchExp( stm-> ) ) ) );
}

void CCodeGeneration::munchStm( IRTree::IStmPtr stm )
{
	if( INSTANCEOF( stm, CSeq ) ) {
		munchStm( CAST( stm, CSeq ) );
	} else if( INSTANCEOF( stm, CMove ) ) {
		munchStm( CAST( stm, CMove ) );
	} else if( INSTANCEOF( stm, CLabel ) ) {
		munchStm( CAST( stm, CLabel ) );
	} else if( INSTANCEOF( stm, CExpr ) ) {
		munchStm( CAST( stm, CExpr ) );
	} else if( INSTANCEOF( stm, CJump ) ) {
		munchStm( CAST( stm, CJump ) );
	} else if( INSTANCEOF( stm, CCondJump ) ) {
		munchStm( CAST( stm, CCondJump ) );
	}
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CMemPtr expr )
{
	std::shared_ptr<const Temp::CTemp> r;
	if( INSTANCEOF( expr->GetMem(), CBinop ) ) {
		auto binOp = std::dynamic_pointer_cast<const CBinop>( expr );
		IExprPtr binOpExpr;
		CConstPtr constantExpr;
		if( INSTANCEOF( binOp->GetLeft( ), CConst ) ) {
			binOpExpr = binOp->GetRight( );
			constantExpr = std::dynamic_pointer_cast<const CConst>( binOp->GetLeft( ) );
		} else {
			binOpExpr = binOp->GetLeft( );
			constantExpr = std::dynamic_pointer_cast<const CConst>( binOp->GetRight( ) );
		}
		emit( new Assembler::COper( std::string( "LOAD 'd0 <- M['s0+" ) +
			std::to_string( constantExpr->GetValue() ) +
			std::string( "]\n" ),
			std::make_shared<const Temp::CTempList>( r, nullptr ),
			std::make_shared<const Temp::CTempList>( munchExp( binOpExpr ), nullptr ) ) );
	} else if( INSTANCEOF( expr->GetMem(), CConst ) ) {
		auto constantExpr = std::dynamic_pointer_cast<const CConst>( expr->GetMem( ) );
		emit( new Assembler::COper( std::string( "LOAD 'd0 <- M['r0+" ) +
			std::to_string( constantExpr->GetValue() ) +
			std::string( "]\n" ),
			std::make_shared<const Temp::CTempList>( r, nullptr ), 
			nullptr ) );
	} else {
		emit( new Assembler::COper( "LOAD 'd0 <- M['s0+0]\n",
			std::make_shared<const Temp::CTempList>( r, nullptr ),
			std::make_shared<const Temp::CTempList>( munchExp( expr->GetMem() ), nullptr ) ) );
	}

	return r;
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CBinopPtr binOp )
{
	std::shared_ptr<const Temp::CTemp> r;
	if( INSTANCEOF( binOp->GetLeft(), CConst ) || INSTANCEOF( binOp->GetRight(), CConst ) ) {
		IExprPtr binOpExpr;
		CConstPtr constantExpr;
		if( INSTANCEOF( binOp->GetLeft(), CConst ) ) {
			binOpExpr = binOp->GetRight();
			constantExpr = std::dynamic_pointer_cast<const CConst>( binOp->GetLeft( ) );
		} else {
			binOpExpr = binOp->GetLeft();
			constantExpr = std::dynamic_pointer_cast<const CConst>( binOp->GetRight( ) );
		}
		emit( new Assembler::COper( std::string( "ADDI 'd0 <- 's0+" ) +
			std::to_string( constantExpr->GetValue() ) +
			std::string( "\n" ),
			std::make_shared<const Temp::CTempList>( r, nullptr ),
			std::make_shared<const Temp::CTempList>( munchExp( binOpExpr ), nullptr ) ) );
	} else {
		emit( new Assembler::COper( "ADD 'd0 <- 's0+'s1\n",
			std::make_shared<const Temp::CTempList>( r, nullptr ),
			std::make_shared<const Temp::CTempList>( munchExp( binOp->GetLeft() ), 
			std::make_shared<const Temp::CTempList>( munchExp( binOp->GetRight() ), nullptr ) ) ) );
	}

	return r;
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CConstPtr constantExpr )
{
	std::shared_ptr<const Temp::CTemp> r;
	
	emit( new Assembler::COper( std::string( "ADDI 'd0 <- r0+" ) +
		std::to_string( constantExpr->GetValue() ) +
		std::string( "\n" ),
		nullptr,
		std::make_shared<const Temp::CTempList>( r, nullptr ) ) );

	return r;
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CTempPtr expr )
{
	return expr->GetTemp();
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CNamePtr expr )
{
	//TODO
	return std::make_shared<const Temp::CTemp>();
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CCallPtr expr )
{
	//TODO
	return std::make_shared<const Temp::CTemp>();
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::IExprPtr expr )
{
	if( INSTANCEOF( expr, CMem ) ) {
		return munchExp( CAST( expr, CMem ) );
	}
	if( INSTANCEOF( expr, CBinop ) ) {
		return munchExp( CAST( expr, CBinop ) );
	}
	if( INSTANCEOF( expr, CConst ) ) {
		return munchExp( CAST( expr, CConst ) );
	}
	if( INSTANCEOF( expr, CTemp ) ) {
		return munchExp( CAST( expr, CTemp ) );
	}
	if( INSTANCEOF( expr, CName ) ) {
		return munchExp( CAST( expr, CName ) );
	}
	if( INSTANCEOF( expr, CCall ) ) {
		return munchExp( CAST( expr, CCall ) );
	}
}
