#include "Tile.h"

Tile::Tile()
{
}

Tile::Tile(char letter,int row, int column, int score=1, int bonus=1)
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
void Tile::SetLetter(char c)
{
	this->letter_=c;
}
int Tile::GetBonus()
{
	return bonus_;
}

void Tile::SetParams(char letter,int row,int col,int score=1,int bonus=1)
{
	letter_=letter;
	row_=row;
	col=column_;
	score_=score;
	bonus_=bonus;
}

void Tile::CalculateTotalScore()
{
	this->score_ = this->score_ * this->bonus_;
}



Tile::~Tile()
{
}
