#pragma once
#include<iostream>
#include<vector>
#include"Tile.h"


using namespace std;
class Rack
{
public:
	vector<Tile> tiles_;
	Rack();
	Rack(vector<Tile> tiles);
	Rack(string rack);
	int GetLength();
	~Rack();
	void SetTile(char c, int loc);
	char GetLetter(int loc);
	void generateRanRack(Rack rack);
};

