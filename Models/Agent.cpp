#include "Agent.h"

Agent::Agent(Board *board, Bag* bag, Rack* rack){
    board_ = board;
    bag_ = bag;
    rack_ = rack;
}

Move Agent::MidGame(vector <Move> moves, map<string, double> * syn2, map<char, double> * worth, MoveGenerator * movGen){
    MidgameEvaluator* evaluator = NULL;
    evaluator = new MidgameEvaluator(&moves, board_, syn2, worth);
	vector<Move> * evaluatedMoves = evaluator->Evaluate();
    if (this->bag_->GetRemainigLetters().size()<30)
    {
        MonteCarlo Simulator(*this->board_,*evaluatedMoves,*rack_, *rack_,*this->bag_,movGen,syn2,worth,true); //bool true
        
        this->chosenMove_ = &(*evaluatedMoves)[Simulator.Simulation()];
    }
    else{
    //return best move
    this->chosenMove_ = &(*evaluatedMoves)[0];
    }
    return *this->chosenMove_;

}

Move Agent::PreEndGame(map<string, double> * syn2,map<char, double>* worth, MoveGenerator * movGen, vector <Move> moves){
    vector <char> remLetters= bag_->GetRemainigLetters();
    PreendgameEvaluator* PreEval = NULL;
    PreEval = new PreendgameEvaluator(syn2,board_,movGen,moves, remLetters);
    vector<Move> * EvaluatorMoves = PreEval->Evaluate();
    MonteCarlo Simulator(*this->board_,*EvaluatorMoves,*rack_, *rack_,*this->bag_,movGen,syn2,worth,false); //bool false
    //send EvaluatorMoves to simulator 
    this->chosenMove_ =&(*EvaluatorMoves)[Simulator.Simulation()];
    //return best move by simulation
    return *this->chosenMove_;
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

Move Agent::GetChosenMove(){
    int rowIterator = this->chosenMove_->GetPlay()->GetRow();
    int columnIerator = this->chosenMove_->GetPlay()->GetColumn();
    Tile* boardTiles[15][15];
    this->board_->GetTiles(boardTiles);
    Play* filteredPlay = NULL;
    for (int i = 0; i < (int)this->chosenMove_->GetPlay()->GetLetters().size(); i++)
    {
        if (this->chosenMove_->GetPlay()->GetLetters()[i] != boardTiles[rowIterator][columnIerator]->GetLetter() ){
            // create a tile
            // add the tile to the pointer to play
            // create a move to return it with the letters actually played on the board.
        }
        if (this->chosenMove_->GetPlay()->GetIsHorizontal()){
            columnIerator++;
        }
        else{
            rowIterator++;
        }
    }
    return *this->chosenMove_;
    

}

void Agent::EndGame(vector<Move> moves){

}