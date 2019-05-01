#pragma once
#include "../Models/Move.h"

/// The abstract class (contract) for any Evaluator to implement
class Evaluator
{
    public:
        /// This function should only take a move and evaluate its score (importance maybe) accordingly.
        virtual double Evaluate(Move * move) = 0;
        virtual vector<Move> * Evaluate() = 0;
};