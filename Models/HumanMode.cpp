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
            srand((int)time(NULL));
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
             srand((int)time(NULL));
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
    this->doubleValued_ = syn2;
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
                    if (index <(int)  (*evaluatedMoves).size()){
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
    return *chosenMove_;
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

Play HumanMode::GetOpponentPlay(vector<WordGUI> wordVector, Rack &OpponentRack, Tile* boardTiles[15][15]){
    bool Horizontal;
    bool valid= true;

    int row = wordVector[0].row;
    int col = wordVector[0].col;
    //check horizontal
    for (int i=1; i <(int)wordVector.size(); i++){
        if (wordVector[i].row != row){
            valid = false; 
        }
    }

    //word wasn't horizontal, check vertical
    if (!valid){
        valid = true;
        for (int i=1; i <(int)wordVector.size(); i++){
            if (wordVector[i].row != row){
                valid = false; 
            }
        }

        if (!valid){ //word is not horizontal neither vertical, return NULL Play noted by play with pos = -1, -1
            Play p;
            p.SetStartPos(-1, -1);
            return p;
        }
        else{
            Horizontal = false;
        }
    }else{
        Horizontal = true;
    }
    
    //by here, word is on the same col or same row

    //get Start Position
    //Get letters that are on the board before the word
    int rowDec;
    int colDec;
    if (Horizontal) {
        rowDec=0;
        colDec = -1;
    }else{
        rowDec=-1;
        colDec = 0;
    }

    //row =7, col=7 -> if horizontal check 7,6, but don't change col value unless you are sure there is a tile on board at pos 7,6
    while( ((row + rowDec) >0 && (col + colDec) >0) && boardTiles[row + rowDec][col + colDec]->GetLetter()!='0'){
        if (Horizontal){
            col--;
        }
        else{
            row--;
        }
    }

	int ArrayIndex=0;
    vector<Tile> ActualPlay;
    valid = true;

    //by here, we have the start pos in row, col, whether it's not the board or it's a new letter
    while (1){
		if (boardTiles[row][col]->GetLetter() == '0' && ArrayIndex < (int)wordVector.size() && wordVector[ArrayIndex].row== row && wordVector[ArrayIndex].col == col){
            //board is empty at this position, there are still new tiles, and this tile is at pos row, col, then draw it from array
			int prevSize= OpponentRack.GetLength();
            Tile t= OpponentRack.RemoveAndReturnTile(wordVector[ArrayIndex].letter[0]);
            t.setPosition(row, col);
            //t.SetParams(wordVector[ArrayIndex].letter[0],row,col,tilescorecalculator_.GetTileScore(wordVector[ArrayIndex].letter[0]),1);
            int newSize = OpponentRack.GetLength();
            if (newSize == prevSize){ //tiles was not found in rack, search for blank
                Tile t= OpponentRack.RemoveAndReturnTile('?');
                t.SetLetter(wordVector[ArrayIndex].letter[0]);
                t.setPosition(row, col);
                ActualPlay.push_back(t);
            }
            else{
                ActualPlay.push_back(t);
            }
            
			ArrayIndex++;
		}
		else if (boardTiles[row][col]->GetLetter() != '0'){ //letter on board, take it 
			ActualPlay.push_back(*boardTiles[row][col]);
		}
		else if (ArrayIndex >= (int)wordVector.size()) //new letters are drawn and board is empty 
			break;
		
        else { //there are still new tiles, but an empty tile was left between existing tiles (gap was found)
            valid = false;
            break;
        }
		
        if(Horizontal){
            col++;
        }
        else{
            row++;
        }
        if (col>14 || row> 14){
            if (ArrayIndex < (int)wordVector.size()){ //if there are still new tiles
                valid= false;
            }
            break;
        }
    }
    
    if (!valid){
        Play p;
        p.SetStartPos(-1, -1);
        return p;
    }

    Play p;
    p.SetTiles(ActualPlay);
    ActualPlay[0].GetIndex(row, col);
    p.SetStartPos(row, col);
    p.SetHorizontal(Horizontal);
    return p;
}

void HumanMode::AddPlayToBoard(Play p, Tile* boardTiles[15][15]){
    bool Horizontal = p.GetIsHorizontal();
    vector <Tile> tiles = p.GetTiles();
    int row = p.GetRow();
    int col= p.GetColumn();

    for (int i=0; i< (int) tiles.size(); i++){
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
    int row = p.GetRow();
    int col = p.GetColumn();
    for (int i=0; i< (int) tiles.size(); i++){
        char letter= tiles[i].GetLetter();
        int score= tiles[i].GetScore();
        

        board_->Probe(letter, row, col, score);

        int Prevsize= rack.GetLength();
        rack.RemoveTile(letter);
        int Newsize= rack.GetLength();
        if (Newsize == Prevsize){ //letter wasn't found, search for blank
        rack.RemoveTile('?');
        }
        if (p.GetIsHorizontal()){
            col++;
        }
        else{
            row++;
        }
    }
}

AgentMove HumanMode::MoveToGui(Move move){

    string moveToGui="";
    for (int i = 0; i < (int)move.GetPlay()->GetLetters().size(); i++)
    {
        moveToGui+=move.GetPlay()->GetLetters()[i];
    }
    AgentMove aMove;
    aMove.tiles = moveToGui;
    aMove.col=move.GetPlay()->GetColumn();
    aMove.row = move.GetPlay()->GetRow();
    aMove.score = move.GetPlay()->GetScore();
    aMove.dir = move.GetPlay()->GetIsHorizontal();
    return aMove;
}

 bool HumanMode::CheckGameOver(bool MyMoves, bool OppMoves){
    if (board_->GetCount() == 100)
        return true;

    if (this->rack_->GetLength() == 0 || this->opponentRack_->GetLength() == 0)    
        return true;

    if (bag_->GetRemainigLetters().size() == 0 && MyMoves == 0 && OppMoves == 0){//end game
        return true;
    }    
    return false;
 }

 Move HumanMode::GetPassMove(){
    Play *passPlay = new Play("",0,0,false);
    Move passMove;
    passPlay->SetScore(0);
    passMove.SetRack(*this->rack_);
    passMove.SetPlay(passPlay);
    passMove.SetPenalty(0);
    passMove.SetRackLeave(CalculateLeave(passMove.GetRack()));
    passMove.SetHeuristic();
    passMove.CalculateScore();
    return passMove;
}

double HumanMode::CalculateLeave(string rack)
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
				rackLeave += (*doubleValued_)[leave];
            }
        }
    }
	return rackLeave;
}
Move HumanMode::EndGame(vector<Move> moves, map<string, double> * syn2, map<char, double> * worth, MoveGenerator * movGen,Rack MyRack, int numTilesByOpponent)
{

       vector <char> remLetters= this->bag_->GetRemainigLetters();
       EndgameEvaluator* Eval = NULL;
       Eval = new EndgameEvaluator(moves,*board_,syn2,worth,movGen,MyRack);
       
       vector<Move> * evaluatedMoves = Eval->Evaluate();
   
		SimulationEndGame * s =new SimulationEndGame(*movGen,*board_);
		 int scoreMy=100;
		 int scoreop=120;
         Rack Rackopp=Eval->GetOppRack();
        //s->LookAhead(3,0,MyRack,Rackopp,*this->board_,0,0,Eval);
		 Move *m= s->LookAhead(3,0,MyRack,Rackopp,*board_,scoreMy,scoreop,Eval);
        return *m;

}

AgentMove HumanMode::PassMoveToGui(){
    AgentMove SentMove;
	SentMove.dir= -1;
	SentMove.row = -1;
	SentMove.col= -1;
	SentMove.score = -1;
	SentMove.dir= -1;
	SentMove.tiles = "";
    return SentMove;
}