#include <iostream>
#include<vector>
using namespace std;


char Board[15][15];
char Rack[7];

// returns an array of 2d arrays
// each 2D array in the 3d array represents a view of the board after a specific move
char*** GenerateRackWords() {
	// 1. sends all possible permutations of the rack to the GADDAG
	// 2. GADDAG returns all possible words to be played
	// 3. sort the words by length 
	// 4. loops vertically and horizontally to fit any of the words
	// 5. adds a board for each possible word that could be added on the board
}


bool IsValid(string word, int startPosition) {
	// 1. checks if any character is not in the rack
	// 2. if not then check if it is on the board in the required position
	// 3. if it was not on the board or not on the required position return false
	// 4. if it was on the board and on the required position then keep the flag true
	// 5. return the flag

}
// function that either loops column  by column  or row by row
// returns an array of 2d arrays
// each 2D array in the 3d array represents a view of the board after a specific move
char*** LoopBoard(int direction) 
{

	if (direction == 0) {
		for (int row = 0; row < 10; row++) 
		{
			int columnCount = 0;
			while (columnCount<15)
			{
				columnCount;
				vector<char> subWord;
				int wordCount = 0;
				while (columnCount < 15 && Board[row][columnCount]!=NULL)
				{
					subWord.push_back(Board[row][columnCount]);
					columnCount++;
					wordCount++;
				}
				if (subWord.size() > 0) 
				{
					// 1. Search the GADDAG for the subWord
					// 2. Get all occurences 
					// 3. loop on all returned occurences and check using IsValid(occurence,columnCount - wordCount)
					// 4. if valid add the word to a 1D array/vector and keep count it will be needed later call it possibleBoards
					// 5. at the end there will be a 3d array Result[possibleBoards][15][15] or a vector of char**
				}
				
			}
			// 1. gets the first substring that appears in the row
			// 2. searches the dictionary of all it's permutations
			// 3. returns all permutatons in a list of strings 
			// 4. check if the each permutation IsValid
			// 5. Validity is checked both ways whether my rack and/or the board has all the letters of the permutation
			// 6. remove invalid words
			// 7. generate an updated board for each new word that can be generated
			// 8. repeat while row is unfinished
		}
	}
	else if (direction == 1) 
	{
		for (int column = 0; column < 10; column++) {
			// 1. gets the first substring that appears in the column
			// 2. searches the dictionary of all it's permutations
			// 3. returns all permutatons in a list of strings 
			// 4. check if the each permutation IsValid
			// 5. Validity is checked both ways whether my rack and/or the board has all the letters of the permutation
			// 6. remove invalid words
			// 7. generate an updated board for each new word that can be generated
			// 8. repeat while column is unfinished
			int rowCount;
			
			while (rowCount < 15)
			{
				rowCount = 0;
				vector<char> subWord;
				int wordCount = 0;
				while (rowCount < 15 && Board[rowCount][column] != NULL)
				{
					subWord.push_back(Board[rowCount][column]);
					rowCount++;
					wordCount++;
				}
				if (subWord.size() > 0)
				{
					// 1. Search the GADDAG for the subWord
					// 2. Get all occurences 
					// 3. loop on all returned occurences and check using IsValid(occurence,columnCount - wordCount)
					// 4. if valid add the word to a 1D array/vector and keep count it will be needed later call it possibleBoards
					// 5. at the end there will be a 3d array Result[possibleBoards][15][15] or a vector of char**
				}
			}
	}
}


void GenerateMoves()
{
	// First: loop on the board row by row
	LoopBoard(0);

	// Second loop on the board column by column
	LoopBoard(1);

	// Third look for words that are only generated from your rack and where can they fit in on the board whether horizontally of vertically
	GenerateRackWords();
}
void main() {

}