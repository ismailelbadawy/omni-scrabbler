#include "Play.h"



Play::Play()
{
}

Play::Play(Tile letter, int row, int column)
{
	tile_ = Tile(letter.GetLetter(), row, column, letter.GetScore(), letter.GetBonus());
	row_ = row;
	column_ = column;
}

Play::Play(char letter, int row, int column)
{
	tile_ = Tile(letter, row, column);
}

int Play::GetRow()
{
	return row_;
}

int Play::GetColumn()
{
	return column_;
}

char Play::GetLetter()
{
	return tile_.GetLetter();
}

int Play::GetScore()
{
	return score_;
}

void Play::CalculateScore()
{
	// To be implemented later
}


Play::~Play()
{
}
