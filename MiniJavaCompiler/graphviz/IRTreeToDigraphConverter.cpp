#include "IRTreeToDigraphConverter.h"
#include "../IRTree/Expressions.h"
#include "../IRTree/Statements.h"

using namespace std;

namespace IRTree {

void CIRTreeToDigraphConverter::Visit( const CMove* node )
{
	node->GetDestExpr()->Accept( this );
	string destString = lastNodeName;

	node->GetSrcExpr()->Accept( this );
	string srcString = lastNodeName;

	nextNameWithId( "move" );

	treeRepresentation.AddEdge( lastNodeName, destString, "dest" );
	treeRepresentation.AddEdge( lastNodeName, srcString, "src" );
}

void CIRTreeToDigraphConverter::Visit( const CExpr* node )
{
	node->GetExp()->Accept( this );
	string prevString = lastNodeName;

	nextNameWithId( "exp" );
	treeRepresentation.AddEdge( lastNodeName, prevString );
}

void CIRTreeToDigraphConverter::Visit( const CJump* node )
{
	nextNameWithId( "jump" );
	for( auto label : node->GetLabels() ) {
		treeRepresentation.AddEdge( lastNodeName, label->GetName()->GetString(), "to_label" );
	}
}

void CIRTreeToDigraphConverter::Visit( const CCondJump* node )
{
	node->GetRightExpr()->Accept( this );
	string rightString = lastNodeName;
	node->GetLeftExpr()->Accept( this );
	string leftString = lastNodeName;

	nextNameWithId( "CondJump" );
	
	treeRepresentation.AddEdge( lastNodeName, leftString, "left" );
	treeRepresentation.AddEdge( lastNodeName, "binop_Eq", "binop_Eq" );
	treeRepresentation.AddEdge( lastNodeName, rightString, "right" );
	treeRepresentation.AddEdge( lastNodeName, node->GetIfTrueLabel()->GetName()->GetString(), "iftrue" );
	treeRepresentation.AddEdge( lastNodeName, node->GetIfFalseLabel()->GetName()->GetString(), "iffalse" );
}

void CIRTreeToDigraphConverter::Visit( const CSeq* node )
{
	if( node->GetLeftStm() != nullptr ) {
		node->GetLeftStm()->Accept( this );
		string leftString = lastNodeName;
		if( node->GetRightStm() != nullptr ) {
			node->GetRightStm()->Accept( this );
			string rightString = lastNodeName;
			nextNameWithId( "seq" );
			treeRepresentation.AddEdge( lastNodeName, leftString, "left" );
			treeRepresentation.AddEdge( lastNodeName, rightString, "right" );
		} else {
			nextNameWithId( "seq" );
			treeRepresentation.AddEdge( lastNodeName, leftString, "left" );
		}
	} else {
		nextNameWithId( "seq" );
	}
}

void CIRTreeToDigraphConverter::Visit( const CConst* node )
{
	nextNameWithId( string( "const_" ) + to_string( node->GetValue() ) );
}

void CIRTreeToDigraphConverter::Visit( const CName* node )
{
	nextNameWithId( string( "name_" ) + node->GetName()->GetName()->GetString() );
}

void CIRTreeToDigraphConverter::Visit( const CTemp* node )
{
	nextNameWithId( string( "temp_" ) + node->GetTemp()->GetName()->GetString() );
}

void CIRTreeToDigraphConverter::Visit( const CBinop* node )
{
	node->GetLeft()->Accept( this );
	string leftString = lastNodeName;
	node->GetRight()->Accept( this );
	string rightString = lastNodeName;
	//graphviz отказывается работать с символами типа + *
	switch( node->GetBinOp() ) {
	case IRTree::IExpr::MUL:
			nextNameWithId( "binop__Mul" );
			break;
	case IRTree::IExpr::PLUS:
			nextNameWithId( "binop__Plus" );
			break;
	case IRTree::IExpr::DIV:
			nextNameWithId( "binop__Division" );
			break;
	case IRTree::IExpr::MINUS:
			nextNameWithId( "binop__Minus" );
			break;
	case IRTree::IExpr::XOR:
			nextNameWithId( "binop__Xor" );
			break;
	case IRTree::IExpr::LESS:
			nextNameWithId( "binop__Less" );
			break;
	case IRTree::IExpr::GT:
			nextNameWithId( "binop__Greater" );
			break;
	case IRTree::IExpr::AND:
			nextNameWithId( "binop__And" );
			break;
	}
	treeRepresentation.AddEdge( lastNodeName, rightString, "right" );
	treeRepresentation.AddEdge( lastNodeName, leftString, "left" );
}

void CIRTreeToDigraphConverter::Visit( const CMem* node )
{
	node->GetMem()->Accept( this );
	string prevString = lastNodeName;
	nextNameWithId( "mem" );
	treeRepresentation.AddEdge( lastNodeName, prevString );
}

void CIRTreeToDigraphConverter::Visit( const CCall* node )
{
	string funcString = node->GetFunctionName()->GetString();
	string argsString;
	std::vector<string> nodes;
	for( auto arg : node->GetArguments() ) {
		arg->Accept( this );
		nodes.push_back( lastNodeName );
	}
	nextNameWithId( "call" );
	treeRepresentation.AddEdge( lastNodeName, funcString, "func" );
	for( int i = 0; i < nodes.size(); ++i ) {
		treeRepresentation.AddEdge( lastNodeName, nodes[i], "arg" );
	}
	//treeRepresentation.AddEdge( lastNodeName, argsString, "args" );
/*
	string funcString = node->GetFunctionName()->GetString();
	string argsString;
	for( auto arg : node->GetArguments() ) {
		arg->Accept( this );
		argsString += " " + lastNodeName;
	}
	nextNameWithId( "call" );
	treeRepresentation.AddEdge( lastNodeName, funcString, "func" );
	treeRepresentation.AddEdge( lastNodeName, argsString, "args" );*/
}

void CIRTreeToDigraphConverter::Visit( const CESeq* node )
{
	node->GetStatement()->Accept( this );
	string stmString = lastNodeName;
	node->GetExpression()->Accept( this );
	string expString = lastNodeName;
	nextNameWithId( "eseq" );
	treeRepresentation.AddEdge( lastNodeName, expString, "exp" );
	treeRepresentation.AddEdge( lastNodeName, stmString, "stm" );
}

void CIRTreeToDigraphConverter::Visit( const CLabel* node )
{
	nextNameWithId( string( "label:" ) + node->GetLabel()->GetName()->GetString() );
}

void CIRTreeToDigraphConverter::nextNameWithId( std::string label )
{
	lastNodeName = label + string( "__id_" ) + to_string( minId++ );
	treeRepresentation.SetNodeLabel( lastNodeName, label );
}

void CIRTreeToDigraphConverter::LinkedVisit( const IStm* node )
{
	string fromName = lastNodeName;
	node->Accept( this );
	string toName = lastNodeName;
	if( !fromName.empty() ) {
		treeRepresentation.AddEdge( fromName, toName, "next" );
	}
}

void CIRTreeToDigraphConverter::LinkedVisit( const IExpr* node )
{
	string fromName = lastNodeName;
	node->Accept( this );
	string toName = lastNodeName;
	if( !fromName.empty() ) {
		treeRepresentation.AddEdge( fromName, toName, "next" );
	}
}

} //namespace IRTree 