#include "Move.h"



Move::Move()
{
}



Move::Move(Board &board, bool horizontal)
{
	string word;
	ishorizontal_ = horizontal;
	for (int i = 0; i < (int)plays_.size(); i++)
	{
		word[i] = plays_.at(i).GetLetter();
	}
	board.UpdateBoard(word,plays_.at(0).GetRow(),plays_.at(0).GetColumn(),ishorizontal_);
}


void Move::UpdateRack(string newRack)
{
	for (int i = 0; i < (int)newRack.size(); i++) {
		this->rack_[i] = newRack[i];
	}
}

void Move::InitializeRack(char rack[7])
{
	for (int j = 0; j < 7; j++)
	{
		this->rack_[j] = rack[j];
	}
}

void Move::SetPlays(vector<Tile> letters, bool horizontal)
{
	ishorizontal_ = horizontal;
	
	for (int i = 0; i < (int)letters.size(); i++)
	{		
		Tile t = letters.at(i);
		int row;
		int column;
		t.GetIndex(row, column);
		Play p = Play(t,row,column);
		plays_.push_back(p);
	}
}

vector<Play> Move::GetPlays()
{
	return plays_;
}

void Move::GetRack(char rack [7])
{
	for (int j = 0; j < 7; j++)
	{
		rack[j] = this->rack_[j];
	}
}

int Move::GetScore()
{
	return score_;
}


void Move::SetScore(int score)
{
	score_ = score;
}

void Move::CalculateScore()
{
	//To be implemented in static evaluation period.

}

Move::~Move()
{
}
