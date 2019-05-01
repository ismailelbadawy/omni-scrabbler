#include"PreendgameEvaluator.h"

PreendgameEvaluator::PreendgameEvaluator(){

}
PreendgameEvaluator::~PreendgameEvaluator(){

}

double PreendgameEvaluator::Evaluate(Move * move)
{
    return this->ComputeBestMove().GetScore();
}

PreendgameEvaluator::PreendgameEvaluator(map<string, double> *rackLeave,Board* board, MoveGenerator * movGen,vector<Move> possibleMoves){
    this->doubleValued_ = rackLeave;
    this->board_ = board;
    this->movegenerator_ = movGen;
    this->possiblemoves_ = possibleMoves;
    this->remainingletters_ = {'a','b','d','a','e','c','h','h','x','p'}; //for testing only
    this->numTilesByOpponent_ = 3; //for testing only
}

double PreendgameEvaluator::CalculateLeave(string rack)
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

void PreendgameEvaluator:: Combination(int start, int size, string &rem, string &temprack, double &MaxScore, string &BestRackLeave){
    if (size == 0) {
		//double rackleave_score = rackleavefn(temprack)
        double rackleave_score = CalculateLeave(temprack);
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
		Combination(i + 1, size - 1, rem, temprack, MaxScore, BestRackLeave);
		temprack.pop_back();
	}

}

void PreendgameEvaluator::OpponentRackEstimation(){
    //vector<char> remainingletters = remainingletters_; //take a copy of remainingletters_
    string remainingletters= "";
    for (int i=0; i< (int)remainingletters_.size(); i++){
        remainingletters += remainingletters_[i];
    }
    
    int TilesLeftOponent = 7 - numTilesByOpponent_; //if opponent played 3 letters in the last turn, there are 4 letters left to guess
    double MaxRackLeave=0.0;

    //vector<char> tempRack, OpponentRack;
    string tempRack, OpponentRack;
  
    //get first part of oponent rack based on his last move
    Combination(0, TilesLeftOponent, remainingletters, tempRack, MaxRackLeave, OpponentRack);

    //remove from remainingletters letters picked in opponent Rack
    for (int i=0; i< (int)OpponentRack.size(); i++){
        remainingletters.erase(std::remove(remainingletters.begin(), remainingletters.end(), OpponentRack[i]), remainingletters.end());
    } 

    //get second part of oponent rack randomly
    for (int i = 0; i < numTilesByOpponent_; i++) {
		int randomIndex = rand() % remainingletters.size();
		OpponentRack.push_back(remainingletters[randomIndex]);	
		remainingletters.erase(remainingletters.begin()+randomIndex);

	}

    string OppRackString ="";
    for (int i=0; i<(int)OpponentRack.size(); i++){
        OppRackString += OpponentRack[i];
    }
    Rack rack(OppRackString);
    this->enemyrack_ = rack;
}

double PreendgameEvaluator::CalculatePenalty(Move * move, Board board)
{

        
        int rowIterator = move->GetPlay()->GetRow() ;
        int columnIterator = move->GetPlay()->GetColumn();
        // Check if the move is horizontal
        // then calculate the penalty on the vertical columns
        // else calculate the penalty on the horizontal rows
       
        double twCount = 0;
        double dwCount=0;
        double dlCount=0;
        double tlCount=0;
        bool isHorizontal = move->GetPlay()->GetIsHorizontal();
        // loop on each word to check how near or far any bonus is to it
        for(Tile tile : move->GetPlay()->GetTiles())
        {
            int lowerBound = 0;
            int upperBound = 0;
            int initialPosition = 0;
            
            if (isHorizontal)
            {
                lowerBound = rowIterator-7 >= 0?rowIterator-7:0;
                upperBound = rowIterator+7 <= 14?rowIterator+7:14;
                initialPosition = move->GetPlay()->GetRow();
            }
            else
            {
                lowerBound = columnIterator-7 >= 0?columnIterator-7:0;
                upperBound = columnIterator+7 <= 14?columnIterator+7:14;
                initialPosition = move->GetPlay()->GetColumn();

            }

            for (int i = lowerBound; i < upperBound && i != initialPosition; i++)
            {
                int bonus=0;
                if (isHorizontal)
                {
                    bonus = this->GetBonus(i,columnIterator, board);
                }
                else
                {
                    bonus = this->GetBonus(rowIterator,i, board);
                }
                switch (bonus)
                {
                    case 5:

                        twCount += (15.0/abs(initialPosition-i));
                        break;
                    case 4: 
                        dwCount +=(12.0/abs(initialPosition-i));
                        break;
                    case 3:
                        tlCount+=(6.0/abs(initialPosition-i));
                        break;
                    case 2:
                        dlCount+=(3.0/abs(initialPosition-i));
                        break;
                    default:
                        break;
                }

            }
            if (isHorizontal)
            {
            columnIterator++;
            }
            else{
                rowIterator++;
            }
        move->SetPenalty(twCount+tlCount+dwCount+dlCount);
        // Let's sort this array

        }
    return 0.0;
}
  
int PreendgameEvaluator::GetBonus(int rowIterator, int columnIterator, Board board)
{
    if (!board.IsPositionEmpty(rowIterator,columnIterator)){
        return 0;
    }
	if ((rowIterator==7&&(columnIterator == 0 ||columnIterator == 14))|| 
			 ((rowIterator== 0 || rowIterator == 14)&&(columnIterator==0 || columnIterator==7 || columnIterator==14)))
			{
				return 5;
			}
			// Double Word
			else if (((rowIterator == columnIterator)&&(rowIterator<5 || rowIterator>9)&&(rowIterator!=14 && rowIterator!=0))||
			((14-rowIterator==columnIterator)&&(columnIterator<5||columnIterator>9)&&(columnIterator!=14 && columnIterator!=0))){
				return 4;
                }
			// Triple Letter 
			else if (((rowIterator==1||rowIterator==13)&&(columnIterator==5 || columnIterator==9))||
					(((rowIterator==5 ||rowIterator ==9) &&( columnIterator==1 || columnIterator == 13 ||columnIterator==5 || columnIterator==9))))
			{
				return 3;
			}
			// Double Letter
			else if 
				(((rowIterator == 0 || rowIterator == 14)&&(columnIterator == 3 || columnIterator ==11))||
				((rowIterator==2 || rowIterator == 12)&&(columnIterator==6 || columnIterator == 8))||
				((rowIterator==3 || rowIterator == 11)&&(columnIterator==0 || columnIterator==7 || columnIterator==14))||
				((rowIterator==6 || rowIterator == 8)&&(columnIterator==2 ||columnIterator==6 || columnIterator==8 || columnIterator==12))||
				((rowIterator == 7)&&(columnIterator == 3 || columnIterator==11)))
			{
				return 2;
			}

    return 0;
}

Move PreendgameEvaluator::OpponentBestMove(){

    vector<Move> opponentMoves = this->movegenerator_->Generate(&this->enemyrack_,*this->board_,false);
    Move* bestMove = NULL;
    double maxTotalScore  = 0;
    for (int i = 0; i <(int)opponentMoves.size(); i++)
    {
       opponentMoves[i].SetRackLeave(this->CalculateLeave(opponentMoves[i].GetRack()));
       opponentMoves[i].SetPenalty(this->CalculatePenalty(&opponentMoves[i],*this->board_));
       opponentMoves[i].CalculateScore();
       if (opponentMoves[i].GetScore()> maxTotalScore){
           maxTotalScore  = opponentMoves[i].GetHeuristic()+opponentMoves[i].GetPlay()->GetScore();
           bestMove = &opponentMoves[i];
       }

    }
    return *bestMove;
    
}

Move PreendgameEvaluator::ComputeBestMove()
{
    this->OpponentRackEstimation();
    Move opponentMove = this->OpponentBestMove();
    int opponentRow = opponentMove.GetPlay()->GetRow();
    int opponentColumn = opponentMove.GetPlay()->GetColumn();
    int opponentLength = opponentMove.GetPlay()->GetTiles().size();
    double maxScore = 0;
    Move * bestMove = NULL;
    for (int i = 0; i < (int) this->possiblemoves_.size(); i++)
    {
        possiblemoves_[i].SetPenalty(this->CalculatePenalty(&possiblemoves_[i],*this->board_));
        possiblemoves_[i].SetRackLeave(this->CalculateLeave(possiblemoves_[i].GetRack()));
        possiblemoves_[i].CalculateScore();
        int myRow = possiblemoves_[i].GetPlay()->GetRow();
        int myColumn = possiblemoves_[i].GetPlay()->GetColumn();
        int myLength =  possiblemoves_[i].GetPlay()->GetTiles().size();


        if((opponentMove.GetPlay()->GetIsHorizontal() && possiblemoves_[i].GetPlay()->GetIsHorizontal())) 
        {
            if (myRow == opponentRow){
                if ((opponentColumn<=myColumn) && 
                (opponentColumn+opponentLength>=myColumn) || 
                (myColumn<=opponentColumn)&&
                (myColumn+myLength>=opponentColumn)){
                    if (maxScore<=possiblemoves_[i].GetScore()){
                        maxScore = possiblemoves_[i].GetScore();
                        bestMove = &possiblemoves_[i];
                    }
                }
            }
        }
        else if(!opponentMove.GetPlay()->GetIsHorizontal() && possiblemoves_[i].GetPlay()->GetIsHorizontal()){
       if (myRow>=opponentRow && myRow<=opponentRow+opponentLength){
           if ((opponentColumn>=myColumn && myColumn+myLength>=opponentColumn)){
                if (maxScore<=possiblemoves_[i].GetScore()){
                        maxScore = possiblemoves_[i].GetScore();
                        bestMove = &possiblemoves_[i];
                    }
           }
       }
        }
        else if (opponentMove.GetPlay()->GetIsHorizontal() && !possiblemoves_[i].GetPlay()->GetIsHorizontal()){
        if (myColumn>=opponentColumn && myColumn<=opponentColumn+opponentLength){
           if ((opponentRow>=myRow && myRow+myLength>=opponentRow)){
                if (maxScore<=possiblemoves_[i].GetScore()){
                        maxScore = possiblemoves_[i].GetScore();
                        bestMove = &possiblemoves_[i];
                    }
           }
        }
        else if ((!opponentMove.GetPlay()->GetIsHorizontal() && !possiblemoves_[i].GetPlay()->GetIsHorizontal())) 
        { 
            if (myColumn == opponentColumn){
                if ((opponentRow<=myRow) && 
                (opponentRow+opponentLength>=myRow) || 
                (myRow<=opponentRow)&&
                (myRow+myLength>=opponentRow)){
                    if (maxScore<=possiblemoves_[i].GetScore()){
                        maxScore = possiblemoves_[i].GetScore();
                        bestMove = &possiblemoves_[i];
                    }
                }
            }
        }
        
    }
    if (bestMove == NULL)
    {
    for (int i = 0; i < (int) this->possiblemoves_.size(); i++)
    {
          if (maxScore<=possiblemoves_[i].GetScore())
        {
            maxScore = possiblemoves_[i].GetScore();
            bestMove = &possiblemoves_[i];
        } 
    }

}

}
return *bestMove;
}


vector<Move> * PreendgameEvaluator::Evaluate()
{
    vector<Move> * weReturn = new vector<Move>();
    std::sort(possiblemoves_.begin(), possiblemoves_.end());
    for(int i = 0; i < possiblemoves_.size(); i++)
    {
        weReturn->push_back(possiblemoves_.at(i));
    }
    return weReturn;
}