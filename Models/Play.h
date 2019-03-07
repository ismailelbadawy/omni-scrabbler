#pragma once
#include<iostream>
#include"Tile.h"
using namespace std;
class Play
{
	Tile* tile_;
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
	char* GetLetters();
	int GetScore();
	void CalculateScore();
	~Play();
};

