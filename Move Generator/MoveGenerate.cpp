#include<iostream>
#include<vector>
#include"GADDAG/GADDAG.h"
#include"Models/Board.h"
#include"Models/Move.h"
#include"Models/Rack.h"



bool check_other_dimension(Board board,string word,int row,int col,bool horizontal) //True when called from send_Row
{
	char boardTiles[15[15]];
	board.GetTiles(boardTiles);
	if (horizontal)  //word is horizontal, we need to check the columns of the letters
	{
																//_ _ _ _ _ _|c|_ _ _ _ _ _ _ _			-->>Here we want to insert "ay" after
																//_ _ _ a r r|_|_ _ _ _ _ _ _ _  		"arr", but at col 6 we have to check
		for (int i=0;i<word.size();i++)							//_ _ _ _ _ _|t|_ _ _ _ _ _ _ _ 		if inserting "a" is ok. "cat" is ok!!
		{	
			int startIndex=0;
			int endIndex=0;										//Index that will get us the word to test using the GAD-DAG
			int rowIterUp=0,rowIterDown=0;						//To up or down;
			
			if(row!=0)											//We are at the top row...
			{
				while (boardTiles[row-rowIterUp][col+i]!="0")
				{
					rowIterUp++;
					if (row-rowIterUp==0)						//We must check so that we will not access an out of bound memory
						break;
				}
			}
			if (row!=14)										//We are at the bottow row...
			{
				while (boardTiles[row+rowIterDown][col+i]!="0")
				{
					rowIterDown++;
					if (row+rowIterDown==14)					//We must check so that we will not access an out of bound memory
						break;
				}
			}
			
			startIndex=row-rowIterUp;
			endIndex=row+rowIterDown;
			
			char * arr= new char[endIndex-startIndex+1];
			
			for (int iter=0;iter<endInex-startIndex+1;iter++)
			{
				arr[iter]=boardTiles[startIndex+iter][col+i];
			}	
			
			string toTest(arr);

			delete []arr;

			//if (!test_string_GADDAG(toTest));
			//	return false;
		}
		return true;
	}
	
	else 	//word is vertical, we need to check the row of the letters
	{
																//_ _ _ _ _ _ c _ _ _ _ _ _ _ _			-->>Here we want to insert "a" between
																//_ _ _|a|t|t|_|c|k|_ _ _ _ _ _  		"c" and "t", but at row 1 we have to 
		for (int i=0;i<word.size();i++)							//_ _ _ _ _ _ t _ _ _ _ _ _ _ _ 		check if inserting "a" is ok.
																										//"attack" is ok!!
		{			
			int startIndex=0;
			int endIndex=0;										//Index that will get us the word to test using the GAD-DAG
			int colIterLeft=0,colIterRight=0;					//To up or down;
			
			if(col!=0)											//We are at the top row...
			{
				while (boardTiles[row+i][col-colIterLeft]!="0")
				{
					colIterLeft++;
					if (col-colIterLeft==0)						//We must check so that we will not access an out of bound memory
						break;
				}
			}

			if (col!=14)										//We are at the bottow row...
			{
				while (boardTiles[row+i][col+colIterRight]!="0")
				{
					colIterRight++;
					if (col+colIterRight==14)					//We must check so that we will not access an out of bound memory
						break;
				}
			}
			
			startIndex=col-colIterLeft;
			endIndex=col+colIterRight;
			
			char * arr= new char[endIndex-startIndex+1];
			
			for (int iter=0;iter<endInex-startIndex+1;iter++)
			{
				arr[iter]=boardTiles[col+i][startIndex+iter];
			}	
			
			string toTest(arr);

			delete []arr;
			//if (!test_string_GADDAG(toTest));
			//	return false;
		}

		return true;

	}
}

void check_words(Board board,vector<string> returnedWords,int row,int col,bool horizontal)	//returnedWords--> words from GAD-DAG
	//This function will add the possible moves to the play vector and discard the rest
{														//Row & Col will give the index of the hook
	for (int i=0;i<returnedWords.size();i++)
	{
		if (check_other_dimension(returnedWords[i],int row,int col, horizontal))
		{
			plays_.insert(returnedWords[i]);
		}
	}

}

void send_Row(Board &board)
{
	char boardTiles[15[15]];
	board.GetTiles(boardTiles);

	for (int i=0;i<15; i++)
	{
		//First will send entire row, then entire row - first pattern, then entire row - (first+second) patterns.. and so on till end of row..
		int patternSize=0;
		
		vector< pair <string,vector<int> > > possibleWords; //Vector that will carry possible combinations after we have it returned from GADDAG

		for (int j=0;j<15;j++)
		{
			while (boardTiles[i][j]!="0")
			{
				patternSize++;
				j++;
				if (j==15)
					break;	
			}

			if (patternSize!=0)								//Will be zero if row is empty
			{
				int hookIndex=j-patternSize;				//Begin index of current hook
				int reverseIter=hookIndex-2;				//An iterator to go back till we reach a previous pattern or begin of row
				int hookSize=15-hookIndex;					//Calculating the size of our hook
				
				Tile * hook= new Tile[hookSize];   			//The hook to be send in each iteration
				
				//Now we fill out the hook to be sent		//
				for (int k=0;k<hookSize;k++)
				{
					hook[k]=boardTiles[i][k+hookIndex];
				}
				
				if (reverseIter==-1) //We have reached the beginning of the row (First pattern special case)
				{
					//We are sending the entire row. Therefore, the hook will be the entire row...
					//possibleWords=send_to_GADDAG(hook,rack_,1,1) 
					int possibleWordsCount=possibleWords.size();
					int iter=0;
					delete []hook;
				}
				
				//A previous pattern --->    _ _ a r r _ _ b a _ _ _ _ _ _ -->here we reach the 'r' at pos 4...
				else if (boardTiles[i][reverseIter]!="0")    
				{
					//possibleWords=send_to_GADDAG(hook,rack_,hookIndex-(reverseIter+2),hookIndex)
					delete []hook;
				}
				
				//Here is the general case, we are moving backingwards without hitting a previous pattern yet or reaching start of row
				else if (boardTiles[i][reverseIter]=="0")
				{
					
					while (boardTiles[i][reverseIter]=="0"&&revereIter>0)
					{
						reverseIter--;
					}

					if (reverseIter==0)							//Reached the begining of the row
					{
						//We are sending the entire row. Therefore, the hook will be the entire row...
						//possibleWords=send_to_GADDAG(hook,rack_,hookIndex-1,hookIndex)   			
					}
					else										//Reached a previous pattern 
					{
						//possibleWords=send_to_GADDAG(hook,rack_,hookIndex-(reverseIter+2),hookIndex)
					}

					//Here we are testing the words we received from the GAD-DAG
					while (iter<possibleWordsCount)
					{
						for (int z=0;z<possibleWords[i].second.size();z++)
						{
							check_words(possibleWords[i].first,i,possibleWords[i].second[z],true);
						}
					}
					delete []hook;		
				}
				
				patternSize=0;
			}
		}
	}
}


void send_Col(Board &board)
{
	char boardTiles[15[15]];
	board.GetTiles(boardTiles);

	for (int i=0;i<15; i++)
	{
		//First will send entire col, then entire col - first pattern, then entire col - (first+second) patterns.. and so on till end of col..
		int patternSize=0;
		
		vector< pair <string,vector<int> > > possibleWords; //Vector that will carry possible combinations after we have it returned from GADDAG

		for (int j=0;j<15;j++)
		{
			while (boardTiles[j][i]!="0")
			{
				patternSize++;
				j++;	
				if (j==15)
					break;
			}

			if (patternSize!=0)
			{
				int hookIndex=i-patternSize;				//Begin index of our hook
				int reverseIter=hookIndex-2;				//The iterator to go back till we reach a previous pattern or coloumn begin
				int hookSize=15-hookIndex;					//Calculating the hook size
				
				Tile * hook= new Tile[hookSize];   			//Hook to be sent
				
				//Now we fill out the hook to be sent
				for (int k=0;k<hookSize;k++)
				{
					hook[k]=boardTiles[k+hookIndex][i];
				}
				
				if (reverseIter==-1) //We have reached the beginning of the row (First pattern special case)
				{
					//We are sending the entire row. Therefore, the hook will be the entire row...
					//possibleWords=send_to_GADDAG(hook,rack_,1,1) 
					int possibleWordsCount=possibleWords.size();
					int iter=0;
					delete []hook;
				}
				
				//A previous pattern --->    _ _ a r r _ _ b a _ _ _ _ _ _ -->here we reach the 'r' at pos 4...
				else if (boardTiles[reverseIter][i]!="0")    
				{
					//possibleWords=send_to_GADDAG(hook,rack_,hookIndex-(reverseIter+2),hookIndex)
					delete []hook;
				}
				
				//Here is the general case, we are moving backingwards without hitting a previous pattern yet or reaching start of row
				else if (boardTiles[reverseIter][i]=="0")
				{
					
					while (boardTiles[reverseIter][i]=="0"&&revereIter>0)
					{
						reverseIter--;
					}

					if (reverseIter==0)							//Reached the begining of the row
					{
						//We are sending the entire row. Therefore, the hook will be the entire row...
						//possibleWords=send_to_GADDAG(hook,rack_,hookIndex-1,hookIndex)   			
					}
					else										//Reached a previous pattern 
					{
						//possibleWords=send_to_GADDAG(hook,rack_,hookIndex-(reverseIter+2),hookIndex)
					}

					//Here we are testing the words we received from the GAD-DAG
					while (iter<possibleWordsCount)
					{
						for (int z=0;z<possibleWords[i].second.size();z++)
						{
							check_words(possibleWords[i].first,possibleWords[i].second[z],i,true);
						}
					}
					delete []hook;		
				}
				
				patternSize=0;
			}
		}
	}
}
