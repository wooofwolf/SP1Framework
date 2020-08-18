#pragma once
#include "entity.h"
class npc: public entity
{
private:
	static int noOfNPCs;
	int secsOnFire;

public:
	// Constructors
	npc();
	npc(int x, int y);
	npc(COORD location);
	
	// Destructor
	~npc();

	// Accessors
	int getNoOfNPCs(void);
	int getSecsOnFire(void);

	// Mutators
	void setNoOfNPCs(int number);
	void setSecsOnFire(int number);
	void move(void);
};

