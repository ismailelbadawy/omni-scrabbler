#pragma once
#include "Board.h"
#include "Bag.h"
#include "Rack.h"
#include "Move.h"
#include "../MoveGenerator/MoveGenerator.h"
#include "../Evaluators/Evaluator.h"
#include "../Evaluators/MidgameEvaluator.h"
#include "../Evaluators/PreendgameEvaluator.h"
#include "../Evaluators/EndgameEvaluator.h"

#include"../MonteCarlo/MonteCarlo.h"
#include"../EndGameSimulation/EndGameSim.h"

class Agent{
    Board *board_;
    Bag *bag_;
    Rack *rack_;
    Move *chosenMove_;

    void Combination(int start, int size, string &rem, string &rackleave, double &MaxScore, string &BestRackLeave, map<string, double> *doubleValued);
    Rack OpponentRackEstimation(map<string, double> *doubleValued, vector <char> remainingletters, int numTilesByOpponent);
    double CalculateLeave(string, map<string, double> *rackLeave); //temporary

    public:
    Agent(Board *, Bag*, Rack*);
    
    Move MidGame(vector<Move>, map<string, double> * rackLeave, map<char, double> * charValue, MoveGenerator * movGen, int numTilesByOpponent);
    Move PreEndGame(map<string, double> * syn2,map<char, double>* worth, MoveGenerator * movGen, vector <Move> moves, int numTilesByOpponent);
    Move EndGame(vector<Move> moves, map<string, double> * syn2, map<char, double> * worth, MoveGenerator * movGen,Rack MyRack, int numTilesByOpponent);
    Move GetChosenMove(); // Returns the proper move format to be sent (removes letters already on the board from the Play)
    Move GetPassMove();
};
