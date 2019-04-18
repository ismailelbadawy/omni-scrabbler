#pragma once
#include "Evaluator.h"

/// The midgame evaluator evaluates the move score in the midgame phase.
/// The strategies and techniques used should be listed here like the example below:
/// 1 - Example strategy.
/// 2 - 
class MidgameEvaluator : public Evaluator
{
    public:    
        MidgameEvaluator();
        ~MidgameEvaluator();
        double Evaluate(Move * move) override;
};