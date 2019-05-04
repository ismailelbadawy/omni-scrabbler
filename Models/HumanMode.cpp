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
        if ((*evaluatedMoves).size() > 0){
            this->chosenMove_ = &(*evaluatedMoves)[Simulator.Simulation()];
            return GetChosenMove();
        }  else {
                Move NullMove;
                Tile t;
                t.SetLetter('#');
                vector <Tile> tvector;
                tvector.push_back(t);
                Play p;
                p.SetTiles(tvector);
                
                NullMove.SetPlay(&p);
                return NullMove;
        }
    }
    else{
    //return best move
        if((*evaluatedMoves).size() > 0){
            int index=0;
            this->chosenMove_ = &(*evaluatedMoves)[0];
            if (board_->GetCount() == 0){//first move --> no bingo
                while (chosenMove_->GetRack().length() == 0){//bingo
                    index++;
                    if (index < (*evaluatedMoves).size()){
                        this->chosenMove_ = &(*evaluatedMoves)[index];
                    }
                    
                }
                return GetChosenMove(); //if bingo is the only available move, play it, or return first none bingo move 
            }   
            return GetChosenMove();
        }
        else{
            Move NullMove;
            Tile t;
                t.SetLetter('#');
                vector <Tile> tvector;
                tvector.push_back(t);
                Play p;
                p.SetTiles(tvector);
                
                NullMove.SetPlay(&p);
            return NullMove;
    }
    } 
}

Move HumanMode::PreEndGame(map<string, double> * syn2,map<char, double>* worth, MoveGenerator * movGen, vector <Move> moves){
    vector <char> remLetters= bag_->GetRemainigLetters();
    PreendgameEvaluator* PreEval = NULL;
    PreEval = new PreendgameEvaluator(syn2,board_,movGen,moves, remLetters, -1);
    vector<Move> * EvaluatorMoves = PreEval->EvaluateGame();
    Rack oponentRack = PreEval->GetEnemyRack();
    MonteCarlo Simulator(*this->board_,*EvaluatorMoves,*rack_, oponentRack,*this->bag_,movGen,syn2,worth,false,-1); //bool false
    //send EvaluatorMoves to simulator 
    if ((*EvaluatorMoves).size() > 0){
        this->chosenMove_ =&(*EvaluatorMoves)[Simulator.Simulation()]; 
        return GetChosenMove();
    }
    else{
        Move NullMove;
        Tile t;
                t.SetLetter('#');
                vector <Tile> tvector;
                tvector.push_back(t);
                Play p;
                p.SetTiles(tvector);
                
                NullMove.SetPlay(&p);
        return NullMove;
    }
    //return best move by simulation
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

Play HumanMode::GetOpponentPlay(bool Horizontal, int row, int col, char *Wordarr, int size, Rack &OpponentRack, Tile* boardTiles[15][15]){
    int inc=0;
	int ArrayIndex=0;
    vector<Tile> ActualPlay;
    while (1){
		if (Horizontal){
			if (boardTiles[row][col+inc]->GetLetter() == '0' && ArrayIndex < size){//board is empty at this position, draw from array
				int prevSize= OpponentRack.GetLength();
                Tile t= OpponentRack.RemoveAndReturnTile(Wordarr[ArrayIndex]);
                int newSize = OpponentRack.GetLength();
                if (newSize == prevSize){ //tiles was not found in rack, search for blank
                    Tile t= OpponentRack.RemoveAndReturnTile(Wordarr['?']);
                    t.SetLetter(Wordarr[ArrayIndex]);
                }
                else{
                    ActualPlay.push_back(t);
                }
                
				ArrayIndex++;
			}
			else if (boardTiles[row][col+inc]->GetLetter() != '0' && ArrayIndex < size){ //word on board
				ActualPlay.push_back(*boardTiles[row][col+inc]);
			}
			else {
				break;
			}
		}
		else{//vertical
			if (boardTiles[row+inc][col]->GetLetter() == '0' && ArrayIndex < size){//board is empty at this position, draw from array
				int prevSize= OpponentRack.GetLength();
                Tile t= OpponentRack.RemoveAndReturnTile(Wordarr[ArrayIndex]);
                int newSize = OpponentRack.GetLength();
                if (newSize == prevSize){ //tiles was not found in rack, search for blank
                    Tile t= OpponentRack.RemoveAndReturnTile(Wordarr['?']);
                    t.SetLetter(Wordarr[ArrayIndex]);
                }
                else{
                    ActualPlay.push_back(t);
                }
				ArrayIndex++;
			}
			else if (boardTiles[row+inc][col]->GetLetter() != '0' && ArrayIndex < size){ //word on board
				ActualPlay.push_back(*boardTiles[row][col+inc]);
			}
			else {
				break;
            }    
		
	    }
        inc++;
    }
    Play p;
    p.SetTiles(ActualPlay);
    return p;
}

void HumanMode::AddPlayToBoard(Play p, Tile* boardTiles[15][15]){
    bool Horizontal = p.GetIsHorizontal();
    vector <Tile> tiles = p.GetTiles();
    int row = p.GetRow();
    int col= p.GetColumn();

    for (int i=0; i< tiles.size(); i++){
        char letter= tiles[i].GetLetter();
        int score= tiles[i].GetScore();
        if (Horizontal){
			if (boardTiles[row][col+i]->GetLetter() == '0'){//board is empty at this position, draw from array
                board_->Probe(letter, row, col+i, score);
			}
		}
		else{//vertical
			if (boardTiles[row+i][col]->GetLetter() == '0'){//board is empty at this position, draw from array
				board_->Probe(letter, row+i, col, score);
			} 
		
	    }
    }

}

//NOTE: this fn doesn't need to check on the board as the chosen move is already filtred
void HumanMode::UpdateBoardAndRack(Play p, Rack &rack){
    vector <Tile> tiles = p.GetTiles();

    for (int i=0; i< tiles.size(); i++){
        char letter= tiles[i].GetLetter();
        int score= tiles[i].GetScore();
        int row, col;
        tiles[i].GetIndex(row, col);

        board_->Probe(letter, row, col, score);

        int Prevsize= rack.GetLength();
        rack.RemoveTile(letter);
        int Newsize= rack.GetLength();
        if (Newsize == Prevsize){ //letter wasn't found, search for blank
        rack.RemoveTile('?');
        }
    }
}