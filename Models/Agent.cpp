#include"Agent.h"

Agent::Agent(Board *board, Rack* rack, Bag* bag, MoveGenerator* movegenerator){
    board_ = board;
    rack_ = rack;
    bag_ = bag;
    MoveGenerator movgen(*this->board_);
    this->movegenerator_ = movegenerator;
    this->syn2_ = new map<string, double>();
	this->worth_ = new map<char, double>();
	SuperLeaveLoader loader(syn2_, worth_, "assets/syn2", "assets/worths");

}

void Agent::MidGame(vector<Move>* moves){
    MidgameEvaluator midgameEvaluator(moves,this->board_,this->syn2_,this->worth_);
    vector<Move>* evaluatedMoves = new vector<Move>();
    evaluatedMoves = midgameEvaluator.Evaluate();
    
    if (this->bag_->GetRemainingTiles() < 40 )
    {
        //instantiate from the simulation
    }

    // Choose the best move.
    this->chosenMove = Move(evaluatedMoves->at(0));

    delete evaluatedMoves;
}
