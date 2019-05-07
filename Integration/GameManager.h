#pragma once
#include<iostream>
#include"Comm.h"
#include"GUI.h"
#include "../Models/HumanMode.h"
#include "../Models/Agent.h"
using namespace std;

class GameManager
{
private:
//GameBoard GameBoard;
Comm* client;
GUI* gui; 
int Opponent;
bool Game;
bool turn;  // true is our turn , false is their turn 

string Mode;

MoveGenerator *MovGen_;
map<string, double>* syn2_;
map<char, double>* worth_;

Rack AI_Rack;

string MyRack;
string HumanRack;
AgentMove agentmove;
AgentMove hintmove;
vector <WordGUI> HumanMove;
string FbMessage;            // feedback message about the move
string Best;
string ToExchange;
int Score;
int OpponentScore;
int AgentScore;

int numberTilesByOpponent;

int MoveType;
int GetCorrespondigInt(char letter);
void ConvertStringToVector(string tiles);
char*ConvertMessageAI(int type); //convert gamestate object to a string containing the message
char*ConvertMessageHuman(int type);
string ConvertIntRackToString(int tiles[7]);
string ConvertVecRackToString(vector<string>);
string GetCorrespondigLetter(int number);
void InterpretMessage(char*message); 
vector<string>Split(string passed);
void ConvertStringToMove(vector<string>);  // for human mode

public:
GameManager();
int InitGame(MoveGenerator *MovGen, map<string, double>* syn2, map<char, double>* worth);
void PlayAI(bool &ended, Board *board, Bag *bag);
void PlayHuman(Board *board, Bag *bag, MoveGenerator *MovGen, map<string, double>* syn2, map<char, double>* worth);
string GetMode();
~ GameManager();
void toCapital(string &word);
void toSmall(string &word);

void toCapital(AgentMove &word);
void toSmall(AgentMove &word);

};

