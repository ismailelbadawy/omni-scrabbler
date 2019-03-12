#include "Play.h"



Play::Play()
{
}
Play::Play(string word,int row, int col,bool horizontal)
{
	size_=(int)word.size();
	
	//tile_ = Tile(letter.GetLetter(), row, column, letter.GetScore(), letter.GetBonus());
	if (horizontal)
	{
		for (int i=0;i<size_;i++)
		{
			Tile t;
			t.SetParams(word[i],row,col+i);
			tiles_.push_back(t);
		}
	}
	
	else 
	{
		for (int i=0;i<size_;i++)
		{
			Tile t;
			t.SetParams(word[i],row+i,col);
			tiles_.push_back(t);
		}
	}
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

string Play::GetLetters()
{
	string returnedStr="";
	for (int i=0;i<size_;i++)
	{
	returnedStr+=tiles_.at(i).GetLetter();
	}
	return returnedStr;
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
