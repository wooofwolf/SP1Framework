#pragma once
#include "entity.h"
class npc: public entity
{
private:
	static int noOfNPCs;
	double secsOnFire;
	WORD colour;

public:
	// Constructors
	npc();
	npc(int x, int y);
	npc(COORD location);
	
	// Destructor
	~npc();

	// Accessors
	int getNoOfNPCs(void);
	double getSecsOnFire(void);
	WORD getCol(void);

	// Mutators
	void setNoOfNPCs(int number);
	void setSecsOnFire(double number);
	void setCol(WORD colour);
	void move(void);
};

