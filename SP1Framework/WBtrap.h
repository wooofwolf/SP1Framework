#pragma once
#include "Framework/timer.h"
#include "entity.h"
class WBtrap :public entity
{
public:
	// Constructors
	WBtrap();
	WBtrap(int x, int y);
	WBtrap(COORD location);

	// Destructor
	~WBtrap();
	// Accessors
	int getNoOfWBtraps(void);
};

