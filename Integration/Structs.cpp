#pragma once
#include "easywsclient.hpp"
//#include "easywsclient.cpp" // <-- include only if you don't want compile separately
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include <assert.h>

#include<vector>
#include <stdio.h>
#include <string>
#include <algorithm>    // std::copy
#include<map>
#include <iostream>
using namespace std;

struct TilesStruct
{
	uint8_t Tiles[7];
};

struct ChallengeReject
{
	uint8_t Tiles[7];
	int Player_Time;
	int Total_Time;

};
struct TimesOnly
{
	int Player_Time;
	int Total_Time;
};

struct CountTime
{
	uint8_t Count;
	int Player_Time;
	int Total_Time;
};

struct ServerPlay
{
	uint8_t Column;
	uint8_t Row;
	uint8_t Direction;
	uint8_t Tiles[7];
	int Score;
};

struct AgentMove
{
	string tiles;
	int row;
	int col;
	int dir;
	int score;
};

struct WordGUI{
	string letter;
	int row;
	int col;
};


 


struct OpponentPlayState
{
	uint8_t Column;
	uint8_t Row;
	uint8_t Direction;
	uint8_t Tiles[7];
	int Score;
	int Challenge_Time;
	int Player_Time;
	int Total_Time;
    int  MoveType;   // 0 play , 1 exchange , 2 pass 
      

};

struct GameState
{
	int Order;
	int Tiles[7];
	int Board[15][15];
	int Score;
	int Opponent_Score;
	int Player_Time;
	int Total_Time;
	int ExchangedTiles[7];
	int NewTilesAfterPlay[7];
	OpponentPlayState OpponentMove;
	int OpponentExchangeCount;

};

struct AgentMessage
{   
};

struct EndState 
{
	uint8_t Reason;
	int Score;
	int Opponent_Score;
};