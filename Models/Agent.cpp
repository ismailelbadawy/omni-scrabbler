#include "Agent.h"

Agent::Agent(Board *board, Bag* bag, Rack* rack){
    board_ = board;
    bag_ = bag;
    rack_ = rack;
}

double Agent::CalculateLeave(string rack, map<string, double> *doubleValued)
{
	double rackLeave = 0.0;
    for(int i = 0; i < (int)rack.length(); i++)
    {
        for(int j = i + 1; j < (int)rack.length(); j++)
        {
            // Rack
            if(i != j)
            {
				char leaveValue[] = { rack[i] == '?' ? '?' : (rack[i] - 32), rack[j] == '?' ? '?' : (rack[j] - 32)};
				string leave(leaveValue);
				leave = leave.substr(0, 2);
				rackLeave += (*doubleValued)[leave];
            }
        }
    }
	return rackLeave;
}

void Agent:: Combination(int start, int size, string &rem, string &temprack, double &MaxScore, string &BestRackLeave, map<string, double> *doubleValued){
    if (size == 0) {
		//double rackleave_score = rackleavefn(temprack)
        double rackleave_score = CalculateLeave(temprack, doubleValued);
        if (rackleave_score > MaxScore){
            MaxScore = rackleave_score;
            BestRackLeave = temprack;
            return;
        }
        //else keep the max and best rack leave as they are and return
		return;
	}
	for (int i = start; i <= (int)rem.size() - size; ++i) {
		temprack.push_back(rem[i]);
		Combination(i + 1, size - 1, rem, temprack, MaxScore, BestRackLeave, doubleValued);
		temprack.pop_back();
	}

}

Rack Agent::OpponentRackEstimation(map<string, double> *doubleValued, vector <char> remaininglettersVector, int numTilesByOpponent){
    //vector<char> remainingletters = remainingletters_; //take a copy of remainingletters_
    string remainingletters= "";
    for (int i=0; i< (int)remaininglettersVector.size(); i++){
        remainingletters += remaininglettersVector[i];
    }
    
    int TilesLeftOponent = 7 - numTilesByOpponent; //if opponent played 3 letters in the last turn, there are 4 letters left to guess
    double MaxRackLeave=0.0;

    //vector<char> tempRack, OpponentRack;
    string tempRack, OpponentRack;
  
    //get first part of oponent rack based on his last move
    Combination(0, TilesLeftOponent, remainingletters, tempRack, MaxRackLeave, OpponentRack, doubleValued);

    //remove from remainingletters letters picked in opponent Rack
    for (int i=0; i< (int)OpponentRack.size(); i++){
        remainingletters.erase(std::remove(remainingletters.begin(), remainingletters.end(), OpponentRack[i]), remainingletters.end());
    } 

    //get second part of oponent rack randomly
    for (int i = 0; i < numTilesByOpponent; i++) {
		int randomIndex = rand() % remainingletters.size();
		OpponentRack.push_back(remainingletters[randomIndex]);	
		remainingletters.erase(remainingletters.begin()+randomIndex);

	}

    string OppRackString ="";
    for (int i=0; i<(int)OpponentRack.size(); i++){
        OppRackString += OpponentRack[i];
    }
    Rack rack(OppRackString);
    return rack;
}

Move Agent::MidGame(vector <Move> moves, map<string, double> * syn2, map<char, double> * worth, MoveGenerator * movGen, int numTilesByEnemy){
    MidgameEvaluator* evaluator = NULL;
    evaluator = new MidgameEvaluator(&moves, board_, syn2, worth);
	vector<Move> * evaluatedMoves = evaluator->Evaluate();

    vector<char> remLettersBag = this->bag_->GetRemainigLetters();
    
    if (this->bag_->GetRemainigLetters().size()<30)
    {
        Rack oponentRack = OpponentRackEstimation(syn2, remLettersBag, numTilesByEnemy);
        MonteCarlo Simulator(*this->board_,*evaluatedMoves,*rack_, *rack_,*this->bag_,movGen,syn2,worth,true, numTilesByEnemy); //bool true
        
        this->chosenMove_ = &(*evaluatedMoves)[Simulator.Simulation()];
    }
    else{
    //return best move
    this->chosenMove_ = &(*evaluatedMoves)[0];
    }
    return GetChosenMove();

}

Move Agent::PreEndGame(map<string, double> * syn2,map<char, double>* worth, MoveGenerator * movGen, vector <Move> moves, int numTilesByOpponent){
    vector <char> remLetters= bag_->GetRemainigLetters();
    PreendgameEvaluator* PreEval = NULL;
    PreEval = new PreendgameEvaluator(syn2,board_,movGen,moves, remLetters, numTilesByOpponent);
    vector<Move> * EvaluatorMoves = PreEval->EvaluateGame();
    Rack oponentRack = PreEval->GetEnemyRack();
    MonteCarlo Simulator(*this->board_,*EvaluatorMoves,*rack_, oponentRack,*this->bag_,movGen,syn2,worth,false,numTilesByOpponent); //bool false
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