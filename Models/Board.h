#pragma once
#include<iostream>
#include<vector>
#include"Play.h"
#include"Tile.h"
using namespace std;

class Board
{
	Tile tiles_[15][15];
public:
	Board();
	Board(char tiles[15][15]);
	void GetTiles(Tile tiles[15][15]);
	void UpdateBoard(string word, int row, int column, bool horizontal);
	void Probe(char c,int row,int col);
	~Board();
};

