#pragma once
#include "Evaluator.h"

class MidgameEvaluator : public Evaluator
{
    public:    
        MidgameEvaluator();
        ~MidgameEvaluator();
        double Evaluate(Move * move);
};