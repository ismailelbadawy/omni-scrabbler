#pragma once
#include<iostream>
#include<vector>
#include"Play.h"
#include"Rack.h"

using namespace std;
class Move
{
	Play *play_;
	Rack rack_;
	double heuristicscore_;
	double penalty_;
	double rackLeave_;
public:
	
	Move();
	void SetPlay(Play *p);

	void SetRack(string newRack);
	void SetRack(Rack rack);
	Play* GetPlay();
	string GetRack();
	double  GetScore();
	void SetScore(double score);
	Rack Getwholerack();
	void CalculateScore();
	void SetPenalty(double penalty);
	void SetRackLeave(double leave);
	void SetHeuristic() { this->heuristicscore_ = (this->GetScore() + this->GetRackLeave() - this->GetPenalty() * 0.5); }
	double inline GetPenalty() { return this->penalty_; }
	double inline GetHeuristic() const { return this->heuristicscore_; }
	double inline GetRackLeave(){ return this->rackLeave_; }
	~Move();
};

bool operator<(const Move & m1, const Move & m2);
