#pragma once
#include<iostream>
#include<vector>
#include"Board.h"
#include"Play.h"
#include"Tile.h"
#include"Rack.h"

using namespace std;
class Move
{
	vector<Play> plays_;
	char rack_[7];
	int score_;
	int remainingtiles_;
	bool ishorizontal_;

public:
	
	Move();
	Move(Board &board, bool horizontal);
	void UpdateRack(string newRack);
	void InitializeRack(char rack[7]);
	void SetPlays(vector<Tile> letters,bool horizontal);
	vector<Play> GetPlays();
	void GetRack(char rack[7]);
	int  GetScore();
	void SetScore(int score);
	void CalculateScore();
	~Move();
};

