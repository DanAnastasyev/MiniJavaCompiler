#pragma once

#include <string>

class CSymbol;

class CTemp {
public:
	CTemp();
	explicit CTemp( const CSymbol* symbol );
	~CTemp();

	const std::string& Name() const
	{
		return name;
	}

private:
	static int nextUniqueId;
	std::string name;
};
