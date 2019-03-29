#pragma once
#include<iostream>
#include<vector>
#include"Play.h"
#include"Rack.h"

using namespace std;
class Move
{
	Play play_;
	Rack rack_;
	int heuristicscore_;
	int remainingtiles_;
	bool ishorizontal_;

public:
	
	Move();
	
	void UpdateRack(string newRack);
	void InitializeRack(string rack);
	Play GetPlay();
	string GetRack();
	int  GetScore();
	void SetScore(int score);
	void CalculateScore();
	~Move();
};

