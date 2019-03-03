#pragma once
#include<iostream>
#include"Tile.h"
using namespace std;
class Play
{
	Tile tile_;
	int row_;
	int column_;
	int score_;


public:
	Play();
	Play(Tile letter, int row, int column);
	Play(char letter, int row, int column);
	int GetRow();
	int GetColumn();
	char GetLetter();
	int GetScore();
	void CalculateScore();
	~Play();
};

