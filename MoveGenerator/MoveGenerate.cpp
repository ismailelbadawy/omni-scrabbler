#include"MoveGenerate.h"

MoveGenerate::MoveGenerate(){

}

MoveGenerate::MoveGenerate(string gaddagpath){
	dag=new GADDAG(gaddagpath);
	rack_="";
}
void MoveGenerate::set_Rack(Rack gameRack){
	for (int i=0;i<7;i++)
	{
		rack_+=gameRack.GetLetter(i);
	}
}

bool MoveGenerate::check_other_dimension(Board board,string word,int row,int col,bool horizontal) //True when called from send_Row
{
	Tile boardTiles[15][15];
	board.GetTiles(boardTiles);
	if (horizontal)  //word is horizontal, we need to check the columns of the letters
	{
																//_ _ _ _ _ _|c|_ _ _ _ _ _ _ _			-->>Here we want to insert "ay" after
																//_ _ _ a r r|_|_ _ _ _ _ _ _ _  		"arr", but at col 6 we have to check
		for (int i=0;i<(int)word.size();i++)							//_ _ _ _ _ _|t|_ _ _ _ _ _ _ _ 		if inserting "a" is ok. "cat" is ok!!
		{	
			int startIndex=0;
			int endIndex=0;										//Index that will get us the word to test using the GAD-DAG
			int rowIterUp=0,rowIterDown=0;						//To up or down;
			
			if(row!=0)											//We are at the top row...
			{
				while (boardTiles[row-rowIterUp][col+i].GetLetter()!='0')
				{
					rowIterUp++;
					if (row-rowIterUp==0)						//We must check so that we will not access an out of bound memory
						break;
				}
			}
			if (row!=14)										//We are at the bottow row...
			{
				while (boardTiles[row+rowIterDown][col+i].GetLetter()!='0')
				{
					rowIterDown++;
					if (row+rowIterDown==14)					//We must check so that we will not access an out of bound memory
						break;
				}
			}
			
			startIndex=row-rowIterUp;
			endIndex=row+rowIterDown;
			
			char * arr= new char[endIndex-startIndex+1];
			
			for (int iter=0;iter<endIndex-startIndex+1;iter++)
			{
				arr[iter]=boardTiles[startIndex+iter][col+i].GetLetter();
			}	
			
			string toTest(arr);
			
			delete [] arr;
				
			if (!dag->CheckWordInDict(toTest))
				return false;
		}
		return true;
	}
	
	else 	//word is vertical, we need to check the row of the letters
	{
																//_ _ _ _ _ _ c _ _ _ _ _ _ _ _			-->>Here we want to insert "a" between
																//_ _ _|a|t|t|_|c|k|_ _ _ _ _ _  		"c" and "t", but at row 1 we have to 
		for (int i=0;i<(int)word.size();i++)							//_ _ _ _ _ _ t _ _ _ _ _ _ _ _ 		check if inserting "a" is ok.
																										//"attack" is ok!!
		{			
			int startIndex=0;
			int endIndex=0;										//Index that will get us the word to test using the GAD-DAG
			int colIterLeft=0,colIterRight=0;					//To up or down;
			
			if(col!=0)											//We are at the top row...
			{
				while (boardTiles[row+i][col-colIterLeft].GetLetter()!='0')
				{
					colIterLeft++;
					if (col-colIterLeft==0)						//We must check so that we will not access an out of bound memory
						break;
				}
			}

			if (col!=14)										//We are at the bottow row...
			{
				while (boardTiles[row+i][col+colIterRight].GetLetter()!='0')
				{
					colIterRight++;
					if (col+colIterRight==14)					//We must check so that we will not access an out of bound memory
						break;
				}
			}
			
			startIndex=col-colIterLeft;
			endIndex=col+colIterRight;
			
			char * arr= new char[endIndex-startIndex+1];
			
			for (int iter=0;iter<endIndex-startIndex+1;iter++)
			{
				arr[iter]=boardTiles[col+i][startIndex+iter].GetLetter();
			}	
			
			string toTest(arr);

			delete [] arr;
							
			if (!dag->CheckWordInDict(toTest))
				return false;
		}

		return true;

	}
}
//This function will add the possible moves to the play vector and discard the rest
//Row & Col will give the index of the hook
vector<Play> MoveGenerate::check_words(Board&board,vector<pair<string,vector<int> > > returnedWords,int row,int col,bool horizontal)	//returnedWords--> words from GAD-DAG
{
vector<Play> possiblePlays;
	for (int i=0;i<(int)returnedWords.size();i++)
	{

		if (horizontal)
		{
			for (int j=0;j<(int)returnedWords[i].second.size();j++)
			{
			
				if (check_other_dimension(board,returnedWords[i].first,row,returnedWords[i].second[j], horizontal))
				{
				Play P(returnedWords[i].first,row,returnedWords[i].second[j],horizontal);
				possiblePlays.push_back(P);
				}
						
			}
		}	
		else
		{
			for (int j=0;j<(int)returnedWords[i].second.size();j++)
			{
				if (check_other_dimension(board,returnedWords[i].first,returnedWords[i].second[j],col, horizontal))
				{
					Play P(returnedWords[i].first,returnedWords[i].second[j],col,horizontal);
					possiblePlays.push_back(P);
				}
			}
		}
	}
return possiblePlays;
}


void MoveGenerate::send_Row(Board &board)
{
	Tile boardTiles[15][15];
	board.GetTiles(boardTiles);

	for (int i=0;i<15; i++)
	{
		//First will send entire row, then entire row - first pattern, then entire row - (first+second) patterns.. and so on till end of row..
		int patternSize=0;
		
		vector< pair <string,vector<int> > > possibleWords; //Vector that will carry possible combinations after we have it returned from GADDAG

		for (int j=0;j<15;j++)
		{
			while (boardTiles[i][j].GetLetter()!='0')
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
				
				char * hook= new char[hookSize];   			//The hook to be send in each iteration
				string hookString="";
				//Now we fill out the hook to be sent		//
				for (int k=0;k<hookSize;k++)
				{
					if (boardTiles[i][k+hookIndex].GetLetter()=='0')
					{	
						hook[k]='.';
						hookString+='.';
					}
					else {hook[k]=boardTiles[i][k+hookIndex].GetLetter();
					hookString+=boardTiles[i][k+hookIndex].GetLetter();
				}
				}
				
				if (reverseIter==-1) //We have reached the beginning of the row (First pattern special case)
				{
					//We are sending the entire row. Therefore, the hook will be the entire row...
					//possibleWords=send_to_GADDAG(hook,rack_,1,1) 					
					possibleWords=dag->ContainsHookWithRackAtPos(hookString,rack_,1,1);
				}
				
				//A previous pattern --->    _ _ a r r _ _ b a _ _ _ _ _ _ -->here we reach the 'r' at pos 4...
				else if (boardTiles[i][reverseIter].GetLetter()!='0')    
				{
					//possibleWords=send_to_GADDAG(hook,rack_,hookIndex-(reverseIter+2),hookIndex)
					possibleWords=dag->ContainsHookWithRackAtPos(hookString,rack_,hookIndex-(reverseIter+2),hookIndex);
				}
				
				//Here is the general case, we are moving backingwards without hitting a previous pattern yet or reaching start of row
				else if (boardTiles[i][reverseIter].GetLetter()=='0')
				{
					
					while (boardTiles[i][reverseIter].GetLetter()=='0'&&reverseIter>0)
					{
						reverseIter--;
					}

					if (reverseIter==0)							//Reached the begining of the row
					{
						//We are sending the entire row. Therefore, the hook will be the entire row...
						//possibleWords=send_to_GADDAG(hook,rack_,hookIndex-1,hookIndex)   			
						possibleWords=dag->ContainsHookWithRackAtPos(hookString,rack_,hookIndex-1,hookIndex);
					}
					else										//Reached a previous pattern 
					{
						//possibleWords=send_to_GADDAG(hook,rack_,hookIndex-(reverseIter+2),hookIndex)
						possibleWords=dag->ContainsHookWithRackAtPos(hookString,rack_,hookIndex-(reverseIter+2),hookIndex);
					}

				}

				delete []hook;

				//Here we are testing the words we received from the GAD-DAG
				check_words(board,possibleWords,i,-1,true);
				patternSize=0;
			}
		}
	}
}


void MoveGenerate::send_Col(Board &board)
{
	Tile boardTiles[15][15];
	board.GetTiles(boardTiles);

	for (int i=0;i<15; i++)
	{
		//First will send entire col, then entire col - first pattern, then entire col - (first+second) patterns.. and so on till end of col..
		int patternSize=0;
		
		vector< pair <string,vector<int> > > possibleWords; //Vector that will carry possible combinations after we have it returned from GADDAG

		for (int j=0;j<15;j++)
		{
			while (boardTiles[j][i].GetLetter()!='0')
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
				
				char * hook= new char[hookSize];   			//Hook to be sent
				
				//Now we fill out the hook to be sent
				for (int k=0;k<hookSize;k++)
				{
					hook[k]=boardTiles[k+hookIndex][i].GetLetter();
				}
				
				if (reverseIter==-1) //We have reached the beginning of the row (First pattern special case)
				{
					//We are sending the entire row. Therefore, the hook will be the entire row...
					//possibleWords=send_to_GADDAG(hook,rack_,1,1) 
					possibleWords=dag->ContainsHookWithRackAtPos(hook,rack_,1,1);
					delete []hook;
					
				}
			
				//A previous pattern --->    _ _ a r r _ _ b a _ _ _ _ _ _ -->here we reach the 'r' at pos 4...
				else if (boardTiles[reverseIter][i].GetLetter()!='0')    
				{
					//possibleWords=send_to_GADDAG(hook,rack_,hookIndex-(reverseIter+2),hookIndex)
					possibleWords=dag->ContainsHookWithRackAtPos(hook,rack_,hookIndex-(reverseIter+2),hookIndex);
					delete []hook;
				
				}
				
				//Here is the general case, we are moving backingwards without hitting a previous pattern yet or reaching start of row
				else if (boardTiles[reverseIter][i].GetLetter()=='0')
				{
					
					while (boardTiles[reverseIter][i].GetLetter()=='0'&&reverseIter>0)
					{
						reverseIter--;
					}

					if (reverseIter==0)							//Reached the begining of the row
					{
						//We are sending the entire row. Therefore, the hook will be the entire row...
						//possibleWords=send_to_GADDAG(hook,rack_,hookIndex-1,hookIndex)   			
						possibleWords=dag->ContainsHookWithRackAtPos(hook,rack_,hookIndex-1,hookIndex);
					}
					else										//Reached a previous pattern 
					{
						//possibleWords=send_to_GADDAG(hook,rack_,hookIndex-(reverseIter+2),hookIndex)
						possibleWords=dag->ContainsHookWithRackAtPos(hook,rack_,hookIndex-(reverseIter+2),hookIndex);
					}

					//Here we are testing the words we received from the GAD-DAG
					check_words(board,possibleWords,-1,i,true);
					delete []hook;
				}
				
				patternSize=0;
			}
		}
	}
}

MoveGenerate::~MoveGenerate(){
	
}
