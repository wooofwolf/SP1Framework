#pragma once
#include "entity.h"
class npc: public entity
{
private:
	static int noOfNPCs;
	int SecondsOnFire;

public:
	// Constructors
	npc();
	npc(int x, int y);
	npc(COORD location);
	
	// Destructor
	~npc();

	// Accessors
	int getNoOfNPCs(void);
	int getSecondsOnFire(void);

	// Mutators
	void setNoOfNPCs(int number);
	void setSecondsOnFire(int number);
	void move(void);
};

