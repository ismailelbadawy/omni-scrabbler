#include "EndgameEvaluator.h"
#include <algorithm>

double EndgameEvaluator::CalculatePenalty(Move *move, Board board)
{

    int rowIterator = move->GetPlay()->GetRow();
    int columnIterator = move->GetPlay()->GetColumn();
    // Check if the move is horizontal
    // then calculate the penalty on the vertical columns
    // else calculate the penalty on the horizontal rows

    double twCount = 0;
    double dwCount = 0;
    double dlCount = 0;
    double tlCount = 0;
    bool isHorizontal = move->GetPlay()->GetIsHorizontal();
    // loop on each word to check how near or far any bonus is to it
    for (Tile tile : move->GetPlay()->GetTiles())
    {
        int lowerBound = 0;
        int upperBound = 0;
        int initialPosition = 0;

        if (isHorizontal)
        {
            lowerBound = rowIterator - 7 >= 0 ? rowIterator - 7 : 0;
            upperBound = rowIterator + 7 <= 14 ? rowIterator + 7 : 14;
            initialPosition = move->GetPlay()->GetRow();
        }
        else
        {
            lowerBound = columnIterator - 7 >= 0 ? columnIterator - 7 : 0;
            upperBound = columnIterator + 7 <= 14 ? columnIterator + 7 : 14;
            initialPosition = move->GetPlay()->GetColumn();
        }

        for (int i = lowerBound; i < upperBound && i != initialPosition; i++)
        {
            int bonus = 0;
            if (isHorizontal)
            {
                bonus = this->GetBonus(i, columnIterator, board);
            }
            else
            {
                bonus = this->GetBonus(rowIterator, i, board);
            }
            switch (bonus)
            {
            case 5:

                twCount += (15.0 / abs(initialPosition - i));
                break;
            case 4:
                dwCount += (12.0 / abs(initialPosition - i));
                break;
            case 3:
                tlCount += (6.0 / abs(initialPosition - i));
                break;
            case 2:
                dlCount += (3.0 / abs(initialPosition - i));
                break;
            default:
                break;
            }
        }
        if (isHorizontal)
        {
            columnIterator++;
        }
        else
        {
            rowIterator++;
        }
        move->SetPenalty(twCount + tlCount + dwCount + dlCount);

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
        
        }
         return 0.0;
    }
   

double EndgameEvaluator::CalculateLeave(string rack)
{
    double rackLeave = 0.0;
    for (int i = 0; i < (int)rack.length(); i++)
    {
        for (int j = i + 1; j < (int)rack.length(); j++)
        {
            // Rack
            if (i != j)
            {
                char leaveValue[] = {rack[i] == '?' ? '?' : (rack[i] - 32), rack[j] == '?' ? '?' : (rack[j] - 32)};
                string leave(leaveValue);
                leave = leave.substr(0, 2);
                rackLeave += (*doubleValued_)[leave];
            }
        }
    }
    return rackLeave;
}

int EndgameEvaluator::GetBonus(int rowIterator, int columnIterator, Board board)
{
    if (!board.IsPositionEmpty(rowIterator, columnIterator))
    {
        return 0;
    }
    if ((rowIterator == 7 && (columnIterator == 0 || columnIterator == 14)) ||
        ((rowIterator == 0 || rowIterator == 14) && (columnIterator == 0 || columnIterator == 7 || columnIterator == 14)))
    {
        return 5;
    }
    // Double Word
    else if (((rowIterator == columnIterator) && (rowIterator < 5 || rowIterator > 9) && (rowIterator != 14 && rowIterator != 0)) ||
             ((14 - rowIterator == columnIterator) && (columnIterator < 5 || columnIterator > 9) && (columnIterator != 14 && columnIterator != 0)))
    {
        return 4;
    }
    // Triple Letter
    else if (((rowIterator == 1 || rowIterator == 13) && (columnIterator == 5 || columnIterator == 9)) ||
             (((rowIterator == 5 || rowIterator == 9) && (columnIterator == 1 || columnIterator == 13 || columnIterator == 5 || columnIterator == 9))))
    {
        return 3;
    }
    // Double Letter
    else if (((rowIterator == 0 || rowIterator == 14) && (columnIterator == 3 || columnIterator == 11)) ||
             ((rowIterator == 2 || rowIterator == 12) && (columnIterator == 6 || columnIterator == 8)) ||
             ((rowIterator == 3 || rowIterator == 11) && (columnIterator == 0 || columnIterator == 7 || columnIterator == 14)) ||
             ((rowIterator == 6 || rowIterator == 8) && (columnIterator == 2 || columnIterator == 6 || columnIterator == 8 || columnIterator == 12)) ||
             ((rowIterator == 7) && (columnIterator == 3 || columnIterator == 11)))
    {
        return 2;
    }

    return 0;
}

Rack EndgameEvaluator::EvaluateRack()
{
    int Alphabet[27] = {9, 2, 2, 4, 12, 3, 3, 2, 9, 1, 1, 4, 2, 6, 8, 2, 1, 6, 5, 6, 4, 2, 2, 1, 2, 1};
    Tile *Tiles[15][15];
    board_->GetTiles(Tiles);
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (Tiles[i][j]->GetLetter() != '0')
            {
                Alphabet[Tiles[i][j]->GetLetter() - 97]--;
            }
        }
    }
    for (int i = 0; i < myRack.GetLength(); i++)
    {
        Alphabet[myRack.GetLetter(i) - 97]--;
    }
    string RemainingLetters;
    int counter = 0;

    Tile RackTiles[7];

    for (int i = 0; i < 26; i++)
    {
        while (Alphabet[i] != 0)
        {

            char c = (char)(97 + i);
            RackTiles[counter].SetLetter((char)(97 + i));
            RackTiles[counter].SetScore(1);

            counter++;
            Alphabet[i]--;
        }
    }
    vector<Tile> rackTiles(RackTiles, RackTiles + sizeof RackTiles / sizeof RackTiles[0]);
    Rack red(rackTiles);
    return red;
}


EndgameEvaluator::EndgameEvaluator(vector<Move> moves, Board *board, map<string, double> * rackLeave, map<char, double> * charValue)
{
    this->board_ = board;
    this->possibleMoves_ = moves;
    this->doubleValued_ = rackLeave;
	this->singleValued_ = charValue;
 
	
}
EndgameEvaluator::~EndgameEvaluator()
{
}
double EndgameEvaluator::Evaluate(Move *move)
{
    return move->GetPlay()->GetScore();
}
vector<Move> *EndgameEvaluator::Evaluate()
{

      

    for(int i=0;i< (int) possibleMoves_.size();i++)
{
    possibleMoves_[i].SetPenalty(this->CalculatePenalty(&possibleMoves_[i],*this->board_));
    possibleMoves_[i].SetRackLeave(this->CalculateLeave(possibleMoves_[i].GetRack()));
    possibleMoves_[i].GetPlay()->SetScore(possibleMoves_[i].GetPlay()->GetScore()+(1778*possibleMoves_[i].GetRack().length()));
    possibleMoves_[i].CalculateScore();
}
// Should be called only after instantiation.
    vector<Move> * weReturn = new vector<Move>();
    // So it's safe to sort the moves.
    std::sort(possibleMoves_.begin(), possibleMoves_.end());
    for(int i = 0; i < 25; i++)
    {
        weReturn->push_back(possibleMoves_.at(i));
    }
    return weReturn;


}