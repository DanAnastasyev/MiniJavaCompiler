#pragma once

#include "Position.h"

class CPositionStorage {
public:
	CPositionStorage( CPosition pos ) :
		position( pos )
	{
	}

	CPosition GetPosition() const
	{
		return position;
	}

protected:
	CPosition& position;
};