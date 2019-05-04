#pragma once
#include<iostream>
using namespace std;


class Agent{
    private:
    int turn ;
    string move ; // PLAY, PASS , EXCHANGE

public:
   Agent();
   int GetTurn();
   string GetMove();

   ~Agent();
   


};