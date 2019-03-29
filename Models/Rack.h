#pragma once
#include<iostream>
#include"Tile.h"
class Rack
{
	Tile tiles_[7];
public:
	Rack();
	Rack(Tile tiles[7]);
<<<<<<< HEAD
=======
	Rack(string rack);
>>>>>>> parent of 2b553b7... movegenerator still fails in some cases.
	~Rack();
	void SetTile(char c, int loc);
	char GetLetter(int loc);
};

