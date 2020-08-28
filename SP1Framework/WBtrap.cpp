#include "WBtrap.h"
#include <math.h>


WBtrap::WBtrap()
{
	setAlive(false); //make the trap dead at first before spawning
}

WBtrap::WBtrap(int x, int y)
{
	setCoords(x, y);//set its coordinates using x,y
}

WBtrap::WBtrap(COORD location)
{
	setCoords(location); //sets its coordinates using other entity locations
}

WBtrap::~WBtrap()
{// intentionally left blank
}



