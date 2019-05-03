#include "Board.h"
#include "Bag.h"
#include "Rack.h"
#include "Move.h"
#include "../MoveGenerator/MoveGenerator.h"
#include "../Evaluators/Evaluator.h"
#include "../Evaluators/MidgameEvaluator.h"
#include "../Evaluators/PreendgameEvaluator.h"
#include"../MonteCarlo/MonteCarlo.h"

class Agent{
    Board *board_;
    Bag *bag_;
    Rack *rack_;
    
    public:
    Agent(Board *, Bag*, Rack*);

    void MidGame(vector<Move>, map<string, double> * rackLeave, map<char, double> * charValue, MoveGenerator * movGen);
    void PreEndGame(map<string, double> * syn2,map<char, double>* worth, MoveGenerator * movGen, vector <Move> moves);
    void EndGame(vector<Move>);
    Move GetPassMove();
};
