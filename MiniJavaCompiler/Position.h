#pragma once

#include <utility>

#include "BuildOutputs/parser.tab.hpp"

struct CPosition {
public:
	CPosition( int _beginLine, int _beginColumn, int _endLine, int _endColumn ) :
		beginLine( _beginLine ),
		beginColumn( _beginColumn ),
		endLine( _endLine ),
		endColumn( _endColumn )
	{
	}

	CPosition( YYLTYPE yylloc ) :
		beginLine( yylloc.first_line ),
		beginColumn( yylloc.first_column ),
		endLine( yylloc.last_line ),
		endColumn( yylloc.last_column )
	{
	}

	std::pair< int, int > GetBeginPos() const
	{
		return std::make_pair( beginLine, beginColumn );
	}
	std::pair< int, int > GetEndPos() const
	{
		return std::make_pair( endLine, endColumn );
	}

private:
	int beginLine;
	int beginColumn;
	int endLine;
	int endColumn;
};