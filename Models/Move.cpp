#include "Move.h"



Move::Move()
{
}



Move::Move(Board &board, bool horizontal)
{
	string word;
	ishorizontal_ = horizontal;
	for (int i = 0; i < (int)plays_.size(); i++)
	{
		word[i] = plays_.at(i).GetLetter();
	}
	board.UpdateBoard(word,plays_.at(0).GetRow(),plays_.at(0).GetColumn(),ishorizontal_);
}

void send_Row(Board &board)
{
	
	for (int i=0;i<15; i++)
	{
		//First will send entire row, then entire row - first pattern, then entire row - (first+second) patterns.. and so on till end of row..
		int patternSize=0;
		
		vector<string> possibleWords; //Vector that will carry possible combinations after we have it returned from GADDAG

		for (int j=0;j<15;j++)
		{
			while (board.tiles_[i][j]!="0")
			{
				patternSize++;
				j++;	
			}

			if (patternSize!=0)
			{
				int hookIndex=j-patternSize;
				int reverseIter=hookIndex-2;
				int hookSize=15-hookIndex;
				
				Tile * hook= new Tile[hookSize];   
				
				//Now we fill out the hook to be sent
				for (int k=0;k<hookSize;k++)
				{
					hook[k]=board.tiles_[i][k+hookIndex];
				}
				
				if (reverseIter==-1) //We have reached the beginning of the row (First pattern special case)
				{
					//We are sending the entire row. Therefore, the hook will be the entire row...
					//send_to_GADDAG(hook,rack_,0,15,possibleWords) 
					delete []hook;
				}
				
				//A previous pattern --->    _ _ a r r _ _ b a _ _ _ _ _ _ -->here we reach the 'r' at pos 4...
				else if (board.tiles_[i][reverseIter]!="0")    
				{
					//send_to_GADDAG(hook,rack_,reverseIter+2,15-hookIndex,possibleWords)
					delete []hook;
				}
				
				//Here is the general case, we are moving backingwards without hitting a previous pattern yet or reaching start of row
				else if (board.tiles_[i][reverseIter]=="0")
				{
					
					while (board.tiles_[i][reverseIter]=="0"&&revereIter>0)
					{
						reverseIter--;
					}

					if (reverseIter==0)							//Reached the begining of the row
					{
						//We are sending the entire row. Therefore, the hook will be the entire row...
						//send_to_GADDAG(hook,rack_,0,15,possibleWords)   
						
					}
					else										//Reached a previous pattern 
					{
						//send_to_GADDAG(hook,rack_,reverseIter+2,15-hookIndex,possibleWords)
					}
					delete []hook;		
				}
				
				patternSize=0;
			}
		}
	}
}

void Move::UpdateRack(string newRack)
{
	for (int i = 0; i < (int)newRack.size(); i++) {
		this->rack_[i] = newRack[i];
	}
}

void Move::InitializeRack(char rack[7])
{
	for (int j = 0; j < 7; j++)
	{
		this->rack_[j] = rack[j];
	}
}

void Move::SetPlays(vector<Tile> letters, bool horizontal)
{
	ishorizontal_ = horizontal;
	
	for (int i = 0; i < (int)letters.size(); i++)
	{		
		Tile t = letters.at(i);
		int row;
		int column;
		t.GetIndex(row, column);
		Play p = Play(t,row,column);
		plays_.push_back(p);
	}
}

vector<Play> Move::GetPlays()
{
	return plays_;
}

void Move::GetRack(char rack [7])
{
	for (int j = 0; j < 7; j++)
	{
		rack[j] = this->rack_[j];
	}
}

int Move::GetScore()
{
	return score_;
}


void Move::SetScore(int score)
{
	score_ = score;
}

void Move::CalculateScore()
{
	//To be implemented in static evaluation period.

}

Move::~Move()
{
}
