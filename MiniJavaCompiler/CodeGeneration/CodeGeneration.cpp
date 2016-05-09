#include <CodeGeneration/CodeGeneration.h>

using namespace IRTree;

#define INSTANCEOF(instance, clazz) (std::dynamic_pointer_cast<const clazz>( instance ) != nullptr)
#define CAST(instance, clazz) (std::dynamic_pointer_cast<const clazz>(instance))

Assembler::CBaseInstructionList* CCodeGeneration::GenerateCode( IRTree::CStmListPtr stmList )
{
	Assembler::CBaseInstructionList* list;
	while( stmList != nullptr ) {
		auto stm = stmList->GetHead();
		munchStm( stm );
		stmList = stmList->GetTail();
	}
	list = instructList;
	instructList = last = nullptr;
	return list;
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
			auto binOp = std::dynamic_pointer_cast<const CBinop>( destMem->GetMem() );
			if( binOp->GetBinOp() == IExpr::PLUS || binOp->GetBinOp() == IExpr::MINUS ) {
				// MOVE( MEM( BINOP(PLUS, e1, CONST(i)), e2 ) )
				if( INSTANCEOF( binOp->GetLeft( ), CConst ) || INSTANCEOF( binOp->GetRight( ), CConst ) ) {
					IExprPtr binOpExpr;
					CConstPtr constantExpr;
					if( INSTANCEOF( binOp->GetLeft(), CConst ) ) {
						binOpExpr = binOp->GetRight();
						constantExpr = std::dynamic_pointer_cast<const CConst>( binOp->GetLeft() );
					} else {
						binOpExpr = binOp->GetLeft();
						constantExpr = std::dynamic_pointer_cast<const CConst>( binOp->GetRight() );
					}
					emit( new Assembler::COper( std::string( "MOV ['s0" ) +
						( ( binOp->GetBinOp() == IExpr::PLUS ) ? "+" : "-" ) +
						std::to_string( constantExpr->GetValue() ) +
						std::string( "], 's1\n" ),
						nullptr,
						std::make_shared<const Temp::CTempList>(
						munchExp( binOpExpr ),
						std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) ) );
				} else {
					// MOVE( MEM( BINOP( PLUS, e1, e2 ) ), e3 )
					emit( new Assembler::COper( std::string( "MOV ['s0], 's1\n" ),
						std::make_shared<const Temp::CTempList>( munchExp( binOp ), nullptr ),
						std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) );
				}
			}
		} else if( INSTANCEOF( destMem->GetMem(), CConst ) ) {
			// MOVE( MEM( CONST(i) ), e2 )
			CConstPtr constantExpr = std::dynamic_pointer_cast<const CConst>( destMem->GetMem() );
			emit( new Assembler::COper( std::string( "MOV ['s0+" ) +
				std::to_string( constantExpr->GetValue( ) ) +
				std::string( "], 's1\n" ),
				nullptr,
				std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) );
		} else if( INSTANCEOF( destMem->GetMem(), CTemp ) ) {
			// MOVE( MEM( TEMP ), e2 )
			emit( new Assembler::COper( std::string( "MOV ['s0], 's1\n" ),
				std::make_shared<const Temp::CTempList>( munchExp( destMem->GetMem() ), nullptr ),
				std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) );
		} else if( INSTANCEOF( destMem->GetMem( ), CMem ) ) {
			if( INSTANCEOF( stm->GetSrcExpr(), CMem ) ) {
				// MOVE( MEM(e1), MEM(e2) )
				emit( new Assembler::COper( std::string( "MOV ['s0], ['s1]\n" ),
					nullptr,
					std::make_shared<const Temp::CTempList>( 
					munchExp( stm->GetSrcExpr() ),
					std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) ) );
			} else {
				// MOVE( MEM(e1), e2 )
				emit( new Assembler::COper( std::string( "MOV ['s0], 's1\n" ),
					nullptr,
					std::make_shared<const Temp::CTempList>(
					munchExp( stm->GetSrcExpr() ),
					std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) ) );
			}
		}
	} else if( INSTANCEOF( stm->GetDestExpr(), CTemp ) ) { 
		// MOVE(TEMP(i), e2)
		CTempPtr temp = std::dynamic_pointer_cast<const CTemp>( stm->GetDestExpr() );
		emit( new Assembler::COper( "ADD 'd0, 's0 + r0\n",
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
	if( !INSTANCEOF( stm->GetExpr(), CBinop ) ) {
		throw std::exception( "wrong conditional jump" );
	}
	auto binOp = CAST( stm->GetExpr(), CBinop );
	emit( new Assembler::COper( "CMP 's0, 's1\n", nullptr, std::make_shared<const Temp::CTempList>( munchExp( binOp->GetLeft() ),
		std::make_shared<const Temp::CTempList>( munchExp( binOp->GetRight() ), nullptr ) ) ) );

	// TODO считаем, что CCondJump может идти только по < и >= (появляется в TraceSchedule)
	std::string oper;
	switch( binOp->GetBinOp() ) {
		case IExpr::LESS: oper = "JL"; break;
		case IExpr::GE: oper = "JGE"; break;
	}
	emit( new Assembler::COper( oper + " 'l0\n", nullptr, nullptr, 
		std::make_shared<const Temp::CLabelList>( stm->GetIfTrueLabel(), nullptr ) ) );
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
	std::shared_ptr<const Temp::CTemp> temp( new Temp::CTemp );
	if( INSTANCEOF( expr->GetMem(), CBinop ) ) {
		// MEM( BINOP(PLUS,e1,CONST(i)), e2 )
		auto binOp = std::dynamic_pointer_cast<const CBinop>( expr->GetMem() );
		if( (binOp->GetBinOp() == IExpr::PLUS || binOp->GetBinOp() == IExpr::MINUS) && 
			( INSTANCEOF( binOp->GetLeft(), CConst) || INSTANCEOF( binOp->GetRight(), CConst ) ) ) 
		{
			IExprPtr binOpExpr;
			CConstPtr constantExpr;
			if( INSTANCEOF( binOp->GetLeft(), CConst ) ) {
				binOpExpr = binOp->GetRight();
				constantExpr = std::dynamic_pointer_cast<const CConst>( binOp->GetLeft() );
			} else {
				binOpExpr = binOp->GetLeft();
				constantExpr = std::dynamic_pointer_cast<const CConst>( binOp->GetRight() );
			}
			emit( new Assembler::COper( std::string( "MEM 'd0, ['s0" ) +
				( ( binOp->GetBinOp() == IExpr::PLUS ) ? "+" : "-" ) +
				std::to_string( constantExpr->GetValue() ) +
				std::string( "]\n" ),
				std::make_shared<const Temp::CTempList>( temp, nullptr ),
				std::make_shared<const Temp::CTempList>( munchExp( binOpExpr ), nullptr ) ) );
			return temp;
		}
	} 
	if( INSTANCEOF( expr->GetMem(), CConst ) ) {
		auto constantExpr = std::dynamic_pointer_cast<const CConst>( expr->GetMem( ) );
		emit( new Assembler::COper( std::string( "MEM 'd0, ['r0+" ) +
			std::to_string( constantExpr->GetValue() ) +
			std::string( "]\n" ),
			std::make_shared<const Temp::CTempList>( temp, nullptr ), 
			nullptr ) );
		return temp;
	} 
	emit( new Assembler::COper( "MEM 'd0, ['s0+0]\n",
		std::make_shared<const Temp::CTempList>( temp, nullptr ),
		std::make_shared<const Temp::CTempList>( munchExp( expr->GetMem() ), nullptr ) ) );
	return temp;
}

static bool isArithmeticOperation( int op )
{
	return op == IExpr::PLUS || op == IExpr::MINUS || op == IExpr::MUL || op == IExpr::DIV;
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CBinopPtr binOp )
{
	std::shared_ptr<const Temp::CTemp> temp( new Temp::CTemp );

	if( isArithmeticOperation( binOp->GetBinOp() ) ) {
		std::string oper;
		switch( binOp->GetBinOp() ) {
			case IExpr::PLUS: oper = "ADD"; break;
			case IExpr::MINUS: oper = "SUB"; break;
			case IExpr::MUL: oper = "MUL"; break;
			case IExpr::DIV: oper = "DIV"; break;
		}

		if( INSTANCEOF( binOp->GetLeft(), CConst ) || INSTANCEOF( binOp->GetRight(), CConst ) ) {
			IExprPtr binOpExpr;
			CConstPtr constantExpr;
			if( INSTANCEOF( binOp->GetLeft(), CConst ) ) {
				binOpExpr = binOp->GetRight();
				constantExpr = std::dynamic_pointer_cast<const CConst>( binOp->GetLeft() );
			} else {
				binOpExpr = binOp->GetLeft();
				constantExpr = std::dynamic_pointer_cast<const CConst>( binOp->GetRight() );
			}
			emit( new Assembler::COper( std::string( oper + " 'd0, 's0+" ) +
				std::to_string( constantExpr->GetValue() ) +
				std::string( "\n" ),
				std::make_shared<const Temp::CTempList>( temp, nullptr ),
				std::make_shared<const Temp::CTempList>( munchExp( binOpExpr ), nullptr ) ) );
		} else {
			emit( new Assembler::COper( oper + " 'd0, 's0+'s1\n",
				std::make_shared<const Temp::CTempList>( temp, nullptr ),
				std::make_shared<const Temp::CTempList>( munchExp( binOp->GetLeft() ),
				std::make_shared<const Temp::CTempList>( munchExp( binOp->GetRight() ), nullptr ) ) ) );
		}
	} else {
		// TODO логические операции
	}
	return temp;
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CConstPtr constantExpr )
{
	std::shared_ptr<const Temp::CTemp> temp( new Temp::CTemp );
	
	emit( new Assembler::COper( std::string( "ADD 'd0, r0+" ) +
		std::to_string( constantExpr->GetValue() ) +
		std::string( "\n" ),
		nullptr,
		std::make_shared<const Temp::CTempList>( temp, nullptr ) ) );

	return temp;
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CTempPtr expr )
{
	return expr->GetTemp();
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CNamePtr expr )
{
	return std::make_shared<const Temp::CTemp>();
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CCallPtr expr )
{
	auto temporaries = munchArgs( expr->GetArguments() );
	emit( new Assembler::COper( "CALL 'l0\n", nullptr, nullptr, 
		std::make_shared<const Temp::CLabelList>( std::make_shared<const Temp::CLabel>( expr->GetFunctionName() ), nullptr ) ) );

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

std::shared_ptr<const Temp::CTempList> CCodeGeneration::munchArgs( std::shared_ptr<const IRTree::CExprList> args )
{
	if( args == nullptr ) {
		return nullptr;
	}
	std::shared_ptr<const Temp::CTempList> temporariesList;
	while( args ) {
		std::shared_ptr<const Temp::CTemp> temp( new Temp::CTemp() );
		emit( new Assembler::CMove( "MOV 'd0, 's0\n", temp, munchExp( args->GetHead() ) ) );

		temporariesList = std::make_shared<const Temp::CTempList>( temp, temporariesList );
		args = args->GetTail();
	}

	return temporariesList;
}
