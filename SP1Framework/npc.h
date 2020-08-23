#pragma once
#include "entity.h"
class npc: public entity
{
private:
	static int noOfNPCs;
	bool drenched;
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
	bool getDrenched(void);
	double getSecsOnFire(void);
	WORD getCol(void);

	// Mutators
	void setNoOfNPCs(int number);
	void setDrenched(bool drench);
	void setSecsOnFire(double number);
	void setCol(WORD colour);
	void move(void);
};

