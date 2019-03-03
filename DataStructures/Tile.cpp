#include "Tile.h"

Tile::Tile()
{
}

Tile::Tile(char letter,int row, int column, int score, int bonus)
{
	this->letter_ = letter;
	this->score_ = score;
	this->bonus_ = bonus;
	this->row_ = row;
	this->column_ = column;
}

void Tile::GetIndex(int & row, int & column)
{
	row = row_;
	column = column_;
}

int Tile::GetScore()
{
	return score_;
}

char Tile::GetLetter()
{
	return letter_;
}

int Tile::GetBonus()
{
	return bonus_;
}

void Tile::CalculateTotalScore()
{
	this->score_ = this->score_ * this->bonus_;
}



Tile::~Tile()
{
}
