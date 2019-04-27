#pragma once
#include"../Models/Bag.h"
#include"../Models/Rack.h"
#include"../Models/Board.h"
#include"../Models/Move.h"
#include"../MoveGenerator/MoveGenerator.h"
#include <algorithm>
#include <random>

class PreendgameEvaluator{
    Rack enemyrack_;
    Move enemymove_;
    vector<char> remainingletters_; //max size=16

    map<string, double> * doubleValued_; //temporary
    double CalculateLeave(string); //temporary

    int numTilesByOponent_; //number of tiles played by oponent in his last turn
    void Combination(int start, int size, string &rem, string &rackleave, double &MaxScore, string &BestRackLeave);
    
public:
    Rack OponentRackEstimation(); //public for testing only, should be private later
    PreendgameEvaluator();
    PreendgameEvaluator(map<string, double> *);
    ~PreendgameEvaluator();    
};