#pragma once
#include<iostream>
#include<vector>
#include"Tile.h"
using namespace std;

class Board
{
	Tile tiles_[15][15];
	int count_;
public:
	Board();
	Board(char tiles[15][15]);
	void GetTiles(Tile* tiles[15][15]);
	int GetCount();
	bool IsPositionEmpty(int row, int column);
	void UpdateBoard(string word, int row, int column, bool horizontal);
	void Probe(char c,int row,int col, int score);
	~Board();
};

