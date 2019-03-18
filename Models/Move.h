#pragma once
#include<iostream>
#include<vector>
#include"Play.h"
#include"Rack.h"

using namespace std;
class Move
{
	vector<Play> plays_;
	Rack rack_;
	int score_;
	int remainingtiles_;
	bool ishorizontal_;

public:
	
	Move();
	void UpdateRack(string newRack);
	void InitializeRack(char rack[7]);
	vector<Play> GetPlays();
	void GetRack(char rack[7]);
	int  GetScore();
	void SetScore(int score);
	void CalculateScore();
	~Move();
};

