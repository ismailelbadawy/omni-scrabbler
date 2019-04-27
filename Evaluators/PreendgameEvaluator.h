#pragma once
#include"../Models/Bag.h"
#include"../Models/Rack.h"
#include"../Models/Board.h"
#include"../Models/Move.h"
#include"../MoveGenerator/MoveGenerator.h"



class PreendgameEvaluator{
    Rack enemyrack_;
    Move enemymove_;
    vector<char> remainingletters_;
public:
    PreendgameEvaluator();
    ~PreendgameEvaluator();
};