#include "Play.h"



Play::Play()
{
}

Play::Play(string word,int row, int col,bool horizontal)
{
	size_=word.length();
	tile_=new Tile[size_];
	//tile_ = Tile(letter.GetLetter(), row, column, letter.GetScore(), letter.GetBonus());
	if (horizontal)
	{
		for (int i=0;i<size_;i++)
		{
			tile_[i].SetParams(word[i],row,col+i);
		}
	}
	
	else 
	{
		for (int i=0;i<size_;i++)
		{
			tile_[i].SetParams(word[i],row+i,col);
		}
	}
	
	row_ = row;
	column_ = col;
	isHorizontal=horizontal;
}

//Play::Play(char letter, int row, int column)
//{
//	tile_ = Tile(letter, row, column);
//}

int Play::GetRow()
{
	return row_;
}

int Play::GetColumn()
{
	return column_;
}

char* Play::GetLetters()
{
	char *returnedArr=new char[size_];
	for (int i=0;i<size_;i++)
	{
		returnedArr[i]=tile_[i].GetLetter();
	}
	return returnedArr;
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
