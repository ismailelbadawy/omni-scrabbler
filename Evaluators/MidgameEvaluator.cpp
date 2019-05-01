#include "MidgameEvaluator.h"
#include <algorithm>

double MidgameEvaluator::Evaluate(Move * move)
{
    return move->GetPlay()->GetScore();
}

MidgameEvaluator::MidgameEvaluator()
{

}

double MidgameEvaluator::CalculatePenalty()
{
    for(int i = 0; i < (int)(possibleMoves_->size()); i++)
    {
        Move* move = &(*possibleMoves_)[i];
        
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
                    bonus = this->GetBonus(i,columnIterator);
                }
                else
                {
                    bonus = this->GetBonus(rowIterator,i);
                }
                switch (bonus)
                {
                    case 5:

                        twCount += (5.0/abs(initialPosition-i));
                        break;
                    case 4: 
                        dwCount +=(4.0/abs(initialPosition-i));
                        break;
                    case 3:
                        tlCount+=(2.0/abs(initialPosition-i));
                        break;
                    case 2:
                        dlCount+=(1.0/abs(initialPosition-i));
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
        string rack = move->GetRack();
		if (rack.length() == 0)
		{
			move->SetRackLeave(0);
		}else if(rack.length() == 1)
        {
			move->SetRackLeave((*singleValued_)[rack[0]]);
		}
		else 
		{
			move->SetRackLeave(this->CalculateLeave(rack));
		}
		move->CalculateScore();
        move->SetHeuristic();
        cout << "Move (" << i << ") has penalty of : " << move->GetPenalty() << " rack leave of : " << move->GetRackLeave() << " score of : " << move->GetScore() << " Total : " << move->GetHeuristic() << endl;
        // Must be called after the rack leave and 
        
        }
    }
    return 0.0;
}
  
int MidgameEvaluator::GetBonus(int rowIterator, int columnIterator){
    if (!this->board_->IsPositionEmpty(rowIterator,columnIterator)){
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


double MidgameEvaluator::CalculateLeave(string rack)
{
	double rackLeave = 0.0;
    for(int i = 0; i < (int)rack.length(); i++)
    {
        for(int j = i + 1; j < (int)rack.length(); j++)
        {
            // Rack
            if(i != j)
            {
				char leaveValue[] = { rack[i] == '?' ? '?': rack[i] - 32, rack[j] == '?' ? '?' : rack[j] - 32};
				string leave(leaveValue);
				leave = leave.substr(0, 2);
				rackLeave += (*doubleValued_)[leave];
            }
        }
    }
	return rackLeave;
}

MidgameEvaluator::MidgameEvaluator(vector<Move>* moves, Board *board, map<string, double> * rackLeave, map<char, double> * charValue){
  
    this->board_ = board;
    this->possibleMoves_ = moves;
    this->doubleValued_ = rackLeave;
	this->singleValued_ = charValue;
	this->CalculatePenalty();
}

vector<Move>* MidgameEvaluator::Evaluate()
{
    // Should be called only after instantiation.
    vector<Move> * weReturn = new vector<Move>();
    // So it's safe to sort the moves.
    std::sort(possibleMoves_->begin(), possibleMoves_->end());
    for(int i = 0; i < 25; i++)
    {
        weReturn->push_back(possibleMoves_->at(i));
    }
    return weReturn;
}


MidgameEvaluator::~MidgameEvaluator()
{

}