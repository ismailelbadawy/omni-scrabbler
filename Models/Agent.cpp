#include "Agent.h"

Agent::Agent(Board *board, Bag* bag, Rack* rack){
    board_ = board;
    bag_ = bag;
    rack_ = rack;
}

void Agent::MidGame(vector <Move> moves, map<string, double> * syn2, map<char, double> * worth){
    MidgameEvaluator* evaluator = NULL;
    evaluator = new MidgameEvaluator(&moves, board_, syn2, worth);
	vector<Move> * evaluatedMoves = evaluator->Evaluate();

    //return best move
    Move BestMove= (*evaluatedMoves)[0];
}

void Agent::PreEndGame(map<string, double> * syn2, MoveGenerator * movGen, vector <Move> moves){
    vector <char> remLetters= bag_->GetRemainigLetters();
    PreendgameEvaluator* PreEval = NULL;
    PreEval = new PreendgameEvaluator(syn2,board_,movGen,moves, remLetters);
    vector<Move> * EvaluatorMoves = PreEval->Evaluate();

    //send EvaluatorMoves to simulator 
    //return best move by simulation
}

void Agent::EndGame(vector<Move> moves){

}