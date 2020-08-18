#include "npc.h"
#include "fireboy.h"
#include <math.h>

int npc::noOfNPCs = 0;

npc::npc()
{
	setCoords(0, 0);
	setAlive(true);

	SecondsOnFire = 0;
	noOfNPCs++;
}

npc::npc(int x, int y)
{
	setCoords(x, y);
	setAlive(true);

	SecondsOnFire = 0;
	noOfNPCs++;
}

npc::npc(COORD location)
{
	setCoords(location);
	setAlive(true);

	SecondsOnFire = 0;
	noOfNPCs++;
}

npc::~npc()
{
	noOfNPCs--;
}

int npc::getNoOfNPCs(void)
{
	return noOfNPCs;
}

int npc::getSecondsOnFire(void)
{
	return SecondsOnFire;
}

void npc::setNoOfNPCs(int number)
{
	noOfNPCs = number;
}

void npc::setSecondsOnFire(int number)
{
	SecondsOnFire = number;
}

void npc::move(void)
{
	// NPC movement in game.cpp
}
