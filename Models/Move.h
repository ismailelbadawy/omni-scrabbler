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
public:
	
	Move();
	void SetPlay(Play *p);

	void SetRack(string newRack);
	void SetRack(Rack rack);
	Play* GetPlay();
	string GetRack();
	double  GetScore();
	void SetScore(double score);
	void CalculateScore();
	void SetPenalty(double penalty);
	double inline GetPenalty() { return penalty_; }
	double inline GetHeuristic(){ return heuristicscore_; }
	~Move();
};

