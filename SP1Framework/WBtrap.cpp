#include "WBtrap.h"
#include <math.h>

int WBtrap::noOfWBTraps = 0;

WBtrap::WBtrap()
{
	
}

WBtrap::WBtrap(int x, int y)
{
	setCoords(x, y);
}

WBtrap::WBtrap(COORD location)
{
	setCoords(location);
}

WBtrap::~WBtrap()
{
}

int WBtrap::getNoOfWBtraps(void)
{
	return noOfWBTraps;
}


