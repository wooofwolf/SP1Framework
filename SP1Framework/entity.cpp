#include "entity.h"

entity::entity()
{
	// Default Location
	location.X = 0;
	location.Y = 0;
	
	alive = true;
}

entity::entity(int x, int y)
{
	location.X = x;
	location.Y = y;

	alive = true;
}

entity::entity(COORD location)
{
	this->location = location;
}

entity::~entity()
{
	// Left Blank
}

COORD entity::getCoords(void)
{
	return location;
}

bool entity::getAlive(void)
{
	return alive;
}

void entity::setCoords(int x, int y)
{
	location.X = x;
	location.Y = y;
}

void entity::setCoords(COORD location)
{
	this->location = location;
}

void entity::setAlive(bool alive)
{
	this->alive = alive;
}
