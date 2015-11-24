#pragma once

#include <string>

class CLabel {
public:
	CLabel();
	explicit CLabel( ... );

	const std::string& Name() const
	{
		return name;
	}

private:
	static int nextUniqueId;
	std::string name;
};