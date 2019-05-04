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
	int GetPosition(char letter);
	~Rack();
	void SetTile(char c, int loc);
	char GetLetter(int loc);
	void generateRanRack(Rack rack);
	vector <Tile> GetRackTiles();
	void SetRackTiles(vector<Tile> Tiles);
	void RemoveTile(Tile tile);
	void RemoveTile(char letter);
	Tile RemoveAndReturnTile(char letter);
	string RackToString();
	Tile GetTile(int pos);
};

