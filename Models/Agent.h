#include "Board.h"
#include "Bag.h"
#include "Rack.h"
#include "Move.h"
#include "../MoveGenerator/MoveGenerator.h"
#include "../Evaluators/Evaluator.h"
#include "../Evaluators/MidgameEvaluator.h"
#include "../Evaluators/PreendgameEvaluator.h"

class Agent{
    Board *board_;
    Bag *bag_;
    Rack *rack_;
    
    public:
    Agent(Board *, Bag*, Rack*);

    void MidGame(vector<Move>, map<string, double> * rackLeave, map<char, double> * charValue);
    void PreEndGame(map<string, double> * syn2, MoveGenerator * movGen, vector <Move> moves);
    void EndGame(vector<Move>);
}
