#pragma once

#include<iostream>
#include<vector>
#include"Tile.h"

using namespace std;

class Play
{
	vector<Tile> tiles_;
	int size_;
	int row_;
	int column_;
	int score_;
	bool isHorizontal;


public:
	Play();
	Play(string word,int row, int column,bool horizontal);
//	Play(char letter, int row, int column);
	int GetRow();
	int GetColumn();
	string GetLetters();
	int GetScore();
	void CalculateScore();
	~Play();
};

