#include "Move.h"
#include <vector>


Move::Move()
{
}

void Move::UpdateRack(string newRack)
{
	for (int i = 0; i < (int)newRack.size(); i++) {
		this->rack_.SetTile(newRack[i],i);
	}
}

void Move::InitializeRack(string rack)
{
	for (int j = 0; j < 7; j++)
	{
		this->rack_.SetTile(rack[j],j);
	}
}

Play Move::GetPlay()
{
	return play_;
}

string Move::GetRack()
{
	string s ="";
	for (int j = 0; j < 7; j++)
	{
		s+= rack_.GetLetter(j);
	}
	return s;
}

int Move::GetScore()
{
	return heuristicscore_;
}


void Move::SetScore(int score)
{
	heuristicscore_ = score;
}

void Move::CalculateScore()
{
	//To be implemented in static evaluation period.

}

Move::~Move()
{
}
