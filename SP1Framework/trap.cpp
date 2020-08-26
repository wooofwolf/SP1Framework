#include "trap.h"

trap::trap()
{
	setAlive(false);
	setCoords(0, 0);
}

trap::trap(int x, int y)
{
	setAlive(false);
	setCoords(x, y);
}

trap::trap(COORD location)
{
	setAlive(false);
	setCoords(location);
}

trap::~trap()
{
	// Leave Blank for now
}
