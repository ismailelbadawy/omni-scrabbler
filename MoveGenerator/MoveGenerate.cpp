#include"MoveGenerate.h"

MoveGenerate::MoveGenerate(){

}

MoveGenerate::MoveGenerate(string gaddagpath){
	dag_ = new GADDAG();
	rack_="";
}

void MoveGenerate::set_Rack(Rack gameRack){
	rack_="";
	for (int i=0;i<7;i++)
	{
		rack_+=gameRack.GetLetter(i);
	}
}

bool MoveGenerate::check_other_dimension(Board board,string word,int row,int col,bool horizontal) //True when called from generateWordsAtRows
{
	Tile boardTiles[15][15];
	board.GetTiles(boardTiles);
	if (horizontal)  //word is horizontal, we need to check the columns of the letters
	{
																//_ _ _ _ _ _|c|_ _ _ _ _ _ _ _			-->>Here we want to insert "ay" after
																//_ _ _ a r r|_|_ _ _ _ _ _ _ _  		"arr", but at col 6 we have to check
		for (int i=0;i<(int)word.size();i++)					//_ _ _ _ _ _|t|_ _ _ _ _ _ _ _ 		if inserting "a" is ok. "cat" is ok!!
		{	
			int otherWordStart=0;
			int otherWordEnd=0;										//Index that will get us the word to test using the GAD-dag_
			int rowIterUp=0,rowIterDown=0;						//To up or down;
			

			if (row!=0 && row!=14 && boardTiles[row-rowIterUp][col+i].GetLetter()=='0' &&
			boardTiles[row+rowIterDown][col+i].GetLetter()=='0'){
				return true;
			}

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
			if (boardTiles[row+rowIterDown][col+i].GetLetter()!='0' &&boardTiles[row-rowIterUp][col+i].GetLetter()!='0' ){
				return true;
			}
			
			otherWordStart=row-rowIterUp;
			otherWordEnd=row+rowIterDown;
			
			char * arr= new char[otherWordEnd-otherWordStart+1];
			
			for (int iter=0;iter<otherWordEnd-otherWordStart+1;iter++)
			{
				arr[iter]=boardTiles[otherWordStart+iter][col+i].GetLetter();
			}	
			
			string wordToTest(arr);
			
			delete [] arr;
				
			if (!dag_->CheckWordInDict(wordToTest))
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
			int otherWordStart=0;
			int otherWordEnd=0;										//Index that will get us the word to test using the GAD-dag_
			int colIterLeft=0,colIterRight=0;					//To up or down;
			
			if (col!=0 && col!=14 &&boardTiles[row+i][col-colIterLeft].GetLetter()=='0'
			&&boardTiles[row+i][col+colIterRight].GetLetter()=='0')
				{
					return true;
				}


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
			if(boardTiles[row+i][col-colIterLeft].GetLetter()=='0'
			&&boardTiles[row+i][col+colIterRight].GetLetter()=='0'){
				return true;
			}
			otherWordStart=col-colIterLeft;
			otherWordEnd=col+colIterRight;
			
			char * arr= new char[otherWordEnd-otherWordStart+1];
			
			for (int iter=0;iter<otherWordEnd-otherWordStart+1;iter++)
			{
				arr[iter]=boardTiles[col+i][otherWordStart+iter].GetLetter();
			}	
			
			string wordToTest(arr);

			delete [] arr;
							
			if (!dag_->CheckWordInDict(wordToTest))
				return false;
		}

		return true;

	}
}
//This function will add the possible moves to the play vector and discard the rest
//Row & Col will give the index of the hook
vector<Play> MoveGenerate::check_words(Board&board,vector<pair<string,vector<int> > > returnedWords,int row,int col,bool horizontal)	//returnedWords--> words from GAD-dag_
{
vector<Play> possiblePlays;
//Loops all over the possible words
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


void MoveGenerate::generateWordsAtRows(Board &board)
{
	Tile boardTiles[15][15];
	board.GetTiles(boardTiles);

	for (int rowIter=0;rowIter<15; rowIter++)
	{
		//First will send entire row, then entire row - first pattern, then entire row - (first+second) patterns.. and so on till end of row..
		int stringToDagSize=0;
		
		vector< pair <string,vector<int> > > possibleWords; //Vector that will carry possible combinations after we have it returned from GADDAG

		for (int colIter=0;colIter<15;colIter++)
		{
			while (boardTiles[rowIter][colIter].GetLetter()!='0')
			{
				stringToDagSize++;
				colIter++;
				if (colIter==15)
					break;	
			}

			if (stringToDagSize!=0)								//Will be zero if row is empty
			{
				int hookBeginIndex=colIter-stringToDagSize;				//Begin index of current hook
				int reverseIterator=hookBeginIndex-2;				//An iterator to go back till we reach a previous pattern or begin of row
				int hookSize=15-hookBeginIndex;					//Calculating the size of our hook
	   			//The hook to be send in each iteration
				string hookString="";
				//Now we fill out the hook to be sent		//
				for (int k=0;k<hookSize;k++)
				{
					if (boardTiles[rowIter][k+hookBeginIndex].GetLetter()=='0')
					{	
						
						hookString+='.';
					}
					else {	
						hookString+=boardTiles[rowIter][k+hookBeginIndex].GetLetter();
				}
				}
				
				if (reverseIterator==-1) //We have reached the beginning of the row (First pattern special case)
				{
					//We are sending the entire row. Therefore, the hook will be the entire row...
					//possibleWords=send_to_GADDAG(hook,rack_,1,1) 					
					possibleWords=dag_->ContainsHookWithRackAtPos(hookString,rack_,1,1);
				}
				
				//A previous pattern --->    _ _ a r r _ _ b a _ _ _ _ _ _ -->here we reach the 'r' at pos 4...
				else if (boardTiles[rowIter][reverseIterator].GetLetter()!='0')    
				{
					//possibleWords=send_to_GADDAG(hook,rack_,hookBeginIndex-(reverseIterator+2),hookBeginIndex)
					possibleWords=dag_->ContainsHookWithRackAtPos(hookString,rack_,hookBeginIndex-(reverseIterator+2),hookBeginIndex);
				}
				
				//Here is the general case, we are moving backingwards without hitting a previous pattern yet or reaching start of row
				else if (boardTiles[rowIter][reverseIterator].GetLetter()=='0')
				{
					
					while (boardTiles[rowIter][reverseIterator].GetLetter()=='0'&&reverseIterator>0)
					{
						reverseIterator--;
					}

					if (reverseIterator==0)							//Reached the begining of the row
					{
						//We are sending the entire row. Therefore, the hook will be the entire row...
						//possibleWords=send_to_GADDAG(hook,rack_,hookBeginIndex-1,hookBeginIndex)   			
						possibleWords=dag_->ContainsHookWithRackAtPos(hookString,rack_,hookBeginIndex-1,hookBeginIndex);
					}
					else										//Reached a previous pattern 
					{
						//possibleWords=send_to_GADDAG(hook,rack_,hookBeginIndex-(reverseIterator+2),hookBeginIndex)
						possibleWords=dag_->ContainsHookWithRackAtPos(hookString,rack_,hookBeginIndex-(reverseIterator+2),hookBeginIndex);
					}

				}

		

				//Here we are testing the words we received from the GAD-dag_
				vector<Play> possiblePlays = check_words(board,possibleWords,rowIter,colIter,true);
				for(int i = 0; i < (int)possiblePlays.size(); i++)
				{
					plays_.push_back(possiblePlays.at(i));
				}
				
				stringToDagSize=0;
			}
		}
	}
}


void MoveGenerate::generateWordsAtCols(Board &board)
{
	Tile boardTiles[15][15];
	board.GetTiles(boardTiles);

	for (int colIter=0;colIter<15; colIter++)
	{
		//First will send entire col, then entire col - first pattern, then entire col - (first+second) patterns.. and so on till end of col..
		int stringToDagSize=0;
		
		vector< pair <string,vector<int> > > possibleWords; //Vector that will carry possible combinations after we have it returned from GADDAG

		for (int rowIter=0;rowIter<15;rowIter++)
		{
			while (boardTiles[rowIter][colIter].GetLetter()!='0')
			{
				stringToDagSize++;
				rowIter++;	
				if (rowIter==15)
					break;
			}

			if (stringToDagSize!=0)
			{
				int hookBeginIndex=colIter-stringToDagSize;			//Begin index of our hook
				int reverseIterator=hookBeginIndex-2;				//The iterator to go back till we reach a previous pattern or coloumn begin
				int hookSize=15-hookBeginIndex;					//Calculating the hook size
				
				string hookString="";
				  			//Hook to be sent
				
				//Now we fill out the hook to be sent
				for (int k=0;k<hookSize;k++)
				{
					hookString+=boardTiles[k+hookBeginIndex][colIter].GetLetter();
				}
				
				if (reverseIterator==-1) //We have reached the beginning of the row (First pattern special case)
				{
					//We are sending the entire row. Therefore, the hook will be the entire row...
					//possibleWords=send_to_GADDAG(hook,rack_,1,1) 
					possibleWords=dag_->ContainsHookWithRackAtPos(hookString,rack_,1,1);
				}
			
				//A previous pattern --->    _ _ a r r _ _ b a _ _ _ _ _ _ -->here we reach the 'r' at pos 4...
				else if (boardTiles[reverseIterator][colIter].GetLetter()!='0')    
				{
					//possibleWords=send_to_GADDAG(hook,rack_,hookBeginIndex-(reverseIterator+2),hookBeginIndex)
					possibleWords=dag_->ContainsHookWithRackAtPos(hookString,rack_,hookBeginIndex-(reverseIterator+2),hookBeginIndex);
				
				}
				
				//Here is the general case, we are moving backingwards without hitting a previous pattern yet or reaching start of row
				else if (boardTiles[reverseIterator][colIter].GetLetter()=='0')
				{
					
					while (boardTiles[reverseIterator][colIter].GetLetter()=='0'&&reverseIterator>0)
					{
						reverseIterator--;
					}

					if (reverseIterator==0)							//Reached the begining of the row
					{
						//We are sending the entire row. Therefore, the hook will be the entire row...
						//possibleWords=send_to_GADDAG(hook,rack_,hookBeginIndex-1,hookBeginIndex)   			
						possibleWords=dag_->ContainsHookWithRackAtPos(hookString,rack_,hookBeginIndex-1,hookBeginIndex);
					}
					else										//Reached a previous pattern 
					{
						//possibleWords=send_to_GADDAG(hook,rack_,hookBeginIndex-(reverseIterator+2),hookBeginIndex)
						possibleWords=dag_->ContainsHookWithRackAtPos(hookString,rack_,hookBeginIndex-(reverseIterator+2),hookBeginIndex);
					}

					//Here we are testing the words we received from the GAD-dag_
					check_words(board,possibleWords,-1,colIter,true);
				}
				

				//Here we are testing the words we received from the GAD-dag_
				vector<Play> possiblePlays = check_words(board,possibleWords,rowIter,colIter,true);
				for(int i = 0; i < (int)possiblePlays.size(); i++)
				{
					plays_.push_back(possiblePlays.at(i));
				}
				

				stringToDagSize=0;
			}
		}
	}
}

vector<Play> MoveGenerate::Generate(const Rack * rack, Board & board){

	this->set_Rack(*rack);
	if(board.GetCount() == 0){
		// Start from the middle try all possibilities.
	}

	plays_.clear();
	
	this->generateWordsAtCols(board);
	this->generateWordsAtRows(board);
	return plays_;
}

MoveGenerate::~MoveGenerate(){
	
}
