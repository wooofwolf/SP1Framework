#pragma once
#include "trap.h"
class Ftrap: public trap
{
private:
	static int noOfFTraps;
	CStopWatch FTwatch;

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
	void startFTWatch(void);
	double getFTSecsPassed(void);
};

