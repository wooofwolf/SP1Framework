#pragma once
#include <Windows.h>
#include "Framework/timer.h"
class entity
{
private:
	COORD location;
	bool alive;

public:
	// Constructors
	entity();
	entity(int x, int y);
	entity(COORD location);

	// Destructor
	~entity();

	// Accessors
	COORD getCoords(void);
	bool getAlive(void);

	// Mutators
	void setCoords(int x, int y);
	void setCoords(COORD location);
	void setAlive(bool alive);
};

