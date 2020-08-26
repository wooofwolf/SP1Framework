#pragma once
#include "entity.h"
class trap: public entity
{
private:
	// None I can think of yet

public:
	// Constructors
	trap();
	trap(int x, int y);
	trap(COORD location);
	// Destructor
	~trap();

	// Accessors
	// Nth yet
	// Mutators
	void move(void);
};

