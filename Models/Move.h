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
	int heuristicscore_;


public:
	
	Move();
	void SetPlay(Play *p);

	void SetRack(string newRack);
	void SetRack(Rack rack);
	Play* GetPlay();
	string GetRack();
	int  GetScore();
	void SetScore(int score);
	void CalculateScore();
	~Move();
};

