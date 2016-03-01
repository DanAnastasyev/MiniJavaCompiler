#include "Visitors/IRBuilderVisitor.h"
#include "RuleClasses/RuleClasses.h"

CIRBuilderVisitor::CIRBuilderVisitor( SymbolsTable::CTable* _table )
{
	symbolsTable = _table;
}

void CIRBuilderVisitor::Visit( const CProgram* program )
{
	if( program->GetMainClass() != nullptr ) {
		program->GetMainClass()->Accept( this );
	}
	if( program->GetClassDeclList() != nullptr ) {
		program->GetClassDeclList()->Accept( this );
	}
}

void CIRBuilderVisitor::Visit( const CMainClass* mainClass )
{
	curClass = symbolsTable->GetClass( mainClass->GetClassName()->GetString() );
	curMethod = curClass->GetMethod( "main" );

	frames.emplace_back( curClass, curMethod, symbolsTable );

	std::shared_ptr<const IRTree::IStm> statements = nullptr;
	if( mainClass->GetStatement() != nullptr ) {
		mainClass->GetStatement()->Accept( this );
		statements = parsedStatements.top();
		parsedStatements.pop();
	}

	frames.back().SetStatements( statements );
}

void CIRBuilderVisitor::Visit( const CClassDeclList* classDeclList )
{
	if( classDeclList->GetClassDecl() != nullptr ) {
		classDeclList->GetClassDecl()->Accept( this );
	}
	if( classDeclList->GetClassDeclList() != nullptr ) {
		classDeclList->GetClassDeclList()->Accept( this );
	}
}

void CIRBuilderVisitor::Visit( const CClassDecl* classDecl )
{
	curClass = symbolsTable->GetClass( classDecl->GetName()->GetString() );

	if( classDecl->GetMethodDeclList() != nullptr ) {
		classDecl->GetMethodDeclList()->Accept( this );
	}
	curClass = nullptr;
}

void CIRBuilderVisitor::Visit( const CClassDeclDerived* classDeclDerived )
{
	curClass = symbolsTable->GetClass( classDeclDerived->GetName()->GetString() );

	if( classDeclDerived->GetMethodDeclList() != nullptr ) {
		classDeclDerived->GetMethodDeclList()->Accept( this );
	}
}

void CIRBuilderVisitor::Visit( const CVarDecl* varDecl )
{}

void CIRBuilderVisitor::Visit( const CVarDeclList* varDeclList )
{}

void CIRBuilderVisitor::Visit( const CMethodDecl* methodDecl )
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
	std::shared_ptr<const IRTree::IExpr> returnExp = parsedExpressions.top();
	parsedExpressions.pop();

	std::shared_ptr<const IRTree::IStm> moveResult =
		std::shared_ptr<const IRTree::IStm>( new IRTree::CMove( std::shared_ptr<const IRTree::IExpr>( 
		new IRTree::CTemp( frames.back().GetReturnPtr() ) ), returnExp ) );

	std::shared_ptr<const IRTree::IStm> methodStm = moveResult;

	if( statements != nullptr ) {
		methodStm = std::shared_ptr<const IRTree::IStm>( new IRTree::CSeq( statements, moveResult ) );
	}

	frames.back().SetStatements( methodStm );
}

void CIRBuilderVisitor::Visit( const CStandardType* type )
{}

void CIRBuilderVisitor::Visit( const CUserType* type )
{}

void CIRBuilderVisitor::Visit( const CStatementListStatement* statement )
{
	if( statement->GetStatementList() != nullptr ) {
		statement->GetStatementList()->Accept( this );
	}
}

void CIRBuilderVisitor::Visit( const CArrayAssignStatement* statement )
{
	// left[i] = right
	// left - переменная на фрейме
	IRTree::CExprPtr left( frames.back().GetVar( statement->GetArrayName() )->GetExp( frames.back().GetFramePtr() ) );
	
	// i - индекс в массиве. Вычисляем offset
	statement->GetElementNumber()->Accept( this );
	IRTree::CExprPtr index = parsedExpressions.top( );
	parsedExpressions.pop();
	IRTree::CExprPtr offset( new IRTree::CBinop( IRTree::IExpr::MUL, index, 
		IRTree::CExprPtr( new IRTree::CConst( Frame::CFrame::WORD_SIZE ) ) ) );
	
	// left = MEM( +( MEM(left), BINOP(MUL, i, CONST(W)) ) )
	left = IRTree::CExprPtr( new IRTree::CMem( IRTree::CExprPtr(
		IRTree::CExprPtr( new IRTree::CBinop( IRTree::CBinop::PLUS, IRTree::CExprPtr( new IRTree::CMem( left ) ), offset ) ) ) ) );

	statement->GetRightPart()->Accept( this );
	IRTree::CExprPtr right = parsedExpressions.top();
	parsedExpressions.pop();

	parsedStatements.emplace( new IRTree::CMove( left, right ) );
}

void CIRBuilderVisitor::Visit( const CAssignStatement* statement )
{
	// left = right
	// left - переменная на фрейме
	IRTree::CExprPtr left( frames.back( ).GetVar( statement->GetLeftPart( ) )->GetExp( frames.back( ).GetFramePtr( ) ) );

	// Парсим right
	statement->GetRightPart()->Accept( this );
	IRTree::CExprPtr right = parsedExpressions.top( );
	parsedExpressions.pop();

	parsedStatements.emplace( new IRTree::CMove( left, right ) );
}

void CIRBuilderVisitor::Visit( const CIfStatement* ifStatement )
{
	ifStatement->GetCondition()->Accept( this );
	IRTree::CExprPtr condition = parsedExpressions.top();
	parsedExpressions.pop();

	// Создаем метки для true, false и выхода из if
	// Создаем метки со сгенерированным названием	
	std::shared_ptr<const Temp::CLabel> trueLabel(new Temp::CLabel());
	std::shared_ptr<const Temp::CLabel> falseLabel(new Temp::CLabel());
	std::shared_ptr<const Temp::CLabel> joinLabel(new Temp::CLabel());

	IRTree::CStmPtr trueStatement, falseStatement;
	if( ifStatement->GetIfTrueStatement() != nullptr ) {
		ifStatement->GetIfTrueStatement()->Accept( this );
		trueStatement = parsedStatements.top();
		parsedStatements.pop();
	}
	if( ifStatement->GetIfFalseStatement() != nullptr ) {
		ifStatement->GetIfFalseStatement()->Accept( this );
		falseStatement = parsedStatements.top();
		parsedStatements.pop();
	}

	IRTree::CStmPtr endifJump = IRTree::CStmPtr(new IRTree::CJump(joinLabel));
	falseStatement = IRTree::CStmPtr(new IRTree::CSeq(IRTree::CStmPtr(new IRTree::CLabel(falseLabel)), IRTree::CStmPtr(new IRTree::CSeq(falseStatement, endifJump))));
	trueStatement = IRTree::CStmPtr( new IRTree::CSeq( IRTree::CStmPtr( new IRTree::CLabel( trueLabel ) ), IRTree::CStmPtr( new IRTree::CSeq( trueStatement, endifJump ) ) ) );
	IRTree::CStmPtr bothStatement;

	if (ifStatement->GetIfFalseStatement() != nullptr) {
		bothStatement = IRTree::CStmPtr(new IRTree::CSeq(trueStatement, IRTree::CStmPtr(new IRTree::CSeq(falseStatement, IRTree::CStmPtr(new IRTree::CLabel(joinLabel))))));
	} else {
		// TODO: check this
		bothStatement = IRTree::CStmPtr(new IRTree::CSeq(trueStatement, IRTree::CStmPtr(new IRTree::CLabel(joinLabel))));
	}

	parsedStatements.push( IRTree::CStmPtr( new IRTree::CSeq( IRTree::CStmPtr( new IRTree::CCondJump( IRTree::IExpr::EQ, condition, 
		IRTree::CExprPtr( new IRTree::CConst(1) ), trueLabel, falseLabel ) ), bothStatement) ) );
}

void CIRBuilderVisitor::Visit( const CWhileStatement* whileStatement )
{
	whileStatement->GetBodyCycle()->Accept( this );
	IRTree::CStmPtr bodyCycleStatement = parsedStatements.top();
	parsedStatements.pop();

	whileStatement->GetCondition()->Accept( this );
	IRTree::CExprPtr condition = parsedExpressions.top();
	parsedExpressions.pop();

	std::shared_ptr<const Temp::CLabel> testLabel( new Temp::CLabel() );
	std::shared_ptr<const Temp::CLabel> nextStepLabel( new Temp::CLabel( ) );
	std::shared_ptr<const Temp::CLabel> doneLabel( new Temp::CLabel() );

	// SEQ( SEQ( LABEL(nextStepLabel), body), JUMP(test) )
	bodyCycleStatement = IRTree::CStmPtr( new IRTree::CSeq( IRTree::CStmPtr( 
		new IRTree::CSeq( IRTree::CStmPtr( new IRTree::CLabel(nextStepLabel) ), bodyCycleStatement ) ),
		IRTree::CStmPtr( new IRTree::CJump( testLabel ) ) ) );

	// SEQ( CCONDJUMP(eq, condition, 1, nextStepLabel, doneLabel), bodyCycleStatement )
	IRTree::CStmPtr conditionStatement = IRTree::CStmPtr( new IRTree::CSeq( IRTree::CStmPtr(
		new IRTree::CCondJump( IRTree::IExpr::EQ, condition, IRTree::CExprPtr( new IRTree::CConst( 1 ) ), nextStepLabel, doneLabel ) ), 
		bodyCycleStatement ) );

	// SEQ( SEQ( LABEL(testLabel, condition) ), doneLabel )
	parsedStatements.push( IRTree::CStmPtr( new IRTree::CSeq( IRTree::CStmPtr( new IRTree::CSeq(
		IRTree::CStmPtr( new IRTree::CLabel( testLabel ) ), conditionStatement ) ),
		IRTree::CStmPtr( new IRTree::CLabel( doneLabel ) ) ) ) );
}

void CIRBuilderVisitor::Visit( const CPrintStatement* printStatement )
{
	printStatement->GetExpression()->Accept( this );
	IRTree::CExprPtr expr = parsedExpressions.top();
	parsedExpressions.pop();
	parsedStatements.emplace( new IRTree::CExpr( IRTree::CExprPtr( new IRTree::CCall( CSymbol::GetSymbol( "name_println" ), { expr } ) ) ));
}

void CIRBuilderVisitor::Visit( const CBinOpExpression* expr )
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

	parsedExpressions.emplace( IRTree::CExprPtr( binOp ) );
}

void CIRBuilderVisitor::Visit( const CIndexExpression* expr )
{
	expr->GetExp()->Accept( this );
	IRTree::CExprPtr array = parsedExpressions.top();
	parsedExpressions.pop();

	expr->GetIndexExp()->Accept( this );
	IRTree::CExprPtr index = parsedExpressions.top();
	parsedExpressions.pop();
	// index смещен на 1, т.к. в первой ячейке лежит длина массива
	index = IRTree::CExprPtr( new IRTree::CBinop( IRTree::IExpr::PLUS, index, IRTree::CExprPtr( new IRTree::CConst( 1 ) ) ) );

	IRTree::CExprPtr offset( new IRTree::CBinop( IRTree::IExpr::MUL, index, IRTree::CExprPtr( new IRTree::CConst( Frame::CFrame::WORD_SIZE ) ) ) );

	parsedExpressions.push( IRTree::CExprPtr( new IRTree::CMem( IRTree::CExprPtr( new IRTree::CBinop( IRTree::IExpr::PLUS, array, offset ) ) ) ) );
}

void CIRBuilderVisitor::Visit( const CLenghtExpression* expr )
{
	expr->GetExp()->Accept( this );
	IRTree::CExprPtr arrayLen = parsedExpressions.top();
	parsedExpressions.pop();

	IRTree::CExprPtr lenght( new IRTree::CTemp( std::shared_ptr<Temp::CTemp>( new Temp::CTemp() ) ) );

	parsedExpressions.push( IRTree::CExprPtr( new IRTree::CESeq( IRTree::CStmPtr( new IRTree::CMove( lenght, arrayLen ) ), lenght ) ) );
}

void CIRBuilderVisitor::Visit( const CMethodExpression* expr )
{
	expr->GetExp()->Accept( this );
	IRTree::CExprPtr object = parsedExpressions.top();
	parsedExpressions.pop();

	std::vector<IRTree::CExprPtr> arguments;
	arguments.push_back(object);
	if( expr->GetArgumentList() != nullptr ) {
		int expresionsStackSize = parsedExpressions.size();
		expr->GetArgumentList()->Accept( this );
		int newExpressionsStackSize = parsedExpressions.size();
		for( ; expresionsStackSize < newExpressionsStackSize; ++expresionsStackSize ) {
			arguments.push_back( parsedExpressions.top() );
			parsedExpressions.pop();
		}
	}

	IRTree::CExprPtr returnValue( new IRTree::CTemp( std::shared_ptr<Temp::CTemp>( frames.back().GetReturnPtr() ) ) );

	parsedExpressions.push( IRTree::CExprPtr( new IRTree::CESeq( IRTree::CStmPtr( new IRTree::CMove(
		returnValue, IRTree::CExprPtr( new IRTree::CCall( expr->GetIdentifier(), arguments ) ) ) ), returnValue ) ) );
}

void CIRBuilderVisitor::Visit( const CIntLiteralExpression* expr )
{
	parsedExpressions.emplace( IRTree::CExprPtr( new IRTree::CConst( expr->GetValue() ) ) );
}

void CIRBuilderVisitor::Visit( const CBoolLiteralExpression* expr )
{
	parsedExpressions.emplace( IRTree::CExprPtr( new IRTree::CConst( expr->GetValue() ) ) );
}

void CIRBuilderVisitor::Visit( const CIdentifierExpression* expr )
{
	parsedExpressions.emplace( frames.back().GetVar( expr->GetIdentifier() )->GetExp( frames.back().GetFramePtr() ) );
}

void CIRBuilderVisitor::Visit( const CThisExpression* expr )
{
	parsedExpressions.emplace( new IRTree::CTemp( frames.back().GetThisPtr() ) );
}

void CIRBuilderVisitor::Visit( const CNewIntArrayExpression* expr )
{
	expr->GetExp()->Accept( this );
	IRTree::CExprPtr arraySize = parsedExpressions.top();
	parsedStatements.pop();

	// (len + 1) * wordSize
	IRTree::CExprPtr allocationSize( new IRTree::CMem( IRTree::CExprPtr( 
		new IRTree::CBinop( IRTree::IExpr::MUL,
		IRTree::CExprPtr( new IRTree::CBinop( IRTree::IExpr::PLUS, arraySize, IRTree::CExprPtr( new IRTree::CConst( 1 ) ) ) ),
		IRTree::CExprPtr( new IRTree::CConst(frames.back().WORD_SIZE) ) ) ) ) );

	IRTree::CExprPtr temp( new IRTree::CTemp( std::shared_ptr<Temp::CTemp>( new Temp::CTemp() ) ) );

	IRTree::CExprPtr mallocCall( new IRTree::CCall( CSymbol::GetSymbol( "__malloc" ), { allocationSize } ) );
	IRTree::CStmPtr allocateMemory( new IRTree::CMove( temp, mallocCall ) );
	// TODO: как сделать по-нормальному
	IRTree::CStmPtr clearMemory( new IRTree::CExpr( IRTree::CExprPtr( new IRTree::CCall( CSymbol::GetSymbol( "__memset" ),
		{ IRTree::CExprPtr( new IRTree::CConst( 0 ) ), allocationSize } ) ) ) );
	IRTree::CStmPtr moveSize( new IRTree::CMove( temp, arraySize ) );

	parsedExpressions.push( IRTree::CExprPtr( new IRTree::CESeq( IRTree::CStmPtr( new IRTree::CSeq(
		allocateMemory, IRTree::CStmPtr( new IRTree::CSeq( clearMemory, moveSize ) ) ) ), temp ) ) );
}

void CIRBuilderVisitor::Visit( const CNewExpression* expr )
{
	SymbolsTable::CClassInfo* object = symbolsTable->GetClass( expr->GetIdentifier()->GetString() );
	int objectSize = object->GerVars().size();

	IRTree::CExprPtr allocationSize(new IRTree::CConst(objectSize * frames.back().WORD_SIZE));

	IRTree::CExprPtr temp( new IRTree::CTemp( std::shared_ptr<Temp::CTemp>( new Temp::CTemp() ) ) );
	IRTree::CExprPtr mallocCall( new IRTree::CCall( CSymbol::GetSymbol( "__malloc" ), { allocationSize } ) );
	IRTree::CStmPtr allocateMemory( new IRTree::CMove( temp, mallocCall ) );
	// TODO: как сделать по-нормальному
	IRTree::CStmPtr clearMemory( new IRTree::CExpr( IRTree::CExprPtr( new IRTree::CCall( CSymbol::GetSymbol( "__memset" ),
		{ temp, IRTree::CExprPtr(new IRTree::CConst(0)) } ) ) ) );

	parsedExpressions.push( IRTree::CExprPtr( new IRTree::CESeq( IRTree::CStmPtr( new IRTree::CSeq(
		allocateMemory, clearMemory ) ), temp ) ) );
}

void CIRBuilderVisitor::Visit( const CUnaryOpExpression* expr )
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

void CIRBuilderVisitor::Visit( const CBracesExpression* expr )
{
	if( expr->GetExp() != nullptr ) {
		expr->GetExp()->Accept( this );
	}
}

void CIRBuilderVisitor::Visit( const CExpressionList* exprList )
{
	// Просто укладываем всё на стек последовательно
	if( exprList->GetExp() != nullptr ) {
		exprList->GetExp()->Accept( this );
	}
	if( exprList->GetExpList() != nullptr ) {
		exprList->GetExpList()->Accept( this );
	}
}

void CIRBuilderVisitor::Visit( const CExpressionRest* expr )
{
	if( expr->GetExp() != nullptr ) {
		expr->GetExp()->Accept( this );
	}
}

void CIRBuilderVisitor::Visit( const CStatementList* stmtList )
{
	int parsedStatementsCount = parsedStatements.size();
	stmtList->GetStatement()->Accept( this );

	// Если распарсили не statement, а expression
	if( parsedStatementsCount + 1 != parsedStatements.size() ) {
		IRTree::CExprPtr exp = parsedExpressions.top();
		parsedExpressions.pop();

		parsedStatements.emplace( new IRTree::CExpr( exp ) );
	}

	// Объединяем в SEQ последовательность, если есть еще элементы листа
	if( stmtList->GetStatementList() != nullptr ) {
		stmtList->GetStatementList( )->Accept( this );

		IRTree::CStmPtr parsedListRest = parsedStatements.top();
		parsedStatements.pop();

		// Statement, который прошли в stmtList->GetStatement()
		IRTree::CStmPtr parsedStm = parsedStatements.top( );
		parsedStatements.pop();

		parsedStatements.emplace( new IRTree::CSeq( parsedStm, parsedListRest ) );
	}
}

void CIRBuilderVisitor::Visit( const CMethodDeclList* methodList )
{
	if( methodList->GetMethodDecl() != nullptr ) {
		methodList->GetMethodDecl()->Accept( this );
	}
	if( methodList->GetMethodDeclList() != nullptr ) {
		methodList->GetMethodDeclList()->Accept( this );
	}
}

void CIRBuilderVisitor::Visit( const CFormalList* list )
{
}

void CIRBuilderVisitor::Visit( const CFormalParam* formal )
{
}

std::vector<Frame::CFrame> CIRBuilderVisitor::GetFrames() const
{
	return frames;
}
