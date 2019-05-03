#include "HumanMode.h"

HumanMode::HumanMode(Board *board, Bag* bag){
    board_ = board;
    bag_ = bag;
}

void HumanMode::SetOpponentRack(Rack &rack){
    vector <Tile> bagRemLetters = this->bag_->GetRemainingTiles();
    vector <Tile> OpponentRack = rack.GetRackTiles();

    int RackSize= OpponentRack.size();
    
    for (int i=0; i< 7-RackSize; i++){
        if (bagRemLetters.size() > 0){
            int randomIndex = rand() % bagRemLetters.size();
		    OpponentRack.push_back(bagRemLetters[randomIndex]);
            bag_->TakeLetter(bagRemLetters[randomIndex]);	
		    bagRemLetters.erase(bagRemLetters.begin()+randomIndex);
        }
        else 
            break;
        
    }
    rack.SetRackTiles(OpponentRack);
    opponentRack_ = &rack;
}

void HumanMode::SetMyRack(Rack &rack){
    vector <Tile> bagRemLetters = this->bag_->GetRemainingTiles();
    vector <Tile> MyRack = rack.GetRackTiles();

    int RackSize = MyRack.size();
    
    for (int i=0; i< 7-RackSize; i++){
        if (bagRemLetters.size() > 0){
            int randomIndex = rand() % bagRemLetters.size();
		    MyRack.push_back(bagRemLetters[randomIndex]);	
            bag_->TakeLetter(bagRemLetters[randomIndex]);
		    bagRemLetters.erase(bagRemLetters.begin()+randomIndex);
        }
        else 
            break;
        
    }
    rack.SetRackTiles(MyRack);
    rack_ = &rack;
}

Move HumanMode::MidGame(vector <Move> moves, map<string, double> * syn2, map<char, double> * worth, MoveGenerator * movGen){
    MidgameEvaluator* evaluator = NULL;
    evaluator = new MidgameEvaluator(&moves, board_, syn2, worth);
	vector<Move> * evaluatedMoves = evaluator->Evaluate();

    if (this->bag_->GetRemainigLetters().size()<30){ //simulate
        MonteCarlo Simulator(*this->board_,*evaluatedMoves,*rack_, *opponentRack_,*this->bag_,movGen,syn2,worth,true, -1); //bool true   
        this->chosenMove_ = &(*evaluatedMoves)[Simulator.Simulation()];
    }
    else{
    //return best move
        this->chosenMove_ = &(*evaluatedMoves)[0];
    }
    return GetChosenMove();
}

Move HumanMode::PreEndGame(map<string, double> * syn2,map<char, double>* worth, MoveGenerator * movGen, vector <Move> moves){
    vector <char> remLetters= bag_->GetRemainigLetters();
    PreendgameEvaluator* PreEval = NULL;
    PreEval = new PreendgameEvaluator(syn2,board_,movGen,moves, remLetters, -1);
    vector<Move> * EvaluatorMoves = PreEval->EvaluateGame();
    Rack oponentRack = PreEval->GetEnemyRack();
    MonteCarlo Simulator(*this->board_,*EvaluatorMoves,*rack_, oponentRack,*this->bag_,movGen,syn2,worth,false,-1); //bool false
    //send EvaluatorMoves to simulator 
    this->chosenMove_ =&(*EvaluatorMoves)[Simulator.Simulation()];
    //return best move by simulation
    return GetChosenMove();
}

Move HumanMode::GetChosenMove(){
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