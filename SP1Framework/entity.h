#pragma once
#include <Windows.h>
class entity
{
private:
	COORD location;
	bool alive;

public:
	// Pure Virtual Function
	virtual void move(void) = 0;

	// Constructors
	entity();
	entity(int x, int y);
	entity(COORD location);

	// Destructor
	~entity();

	// Accessors
	COORD getCoords(void);
	bool getAlive(void);

	// Mutators
	void setCoords(int x, int y);
	void setCoords(COORD location);
	void setAlive(bool alive);
};

