#include "npc.h"
#include <math.h>

int npc::noOfNPCs = 0;

npc::npc()
{
	setCoords(0, 0);
	setAlive(true);

	colour = 0xA0;
	drenched = false;
	secsOnFire = 0;
	noOfNPCs++;
}

npc::npc(int x, int y)
{
	setCoords(x, y);
	setAlive(true);

	colour = 0xA0;
	drenched = false;
	secsOnFire = 0;
	noOfNPCs++;
}

npc::npc(COORD location)
{
	setCoords(location);
	setAlive(true);

	colour = 0xA0;
	drenched = false;
	secsOnFire = 0;
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

bool npc::getDrenched(void)
{
	return drenched;
}

double npc::getSecsOnFire(void)
{
	return secsOnFire;
}

WORD npc::getCol(void)
{
	return colour;
}

double npc::getFsecsPassed(void)
{
	return fireWatch.getElapsedTime();
}

double npc::getWsecsPassed(void)
{
	return waterWatch.getElapsedTime();
}

void npc::setNoOfNPCs(int number)
{
	noOfNPCs = number;
}

void npc::setDrenched(bool drench)
{
	drenched = drench;
}

void npc::setSecsOnFire(double number)
{
	secsOnFire = number;
}

void npc::setCol(WORD colour)
{
	this->colour = colour;
}

void npc::startFTimer(void)
{
	fireWatch.startTimer();
}

void npc::startWTimer(void)
{
	waterWatch.startTimer();
}
