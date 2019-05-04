
#pragma once
#include<iostream>
#include"Comm.h"
#include"GUI.h"
#include"Agent.h"
using namespace std;

class GameManager
{
private:
//GameBoard GameBoard;
Comm* client;
GUI* gui; 
Agent* agent;
int Opponent;
bool Game;
bool turn;  // true is our turn , false is their turn 

string mode;

vector<string> MyRack;
vector<string> HumanRack;
StrMove HumanMove; /// Human Move 
StrMove AgentMove;
StrMove HintMove;
string FbMessage;  // feedback message about the move
vector<string> ToExchange;
int Score;
int OpponentScore;
string MoveType;
void ConvertStringToVector(string tiles);
char*ConvertMessageAI(int type); //convert gamestate object to a string containing the message
char*ConvertMessageHuman(int type);
string ConvertIntRackToString(int tiles[7]);
string ConvertVecRackToString(vector<string>);
string GetCorrespondigLetter(int number);
void InterpretMessage(char*message); 
vector<string>Split(string passed);
void ConvertStringToMove(string rack,string row, string column,string direction);  // for human mode

public:
GameManager();
int InitGame();
void PlayAI(bool &ended);
void PlayHuman();
~ GameManager();
};

