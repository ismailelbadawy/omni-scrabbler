#pragma once
#include "Evaluator.h"
#include"../Models/Move.h"
#include"../Models/Board.h"
#include"../MoveGenerator/MoveGenerator.h"
#include <map>
#include <string.h>

/// The endgame evaluator evaluates the move score in the endgame phase.
/// The strategies and techniques used should be listed here like the example below:
/// 1 - Example strategy.
/// 2 - 
class EndgameEvaluator : public Evaluator
{
    vector<Move> possibleMoves_;
    Board board_;
    map<string, double> * doubleValued_;
	map<char, double> * singleValued_;
    Rack MyRack;
    Rack OppRack;
    bool OppRackEvaluated=false;
    MoveGenerator *movegenerator_;


    double CalculateLeave(string);
    int GetBonus(int rowIterator, int columnIterator, Board board);
    double CalculatePenalty(Move * move, Board board);
     Rack EvaluateRack();
    vector<Move> Qsticking();

    public:    
        EndgameEvaluator(){}
        EndgameEvaluator(vector<Move> moves, Board board, map<string, double> * rackLeave, map<char, double> * charValue, MoveGenerator * movGen,Rack MyRack);
        ~EndgameEvaluator();
        void SetAttributes(vector<Move> possibleMoves_,Board board_,Rack MyRack,Rack OppRack);
        Rack GetOppRack(){return OppRack;}
        double Evaluate(Move * move) override;
        vector<Move> * Evaluate() override;
       
    
       
};