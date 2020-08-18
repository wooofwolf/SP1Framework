#pragma once
#include "entity.h"
class npc: public entity
{
private:
	static int noOfNPCs;
	int turnsOnFire;

public:
	// Constructors
	npc();
	npc(int x, int y);
	npc(COORD location);
	
	// Destructor
	~npc();

	// Accessors
	int getNoOfNPCs(void);
	int getTurnsOnFire(void);

	// Mutators
	void setNoOfNPCs(int number);
	void setTurnsOnFire(int number);
	void move(void);
};

