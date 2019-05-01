#include"Board.h"
#include"Rack.h"
#include"Bag.h"
#include"Move.h"
#include"../MoveGenerator/MoveGenerator.h"
#include"../Evaluators/MidgameEvaluator.h"
#include"../Evaluators/PreendgameEvaluator.h"
#include "../Strategy/SuperLeaveLoader.h"


class Agent{
    Board *board_;
    Rack *rack_;
    Bag *bag_;
    MoveGenerator *movegenerator_;
    map<string, double>* syn2_;
    map<char, double>* worth_;
    Move chosenMove;
    
    public:
    Agent();
    Agent(Board *, Rack*, Bag*, MoveGenerator*);

    //instead of void, data type fatma hat2oul 3aleh
    void MidGame(vector<Move>*);
    void PreEndGame();
    void EndGame();
    void ReturnMove (Move *FinalMove);

};