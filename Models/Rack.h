#pragma once
#include<iostream>
#include"Tile.h"
class Rack
{
	Tile tiles_[7];
public:
	Rack();
	Rack(Tile tiles[7]);
	~Rack();
	void SetTile(char c, int loc);
	char GetLetter(int loc);
};

