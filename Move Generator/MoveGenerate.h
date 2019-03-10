#pragma once
#include<iostream>
#include<vector>
#include"Models/Board.h"
#include"Models/Move.h"
#include"Models/Rack.h"

using namespace std;

class MoveGenerate{
   
    public:
    MoveGenerate();
    void send_Col(Board &board);
    void send_Row(Board &board);
    bool check_other_dimension(Board board,string word,int row,int col,bool horizontal);
    vector<Play> check_words(Board&board,vector<pair<string,vector<int> > > returnedWords,int row,int col,bool horizontal);
    ~MoveGenerate();
};