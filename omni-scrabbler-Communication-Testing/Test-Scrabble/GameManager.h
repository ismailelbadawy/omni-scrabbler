
#pragma once
#include<iostream>
#include"Comm.h"
#include"GUI.h"
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

public:
GameManager();
int InitGame();
void Play();
// char*ConvertMessage(); //convert gamestate object to a string containing the message
~ GameManager();
};