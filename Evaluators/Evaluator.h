#pragma once
#include "../Models/Move.h"

/// The abstract class (contract) for any Evaluator to implement
class Evaluator
{
    public:
        virtual double Evaluate(Move * move) = 0;
};