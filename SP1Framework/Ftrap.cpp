#include "Ftrap.h"

int Ftrap::noOfFTraps = 0;

Ftrap::Ftrap()
{
	setAlive(false);
	noOfFTraps++;
}

Ftrap::Ftrap(int x, int y)
{
	setAlive(false);
	setCoords(x, y);
	noOfFTraps++;
}

Ftrap::Ftrap(COORD location)
{
	setAlive(false);
	setCoords(location);
	noOfFTraps++;
}

Ftrap::~Ftrap()
{
	noOfFTraps--;
}

int Ftrap::getNoOfFTraps(void)
{
	return noOfFTraps;
}

void Ftrap::startFTWatch(void)
{
	FTwatch.startTimer();
}

double Ftrap::getFTSecsPassed(void)
{
	return FTwatch.getElapsedTime();
}
