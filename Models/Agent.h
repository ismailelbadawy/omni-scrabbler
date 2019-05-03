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
    Move *chosenMove_;
    public:
    Agent(Board *, Bag*, Rack*);

    Move MidGame(vector<Move>, map<string, double> * rackLeave, map<char, double> * charValue, MoveGenerator * movGen);
    Move PreEndGame(map<string, double> * syn2,map<char, double>* worth, MoveGenerator * movGen, vector <Move> moves);
    void EndGame(vector<Move>);
    Move GetChosenMove(); // Returns the proper move format to be sent (removes letters already on the board from the Play)
    Move GetPassMove();
};
