#pragma once
class Tile
{
	char letter_;
	int row_;
	int column_;
	int score_;
	int bonus_;
public:
	Tile();
	Tile(char letter,int row,int column,int score = 1,int bonus = 1);
	void GetIndex(int &row, int&column);
	int GetScore();
	char GetLetter();
	int GetBonus();
	void CalculateTotalScore();
	~Tile();
};

