#pragma once
#include "Evaluator.h"
#include"../Models/Move.h"
#include"../Models/Board.h"
#include <map>
#include <string.h>

/// The midgame evaluator evaluates the move score in the midgame phase.
/// The strategies and techniques used should be listed here like the example below:
/// 1 - Example strategy.
/// 2 - 
class MidgameEvaluator : public Evaluator
{
    vector<Move>* possibleMoves_;
    Board* board_;
    map<string, double> * doubleValued_;
	map<char, double> * singleValued_;
    public:    
        MidgameEvaluator();
        MidgameEvaluator(vector<Move>*,Board *board, map<string, double> *, map<char, double> * );
        ~MidgameEvaluator();
        double Evaluate(Move * move) override;
        vector<Move> * Evaluate() override;
    private:
        void RackLeaveEvaluation();
        void VToCRatio();
        double CalculatePenalty();
        double CalculateLeave(string);
        int GetBonus(int row, int column);
};