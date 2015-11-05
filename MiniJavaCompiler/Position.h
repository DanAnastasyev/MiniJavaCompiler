#pragma once

#include <utility>

struct CPosition {
public:
	CPosition( int _beginLine, int _beginColumn, int _endLine, int _endColumn ) :
		beginLine( _beginLine ),
		beginColumn( _beginColumn ),
		endLine( _endLine ),
		endColumn( _endColumn )
	{}

	std::pair<int, int> GetBeginPos() const
	{
		return std::make_pair( beginLine, beginColumn );
	}
	std::pair<int, int> GetEndPos() const
	{
		return std::make_pair( endLine, endColumn );
	}

	std::string ToString() const
	{
		return "(" + std::to_string( beginLine ) + " " + std::to_string( beginColumn ) +
			" " + std::to_string( endLine ) + " " + std::to_string( endColumn ) + ")";
	}

private:
	int beginLine;
	int beginColumn;
	int endLine;
	int endColumn;
};