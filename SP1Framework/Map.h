#pragma once
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
class Map
{
private:
	char map[60][60];
public:

	string line;
	ifstream myfile;
	Map();//constructor 
	~Map();//destructor 
	void mapdisplay();//map display
};

