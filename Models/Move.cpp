#include "Move.h"
#include <vector>
#include <algorithm>

Move::Move()
{
}

void Move::SetPlay(Play* p){
	this->play_ = p;

}


void Move::SetRack(Rack rack){
	this->rack_=rack;
}


void Move::SetRack(string newRack)
{
	for (int i = 0; i < (int)newRack.size(); i++) {
		this->rack_.SetTile(newRack[i],i);
	}
}

Play* Move::GetPlay()
{
	return play_;
}

string Move::GetRack()
{
	string s = "";
	for (int j = 0; j < rack_.GetLength(); j++)
	{
		s+= rack_.GetLetter(j);
	}
	return s;
}

double Move::GetScore()
{
	return heuristicscore_;
}


void Move::SetScore(double score)
{
	heuristicscore_ = score;
}

void Move::CalculateScore()
{
	this->heuristicscore_ =this->rackLeave_+this->play_->GetScore()-this->penalty_;

}

void Move::SetPenalty(double penalty){
	this->penalty_ = penalty;
}

void Move::SetRackLeave(double leave)
{
	this->rackLeave_ = leave;
}

bool operator<(const Move & m1, const Move & m2)
{
	return m1.GetHeuristic() > m2.GetHeuristic();
}

Move::~Move()
{
}
