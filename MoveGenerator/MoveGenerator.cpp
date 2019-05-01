#include"MoveGenerator.h"


MoveGenerator::MoveGenerator(){

}

MoveGenerator::MoveGenerator(Board& board){
	dag_ = new GADDAG("assets/Dict.txt");
	board.GetTiles(this->boardTiles_);
}

void MoveGenerator::SetRack(Rack gameRack){
	rack_="";
	for (int i=0;i<gameRack.GetLength();i++)
	{
		rack_+=gameRack.GetLetter(i);
	}
}

void MoveGenerator::GenerateRackWords(bool boardEmpty){
	
		rackpossibilities_ = ContainsHookWithRack("",rack_);
		if (boardEmpty)
		{
			for(int i = 0; i < (int)rackpossibilities_.size(); i++)
			{

				for (int k = (int)rackpossibilities_[i].GetWord().length(); k>0;k-- ){
				Play* pHorizontal = new Play(rackpossibilities_[i].GetWord(),7,7-k>0?7-k:0,true);
				CalculatePlayScore(pHorizontal,0);
				Play* pVertical = new Play(rackpossibilities_[i].GetWord(),7-k>0?7-k:0,7,false);
				CalculatePlayScore(pVertical,0);
				Move moveH;
				Move moveV;
				moveH.SetPlay(pHorizontal);
				moveH.SetRack(rackpossibilities_[i].GetRemainingRacks()[0]);
				moveV.SetPlay(pVertical);
				moveV.SetRack(rackpossibilities_[i].GetRemainingRacks()[0]);
				moves_.push_back(moveH);
				moves_.push_back(moveV);
				}
			}
		}

}


bool MoveGenerator::CheckOtherDimension(string word,int row,int col,bool horizontal, int& additionalScore) //True when called from generateWordsAtRows
{
	if (IsOnBoard(word,row,col,horizontal)){
		return false;
	}

	if (horizontal)  //word is horizontal, we need to check the columns of the letters
	{
																//_ _ _ _ _ _|c|_ _ _ _ _ _ _ _			-->>Here we want to insert "ay" after
																//_ _ _ a r r|_|_ _ _ _ _ _ _ _  		"arr", but at col 6 we have to check
		for (int i=0;i<(int)word.size();i++)					//_ _ _ _ _ _|t|_ _ _ _ _ _ _ _ 		if inserting "a" is ok. "cat" is ok!!
		{	
			int otherWordStart=0;
			int otherWordEnd=0;										//Index that will get us the word to test using the GAD-dag_
			int rowIterUp=0,rowIterDown=0;						//To up or down;
			int score = 0;

			if (row!=0 && row!=14 && boardTiles_[row-1][col+i]->GetLetter()=='0' &&
			boardTiles_[row+1][col+i]->GetLetter()=='0'){
				continue;
			}

			if(row!=0)											//We are at the top row...
			{
				while (boardTiles_[row-rowIterUp-1][col+i]->GetLetter()!='0')
				{
					
					if (row-rowIterUp-1<=0)						//We must check so that we will not access an out of bound memory
						break;
					rowIterUp++;
				}
			}
			if (row!=14)										//We are at the bottow row...
			{
				while (boardTiles_[row+rowIterDown+1][col+i]->GetLetter()!='0')
				{
					
					if (row+rowIterDown+1>=14)					//We must check so that we will not access an out of bound memory
						break;
					rowIterDown++;
				}
			}
			
			
			otherWordStart=row-rowIterUp;
			otherWordEnd=row+rowIterDown;
			
			string wordToTest="";
			bool wordOnBoard = true;
			for (int iter=otherWordStart;iter<=otherWordEnd;iter++)
			{
				if (boardTiles_[iter][col+i]->GetLetter()=='0'){
					wordToTest+=word[i];
					wordOnBoard=false;
				}
				else{
				wordToTest+=boardTiles_[iter][col+i]->GetLetter();
				
			}	
			}
			
				
			if (!dag_->CheckWordInDict(wordToTest))
			{	
				return false;
			}
			else if(!wordOnBoard) {
				Play * playToTest = new Play(wordToTest,otherWordStart,col+i,!horizontal);
				CalculatePlayScore(playToTest,0);
				additionalScore+=playToTest->GetScore();
				delete playToTest;
				//Calculate the score of this word to be added to the score of the actual word being played,
			}
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
			
			if (col!=0 && col!=14 &&boardTiles_[row+i][col-1]->GetLetter()=='0'
			&&boardTiles_[row+i][col+1]->GetLetter()=='0')
				{
					continue;
				}


			if(col!=0)											//We are at the top row...
			{
				while (boardTiles_[row+i][col-colIterLeft-1]->GetLetter()!='0')
				{
					if (col-colIterLeft-1<=0)						//We must check so that we will not access an out of bound memory
						break;
					colIterLeft++;
				}
			}

			if (col!=14)										//We are at the bottow row...
			{
				while (boardTiles_[row+i][col+colIterRight+1]->GetLetter()!='0')
				{
					
					if (col+colIterRight+1>=14)					//We must check so that we will not access an out of bound memory
						break;
					colIterRight++;
				
				}
			}
			
			otherWordStart=col-colIterLeft;
			otherWordEnd=col+colIterRight;
			
			string wordToTest = "";
			bool wordOnBoard = true;
			for (int iter=otherWordStart;iter<=otherWordEnd;iter++)
			{
				if (boardTiles_[row+i][iter]->GetLetter() =='0'){
					wordToTest+=word[i];
					wordOnBoard=false;
				}
				else{
				wordToTest+=boardTiles_[row+i][iter]->GetLetter();
				
				}	
			}
			
							
			if (!dag_->CheckWordInDict(wordToTest))
			{	
				return false;
			}
			else if(!wordOnBoard) {
				Play * playToTest = new Play(wordToTest,row+i,otherWordStart,!horizontal);
				CalculatePlayScore(playToTest,0);
				additionalScore+=playToTest->GetScore();
				delete playToTest;
				//Calculate the score of this word to be added to the score of the actual word being played,
			}
		}

		return true;

	}
}
//This function will add the possible moves to the play vector and discard the rest
//Row & Col will give the index of the hook
void MoveGenerator::CheckWords(vector<WordPossibility> returnedWords,int row,int col,bool horizontal)	//returnedWords--> words from GAD-dag_
{

//Loops all over the possible words
	for (int i=0;i<(int)returnedWords.size();i++)
	{

		if (horizontal)
		{
			for (int j=0;j<(int)returnedWords[i].GetPositions().size();j++)
			{
				int additionalScore = 0;
				if (CheckOtherDimension(returnedWords[i].GetWord(),row,returnedWords[i].GetPositions().at(j), horizontal,additionalScore))
				{
					Play *P = new Play(returnedWords[i].GetWord(),row,returnedWords[i].GetPositions().at(j),horizontal);
					CalculatePlayScore(P,additionalScore);
					Move move;
					move.SetPlay(P);
					move.SetRack(returnedWords[i].GetRemainingRacks()[j]);
					moves_.push_back(move);
				}
						
			}
		}	
		else
		{
			for (int j=0;j<(int)returnedWords[i].GetPositions().size();j++)
			{
				int additionalScore = 0;
				if (CheckOtherDimension(returnedWords[i].GetWord(),returnedWords[i].GetPositions().at(j),col, horizontal,additionalScore))
				{
					Play *P = new Play(returnedWords[i].GetWord(),returnedWords[i].GetPositions().at(j),col,horizontal);
					CalculatePlayScore(P,additionalScore);
					Move move;
					move.SetPlay(P);
					move.SetRack(returnedWords[i].GetRemainingRacks()[j]);
					moves_.push_back(move);
				}
			}
		}
	}
}

bool MoveGenerator::IsOnBoard(string word, int row, int column, bool isHorizontal){
	
	for (int i = 0; i < (int)word.length(); i++)
	{
		if (this->boardTiles_[row][column]->GetLetter()=='0'){
			return false;
		}
		if (isHorizontal){
			column++;
		}
		else{
			row++;
		}
	}
	return true;
	
}
void MoveGenerator::GenerateWordsAtRows()
{

	for (int rowIter=0;rowIter<15; rowIter++)
	{
		//First will send entire row, then entire row - first pattern, then entire row - (first+second) patterns.. and so on till end of row..
		int stringToDagSize=0;
		
		vector<WordPossibility> possibleWords; //Vector that will carry possible combinations after we have it returned from GADDAG

		for (int colIter=0;colIter<15;colIter++)
		{		
			while (this->boardTiles_[rowIter][colIter]->GetLetter()!='0')
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
					if (this->boardTiles_[rowIter][k+hookBeginIndex]->GetLetter()=='0')
					{	
						
						hookString+='.';
					}
					else {	
						hookString+=this->boardTiles_[rowIter][k+hookBeginIndex]->GetLetter();
				}
				}
				
				if (reverseIterator<=-1) //We have reached the beginning of the row (First pattern special case)
				{
					//We are sending the entire row. Therefore, the hook will be the entire row...
					//possibleWords=send_to_GADDAG(hook,rack_,1,1) 					
					possibleWords=ContainsHookWithRackAtPos(hookString,rack_,1,1);
				}
				
				//A previous pattern --->    _ _ a r r _ _ b a _ _ _ _ _ _ -->here we reach the 'r' at pos 4...
				else if (this->boardTiles_[rowIter][reverseIterator]->GetLetter()!='0')    
				{
					//possibleWords=send_to_GADDAG(hook,rack_,hookBeginIndex-(reverseIterator+2),hookBeginIndex)
					possibleWords=ContainsHookWithRackAtPos(hookString,rack_,hookBeginIndex-(reverseIterator+2),hookBeginIndex);
				}
				
				//Here is the general case, we are moving backingwards without hitting a previous pattern yet or reaching start of row
				else if (this->boardTiles_[rowIter][reverseIterator]->GetLetter()=='0')
				{
					
					while (this->boardTiles_[rowIter][reverseIterator]->GetLetter()=='0'&&reverseIterator>0)
					{
						reverseIterator--;
					}

					if (reverseIterator==0)							//Reached the begining of the row
					{
						//We are sending the entire row. Therefore, the hook will be the entire row...
						//possibleWords=send_to_GADDAG(hook,rack_,hookBeginIndex-1,hookBeginIndex)   			
						possibleWords=ContainsHookWithRackAtPos(hookString,rack_,hookBeginIndex-1,hookBeginIndex);
					}
					else										//Reached a previous pattern 
					{
						//possibleWords=send_to_GADDAG(hook,rack_,hookBeginIndex-(reverseIterator+2),hookBeginIndex)
						possibleWords=ContainsHookWithRackAtPos(hookString,rack_,hookBeginIndex-(reverseIterator+2),hookBeginIndex);
					}

				}

		

				//Here we are testing the words we received from the GAD-dag_
				CheckWords(possibleWords,rowIter,colIter,true);
				
				stringToDagSize=0;
			}
		}
	}
}


void MoveGenerator::GenerateWordsAtCols()

{
	for (int colIter=0;colIter<15; colIter++)
	{
		//First will send entire col, then entire col - first pattern, then entire col - (first+second) patterns.. and so on till end of col..
		int stringToDagSize=0;
		
		vector<WordPossibility> possibleWords; //Vector that will carry possible combinations after we have it returned from GADDAG

		for (int rowIter=0;rowIter<15;rowIter++)
		{
			while (this->boardTiles_[rowIter][colIter]->GetLetter()!='0')
			{
				stringToDagSize++;
				rowIter++;	
				if (rowIter==15)
					break;
			}

			if (stringToDagSize!=0)
			{
				int hookBeginIndex=rowIter-stringToDagSize;			//Begin index of our hook
				int reverseIterator=hookBeginIndex-2;				//The iterator to go back till we reach a previous pattern or coloumn begin
				int hookSize=15-hookBeginIndex;					//Calculating the hook size
				
				string hookString="";
				  			//Hook to be sent
				
				//Now we fill out the hook to be sent
				for (int k=0;k<hookSize;k++)
				{
					if (this->boardTiles_[k+hookBeginIndex][colIter]->GetLetter()=='0')
					{	
						
						hookString+='.';
					}else
					{
						hookString+=this->boardTiles_[k+hookBeginIndex][colIter]->GetLetter();
			
					}
					
					}
				
				if (reverseIterator<=-1) //We have reached the beginning of the row (First pattern special case)
				{
					//We are sending the entire row. Therefore, the hook will be the entire row...
					//possibleWords=send_to_GADDAG(hook,rack_,1,1) 
					possibleWords=ContainsHookWithRackAtPos(hookString,rack_,1,1);
				}
			
				//A previous pattern --->    _ _ a r r _ _ b a _ _ _ _ _ _ -->here we reach the 'r' at pos 4...
				else if (this->boardTiles_[reverseIterator][colIter]->GetLetter()!='0')    
				{
					//possibleWords=send_to_GADDAG(hook,rack_,hookBeginIndex-(reverseIterator+2),hookBeginIndex)
					possibleWords = ContainsHookWithRackAtPos(hookString,rack_,hookBeginIndex-(reverseIterator+2),hookBeginIndex);
				
				}
				
				//Here is the general case, we are moving backingwards without hitting a previous pattern yet or reaching start of row
				else if (this->boardTiles_[reverseIterator][colIter]->GetLetter()=='0')
				{
					
					while (this->boardTiles_[reverseIterator][colIter]->GetLetter()=='0'&&reverseIterator>0)
					{
						reverseIterator--;
					}

					if (reverseIterator==0)							//Reached the begining of the row
					{
						//We are sending the entire row. Therefore, the hook will be the entire row...
						//possibleWords=send_to_GADDAG(hook,rack_,hookBeginIndex-1,hookBeginIndex)   			
						possibleWords=ContainsHookWithRackAtPos(hookString,rack_,hookBeginIndex-1,hookBeginIndex);
					}
					else										//Reached a previous pattern 
					{
						//possibleWords=send_to_GADDAG(hook,rack_,hookBeginIndex-(reverseIterator+2),hookBeginIndex)
						possibleWords=ContainsHookWithRackAtPos(hookString,rack_,hookBeginIndex-(reverseIterator+2),hookBeginIndex);
					}

					//Here we are testing the words we received from the GAD-dag_
					//CheckWords(board,possibleWords,-1,colIter,false);
				}
				

				//Here we are testing the words we received from the GAD-dag_
				CheckWords(possibleWords,rowIter,colIter,false);
				
				stringToDagSize=0;
			}
		}
	}
}

void MoveGenerator::LoopBoard(){
	
	//Loop on all rack permutations
	for (int i=0;i<(int)rackpossibilities_.size();i++)
	{
		//loop on the whole board
			//If the word is touching any tile and not overlapping another word
			//Add to valid moves
			//else move one Tile to the right or Down
			for(int row = 0; row < 15; row++)
			{
				for(int column = 0; column < 15; column++)
				{
					if (this->boardTiles_[row][column]->GetLetter()!='0'){
						continue;
					}
					bool completeWord = true;
					for (int wordLetter = 0;wordLetter<(int)rackpossibilities_[i].GetWord().size();wordLetter++)
					{
						if (column+wordLetter>=15){
							completeWord=false;
							break;
						}
						if (this->boardTiles_[row][column+wordLetter]->GetLetter()!='0')
						{
							completeWord = false;
							break;
						}
					}
					if (!completeWord){
						//row++;
						continue;
					}
					if (WordIsTouching(rackpossibilities_[i].GetWord(),row,column,true)){
						vector<WordPossibility> returnedWords;
						rackpossibilities_[i].SetPosition(column);
						returnedWords.push_back(rackpossibilities_[i]);
						CheckWords(returnedWords,row,column,true);
					}
				
				}

				
		}
	}
	for (int i=0;i<(int)rackpossibilities_.size();i++){
		for (int column=0;column<15;column++){
			for(int row = 0;row<15;row++){
				if (this->boardTiles_[row][column]->GetLetter()!='0'){
					continue;
				}
				bool completeWord = true;
					for (int wordLetter = 0;wordLetter<(int)rackpossibilities_[i].GetWord().size();wordLetter++)
					{
						if (row+wordLetter>=15){
							completeWord=false;
							break;
						}
						if (this->boardTiles_[row+wordLetter][column]->GetLetter()!='0')
						{
							completeWord = false;
							break;
						}
					}
					if (!completeWord){
						//column++;
						continue;
					}
					if (WordIsTouching(rackpossibilities_[i].GetWord(),row,column,false)){
						vector<WordPossibility> returnedWords;
						rackpossibilities_[i].SetPosition(row);
						returnedWords.push_back(rackpossibilities_[i]);
						CheckWords(returnedWords,row,column,false);
					}
			}
		}
	}

}

bool MoveGenerator::WordIsTouching(string word, int row, int column, bool horizontal){
	bool flag=false;
	if  (horizontal){
		
		if (column +word.size()>=15){
			return false;
		}

		if ((column > 0 && this->boardTiles_[row][column-1]->GetLetter()!='0') || 
			(column+word.size()<15 && this->boardTiles_[row][column+word.size()]->GetLetter()!='0')){
				return false;
		}
		
		if (row>0 && row <14)
		{	
			for (int i = 0;i<(int)word.size();i++)
			{
			if (this->boardTiles_[row+1][column+i]->GetLetter()=='0' && this->boardTiles_[row-1][column+i]->GetLetter()=='0')
			{
				continue;
			}
			else 
				flag=true;
			}	
		}
		else if (row ==0){
			for (int i = 0;i<(int)word.size();i++)
			{
			if (this->boardTiles_[row+1][column+i]->GetLetter()=='0'){
				continue;
			}
			else 
				flag=true;
		}
		}
		else if (row==14){
			for (int i = 0;i<(int)word.size();i++)
			{
			if (this->boardTiles_[row-1][column+i]->GetLetter()=='0'){
				continue;
			}
			else 
				flag=true;
		}
		}
	}
	else{

		
		if (row +word.size()>=15){
			return false;
		}

		if (row>0&&this->boardTiles_[row-1][column]->GetLetter()!='0'||
			row+word.size()<15 && boardTiles_[row+word.size()][column]->GetLetter()!='0'){
				return false;
		}
		
		if (column>0 && column <14)
		{
			
			for (int i = 0;i<(int)word.size();i++)
		{	
			if (boardTiles_[row+i][column+1]->GetLetter()=='0' && boardTiles_[row+i][column-1]->GetLetter()=='0'){
				continue;
		}
			else 
				flag=true;
		}
		}
		else if (column == 0){
			for (int i = 0;i<(int)word.size();i++)
			{
			if (boardTiles_[row+i][column+1]->GetLetter()=='0'){
				continue;
			}
			else 
				flag=true;
			}	
		}
		else if (column==14){
			for (int i = 0;i<(int)word.size();i++)
		{	
			if (boardTiles_[row+i][column-1]->GetLetter() =='0'){
				continue;
			}
			else 
				flag=true;
		}}
	}
return flag;
}

vector<Move> MoveGenerator::Generate(const Rack * rack,Board &board,bool isEmpty)
{
	board.GetTiles(this->boardTiles_);
	moves_.clear();
	this->SetRack(*rack);
	GenerateRackWords(isEmpty); 
	this->LoopBoard();
	this->GenerateWordsAtCols();
	this->GenerateWordsAtRows();
	
	return moves_;
}
void MoveGenerator::ContainsHookWithRackRecursive(Node* CurrentNode, vector<WordPossibility> &VectorOfPossibleWords, string letters, string rack, string hook) 

{

		if (CurrentNode == NULL) {

		string Word = dag_->GetWord(letters);

		vector<WordPossibility>::iterator it;

		for (it = VectorOfPossibleWords.begin(); it != VectorOfPossibleWords.end(); it++) {

			if (it->GetWord() == Word) break;

		}



		if (it == VectorOfPossibleWords.end()) { //word not found in Vector

			WordPossibility newWord(Word);

			newWord.SetWord(Word);

			newWord.AddPositions(7);

			newWord.AddRacks(rack);

			VectorOfPossibleWords.push_back(newWord);

		}

		

		return;

	}



	if (hook != "" && hook != " ") 

	{

		if (CurrentNode->get_Letter() != Node::Root)

			letters += CurrentNode->get_Letter();



		if (CurrentNode->ContainsKey(hook[0])) 

		{

			string NewHook = hook;

			NewHook = NewHook.erase(0, 1);

			ContainsHookWithRackRecursive(CurrentNode->AT(hook[0]), VectorOfPossibleWords, letters, rack, NewHook);

		}

	}

	else {

		if (CurrentNode->get_Letter() != Node::Root)

			letters += CurrentNode->get_Letter();



		vector<char> vectorOfChildrenOfCurrentNode = CurrentNode->Keys();

		for (int i = 0; i < (int)vectorOfChildrenOfCurrentNode.size(); i++) {

			char Key = vectorOfChildrenOfCurrentNode[i];



			std::size_t FoundInRack = rack.find(Key);

			std::size_t BlankFoundInRack = rack.find('?');

			if (Key == Node::EOW || Key == Node::Break || FoundInRack != std::string::npos || BlankFoundInRack != std::string::npos) {

				string NewRack = rack;

				if (Key != Node::EOW && Key != Node::Break && FoundInRack != std::string::npos) //found in rack

					NewRack = NewRack.erase(FoundInRack, 1);

				else if (Key != Node::EOW && Key != Node::Break && BlankFoundInRack != std::string::npos) //space in rack

					NewRack = NewRack.erase(BlankFoundInRack, 1);

				//NewRack = (Key != Node::EOW && Key != Node::Break) ? NewRack.erase(FoundInRack, 1) : NewRack;

				ContainsHookWithRackRecursive(CurrentNode->AT(Key), VectorOfPossibleWords, letters, NewRack, hook);

			}

		}

	}

}

void MoveGenerator::ContainsHookWithRackRecursiveAtPos(Node* CurrentNode, vector<WordPossibility> &VectorOfPossibleWords, string letters, string rack, string hook, int MaxPos, int CurrentCount, bool found, int CurrPosOnBoard) {

	if (found) {

		if ((CurrentNode != NULL && CurrentNode->get_Letter() != Node::Break && CurrentNode->get_Letter() != Node::EOW) || CurrentNode == NULL) {

			int MaxLength = 15 - CurrPosOnBoard + CurrentCount; // ----er- if currposonboard= 12 (enters)

																//currcount= 3 then maxlength= 15- 12 +3= 6 

			if ((int)letters.length() - 1 > MaxLength)

				return;

		}

	}



	if (CurrentCount > MaxPos) return;



	bool NewFound = found;



	if (CurrentNode == NULL) {

		string Word = dag_->GetWord(letters);

		vector<WordPossibility>::iterator it;

		for (it = VectorOfPossibleWords.begin(); it != VectorOfPossibleWords.end(); it++) {

			if (it->GetWord() == Word) break;

		}



		if (it == VectorOfPossibleWords.end()) { //word not found in Vector

			WordPossibility newWord(Word);

			newWord.SetWord(Word);

			newWord.AddPositions(CurrPosOnBoard - CurrentCount);

			newWord.AddRacks(rack);

			VectorOfPossibleWords.push_back(newWord);

		}

		else {

			it->AddPositions(CurrPosOnBoard - CurrentCount);

			it->AddRacks(rack);

		}

		return;

	}

	if (hook[0] == '.') { //blank space on board

		int NewCount = CurrentCount;

		if (CurrentNode->get_Letter() != Node::Root)

			letters += CurrentNode->get_Letter();



		vector<char> vectorOfChildrenOfCurrentNode = CurrentNode->Keys();

		for (int i = 0; i < (int)vectorOfChildrenOfCurrentNode.size(); i++) {

			NewFound = found;

			NewCount = CurrentCount;

			string NewHook = hook;

			char Key = vectorOfChildrenOfCurrentNode[i];



			if (Key == Node::Break) NewFound = true;

			if (!NewFound) NewCount++;



			if (NewFound && Key != hook[0] && hook[0] != '.') continue;



			std::size_t FoundInRack = rack.find(Key);

			std::size_t BlankFoundInRack = rack.find('?');

			if (Key == Node::EOW || Key == Node::Break || FoundInRack != std::string::npos || BlankFoundInRack != std::string::npos) {

				string NewRack = rack;

				if (Key != Node::EOW && Key != Node::Break && FoundInRack != std::string::npos)

					NewRack = NewRack.erase(FoundInRack, 1);

				else if (Key != Node::EOW && Key != Node::Break && BlankFoundInRack != std::string::npos)

					NewRack = NewRack.erase(BlankFoundInRack, 1);

				//NewRack = (Key != Node::EOW && Key != Node::Break) ? NewRack.erase(FoundInRack, 1) : NewRack;

				if (Key != Node::Break && Key != Node::EOW && NewFound) NewHook.erase(0, 1);

				ContainsHookWithRackRecursiveAtPos(CurrentNode->AT(Key), VectorOfPossibleWords, letters, NewRack, NewHook, MaxPos, NewCount, NewFound, CurrPosOnBoard);

			}

		}

	}

	else if (hook != "") {

		int NewCount = CurrentCount;

		if (CurrentNode->get_Letter() != Node::Root) {

			letters += CurrentNode->get_Letter();

		}



		vector<char> vectorOfChildrenOfCurrentNode = CurrentNode->Keys();

		for (int i = 0; i <(int)vectorOfChildrenOfCurrentNode.size(); i++) {

			NewFound = found;

			char Key = vectorOfChildrenOfCurrentNode[i];

				

			if (Key == Node::Break) NewFound = true;

			else if (Key != hook[0] && NewFound) continue;



			int NewCount = CurrentCount;

			if (!NewFound && Key != Node::Break && Key != Node::EOW) NewCount++;



			string NewHook = hook;

			if (NewFound && Key == hook[0] && Key != Node::Break && Key != Node::EOW) NewHook = NewHook.erase(0, 1);



			std::size_t FoundInRack = rack.find(Key);

			std::size_t BlankFoundInRack = rack.find('?');

			if (!NewFound && FoundInRack == std::string::npos && BlankFoundInRack == std::string::npos && Key != Node::Break && Key != Node::EOW) //letter(s) before hook not in rack

				continue;

			else if (!NewFound && (Key == Node::EOW || Key == Node::Break || FoundInRack != std::string::npos || BlankFoundInRack != std::string::npos)) {//letter(s) before hook in rack

				string NewRack = rack;

				if (Key != Node::EOW && Key != Node::Break && FoundInRack != std::string::npos)

					NewRack = NewRack.erase(FoundInRack, 1);

				else if (Key != Node::EOW && Key != Node::Break && BlankFoundInRack != std::string::npos)

					NewRack = NewRack.erase(BlankFoundInRack, 1);



				ContainsHookWithRackRecursiveAtPos(CurrentNode->AT(Key), VectorOfPossibleWords, letters, NewRack, hook, MaxPos, NewCount, NewFound, CurrPosOnBoard);

			}

			else if (NewFound)

				ContainsHookWithRackRecursiveAtPos(CurrentNode->AT(Key), VectorOfPossibleWords, letters, rack, NewHook, MaxPos, NewCount, NewFound, CurrPosOnBoard);

		}

	}

	else if (hook == "") {

		if (CurrentNode->get_Letter() != Node::Root)

			letters += CurrentNode->get_Letter();



		int NewCount = CurrentCount;



		vector<char> vectorOfChildrenOfCurrentNode = CurrentNode->Keys();

		for (int i = 0; i < (int)vectorOfChildrenOfCurrentNode.size(); i++) {

			NewCount = CurrentCount;

			char Key = vectorOfChildrenOfCurrentNode[i];



			if (Key == Node::Break) NewFound = true;

			if (!NewFound) NewCount++;



			std::size_t FoundInRack = rack.find(Key);

			std::size_t BlankFoundInRack = rack.find('?');

			if (Key == Node::EOW || Key == Node::Break || FoundInRack != std::string::npos || BlankFoundInRack != std::string::npos) {

				string NewRack = rack;

				if (Key != Node::EOW && Key != Node::Break && FoundInRack != std::string::npos)

					NewRack = NewRack.erase(FoundInRack, 1);

				else if (Key != Node::EOW && Key != Node::Break && BlankFoundInRack != std::string::npos)

					NewRack = NewRack.erase(BlankFoundInRack, 1);

				//NewRack = (Key != Node::EOW && Key != Node::Break) ? NewRack.erase(FoundInRack, 1) : NewRack;

				ContainsHookWithRackRecursiveAtPos(CurrentNode->AT(Key), VectorOfPossibleWords, letters, NewRack, hook, MaxPos, NewCount, NewFound, CurrPosOnBoard);

			}

		}

	}

}





vector<WordPossibility> MoveGenerator::ContainsHookWithRack(string hook, string rack) {

	transform(hook.begin(), hook.end(), hook.begin(), ::tolower);

	reverse(hook.begin(), hook.end());



	vector<WordPossibility> VectorOfPossibleWords;

	ContainsHookWithRackRecursive(dag_->GetRoot(), VectorOfPossibleWords, "", rack, hook);

	return VectorOfPossibleWords;

}





vector<WordPossibility> MoveGenerator::ContainsHookWithRackAtPos(string hook, string rack, int pos, int CurrPosOnBoard) {

	transform(hook.begin(), hook.end(), hook.begin(), ::tolower);



	vector<WordPossibility> VectorOfPossibleWords;



	if (dag_->GetRoot()->ContainsKey(hook[0])) {

		Node* newNode = dag_->GetRoot()->AT(hook[0]);

		ContainsHookWithRackRecursiveAtPos(newNode, VectorOfPossibleWords, "", rack, hook.erase(0,1), pos, 0, false, CurrPosOnBoard);

	}



	return VectorOfPossibleWords;

}



void MoveGenerator::CalculatePlayScore(Play *p, int addedScore){
	int score = 0;

	int isTripleWord = 0;
	int isDoubleWord = 0;
	int columnIterator = p->GetColumn();
	
	int rowIterator = p->GetRow();
	TileScoreCalculator calculator;
	for(int i =0;i<(int)p->GetTiles().size();i++)
	{
		p->UpdateTile(i,calculator.GetTileScore(p->GetTile(i).GetLetter()));
		

		if(boardTiles_[rowIterator][columnIterator]->GetLetter()!='0'){
			score+=p->GetTile(i).GetScore();
		}
		else {
		
			// Triple word
			if ((rowIterator==7&&(columnIterator == 0 ||columnIterator == 14))|| 
			 ((rowIterator== 0 || rowIterator == 14)&&(columnIterator==0 || columnIterator==7 || columnIterator==14)))
			{
				isTripleWord++;
				score+=(p->GetTile(i).GetScore());
			}
			// Double Word
			else if (((rowIterator == columnIterator)&&(rowIterator<5 || rowIterator>9)&&(rowIterator!=14 && rowIterator!=0))||
			((14-rowIterator==columnIterator)&&(columnIterator<5||columnIterator>9)&&(columnIterator!=14 && columnIterator!=0))){
				isDoubleWord++;
				score+=(p->GetTile(i).GetScore());
			}
			// Triple Letter 
			else if (((rowIterator==1||rowIterator==13)&&(columnIterator==5 || columnIterator==9))||
					(((rowIterator==5 ||rowIterator ==9) &&( columnIterator==1 || columnIterator == 13 ||columnIterator==5 || columnIterator==9))))
			{
				score+=(p->GetTile(i).GetScore()*3);
			}
			// Double Letter
			else if 
				(((rowIterator == 0 || rowIterator == 14)&&(columnIterator == 3 || columnIterator ==11))||
				((rowIterator==2 || rowIterator == 12)&&(columnIterator==6 || columnIterator == 8))||
				((rowIterator==3 || rowIterator == 11)&&(columnIterator==0 || columnIterator==7 || columnIterator==14))||
				((rowIterator==6 || rowIterator == 8)&&(columnIterator==2 ||columnIterator==6 || columnIterator==8 || columnIterator==12))||
				((rowIterator == 7)&&(columnIterator == 3 || columnIterator==11)))
			{
				score+=(p->GetTile(i).GetScore()*2);
			}
			else {
				score+=p->GetTile(i).GetScore();
			}			
	}
	if (p->GetIsHorizontal()){
			columnIterator++;
		}
		else{
			rowIterator++;
		}
}
	for (int doubleCount = 0; doubleCount<isDoubleWord;doubleCount++)
	{
		score*=2;
	}
	for (int tripleCount = 0; tripleCount<isTripleWord;tripleCount++)
	{
		score*=3;
	}

	p->SetScore(score+addedScore);
	
}

MoveGenerator::~MoveGenerator(){
	
}
