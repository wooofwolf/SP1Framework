#include "Map.h"
#include <iostream>
#include <string>
#include <fstream>
#include "game.h"
Map::Map() {
	for (int row = 0; row < 60; row++) {// int row = 0, when row is lesser than 20 , add one row until 20
		for (int col = 0; col < 60; col++) { // int col = 0 , when col is  lesser than 20, add one more col until 20

			map[row][col] = ' ';//put spaces to make it more square
		}
	}
}
Map::~Map()
{
}

void Map::mapdisplay()
{
    string line;

    ifstream myfile;
    myfile.open("Testing.txt", ifstream::in);

    char c = myfile.get();
    while (myfile.good())
    {
        if (c == '2')
        {
            cout << '-';
        }
        else if (c == '1')
        {
            cout << "1";
        }
        else if (c == '0')
        {
            cout << " ";
        }
        else
        {
            cout << endl;
        }
        c = myfile.get();
    }

    myfile.close();
}
