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
    if (this->bag_->GetRemainigLetters().size()<300)
    {
        MonteCarlo Simulator(*this->board_,*evaluatedMoves,*rack_, *rack_,*this->bag_,movGen,syn2,worth,true); //bool true
        
        this->chosenMove_ = &(*evaluatedMoves)[Simulator.Simulation()];
    }
    else{
    //return best move
    this->chosenMove_ = &(*evaluatedMoves)[0];
    }
    return GetChosenMove();

}

Move Agent::PreEndGame(map<string, double> * syn2,map<char, double>* worth, MoveGenerator * movGen, vector <Move> moves){
    vector <char> remLetters= bag_->GetRemainigLetters();
    PreendgameEvaluator* PreEval = NULL;
    PreEval = new PreendgameEvaluator(syn2,board_,movGen,moves, remLetters);
    vector<Move> * EvaluatorMoves = PreEval->EvaluateGame();
    MonteCarlo Simulator(*this->board_,*EvaluatorMoves,*rack_, *rack_,*this->bag_,movGen,syn2,worth,false); //bool false
    //send EvaluatorMoves to simulator 
    this->chosenMove_ =&(*EvaluatorMoves)[Simulator.Simulation()];
    //return best move by simulation
    return GetChosenMove();
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
    // create a tile
            // add the tile to the pointer to play
            // create a move to return it with the letters actually played on the board.
    vector <Tile> FinalTiles= this->chosenMove_->GetPlay()->GetTiles();
    for (int i=0; i< (int)FinalTiles.size(); i++){
        FinalTiles[i].GetIndex(rowIterator, columnIerator);
        if (!board_->IsPositionEmpty(rowIterator, columnIerator)){ //tile already exists on board
            FinalTiles.erase(FinalTiles.begin() + i);
            i--;
    
        }
        if (this->chosenMove_->GetPlay()->GetIsHorizontal()){
            columnIerator++;
        }
        else{
            rowIterator++;
        }
    }
    this->chosenMove_->GetPlay()->SetTiles(FinalTiles);
    return *this->chosenMove_;
    

}

void Agent::EndGame(vector<Move> moves){

}