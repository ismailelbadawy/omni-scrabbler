#include "Agent.h"

Agent::Agent(Board *board, Bag* bag, Rack* rack){
    board_ = board;
    bag_ = bag;
    rack_ = rack;
}

void Agent::MidGame(vector <Move> moves, map<string, double> * syn2, map<char, double> * worth, MoveGenerator * movGen){
    MidgameEvaluator* evaluator = NULL;
    evaluator = new MidgameEvaluator(&moves, board_, syn2, worth);
	vector<Move> * evaluatedMoves = evaluator->Evaluate();
    MonteCarlo Simulator(*this->board_,*evaluatedMoves,*rack_, *rack_,*this->bag_,movGen,syn2,worth); //bool true

    //return best move
    Move BestMove= (*evaluatedMoves)[0];
}

void Agent::PreEndGame(map<string, double> * syn2,map<char, double>* worth, MoveGenerator * movGen, vector <Move> moves){
    vector <char> remLetters= bag_->GetRemainigLetters();
    PreendgameEvaluator* PreEval = NULL;
    PreEval = new PreendgameEvaluator(syn2,board_,movGen,moves, remLetters);
    vector<Move> * EvaluatorMoves = PreEval->Evaluate();
    MonteCarlo Simulator(*this->board_,*EvaluatorMoves,*rack_, *rack_,*this->bag_,movGen,syn2,worth); //bool false
    //send EvaluatorMoves to simulator 
    //return best move by simulation
}

Move Agent::GetPassMove(){
    Play *passPlay = new Play("",0,0,false);
    Move passMove;
    passPlay->SetScore(0);
    passMove.SetRack(*this->rack_);
    passMove.SetPlay(passPlay);
    passMove.SetPenalty(0);
    passMove.SetHeuristic();
    passMove.CalculateScore();
    return passMove;
}

void Agent::EndGame(vector<Move> moves){

}