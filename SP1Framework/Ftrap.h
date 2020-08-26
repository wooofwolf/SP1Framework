#pragma once
#include "trap.h"
class Ftrap: public trap
{
private:
	static int noOfFTraps;

public:
	// Constructors
	Ftrap();
	Ftrap(int x, int y);
	Ftrap(COORD location);
	// Destructor
	~Ftrap();

	// Accessors
	int getNoOfFTraps(void);
	// Mutators
	// None for now
};

