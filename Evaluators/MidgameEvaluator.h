#pragma once
#include "Evaluator.h"
#include"../Models/Move.h"
#include"../Models/Board.h"

/// The midgame evaluator evaluates the move score in the midgame phase.
/// The strategies and techniques used should be listed here like the example below:
/// 1 - Example strategy.
/// 2 - 
class MidgameEvaluator : public Evaluator
{
    vector<Move*> possibleMoves_;
    Board* board_;
    public:    
        MidgameEvaluator();
        MidgameEvaluator(vector<Move>,Board *board);
        ~MidgameEvaluator();
        double Evaluate(Move * move) override;
    private:
        void RackLeaveEvaluation();
        void VToCRatio();
        double CalculatePenalty();
        int GetBonus(int row, int column);
};