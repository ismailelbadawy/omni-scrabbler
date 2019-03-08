#include "GADDAG/Node.h"
#include <string>
#include <fstream>
#include <set>

class GADDAG {
private:
	Node* RootNode;

	void SetRoot(Node *NewNode) { RootNode = NewNode; }

	//add new word to GADDAG
	void add(string NewWord);

	//recursive function to get all possible combinations
	void ContainsHookWithRackRecursive(Node* CurrentNode, set<string> &SetOfPossibleWords, string letters, string rack, string hook);

	//recursive function to get all possible combinations such that the first letter of the hook exists at [0..MaxPos] only
	//the function handles the spaces between letters represented by a dot '.'
	//it handles maxLength of the possible words too
	void ContainsHookWithRackRecursiveAtPos(Node* CurrentNode, vector<pair<string, vector<int>>> &VectorOfPossibleWords, string letters, string rack, string hook, int MaxPos, int CurrentCount, bool found, int CurrPosOnBoard);

	//adjust word to be added in the list of possible combinations
	string GetWord(string str);

	//Load dag with dictionary hard coded
	void LoadDagHardCoded() {}

	//Function to read the dictionary represented as an input file 
	//in the assets folder, it takes the inputPath and loads the GADDAG
	void LoadDag(string InputPath);

public:
	//constructor to initialize Root and load dictionary
	GADDAG(); //uses LoadDagHardCoded Function
	GADDAG(string InputPath); //uses LoadDag function and requires the input path of dictionary

	//get RootNode
	Node* GetRoot() { return RootNode; }

	//Function to be called to return a vector of all possible words 
	//it has parameters: hook (word on board to add letters to it) and rack (available letters)
	//if hook= ay and rack= persl --> vector={play, player, plays} 
	//hook can be sent as "" or " " to get all combinations based on the rack only (for the first move in the game)
	vector<string> ContainsHookWithRack(string hook, string rack);

	//function to get all possible combinations such that the first letter of the hook exists at [0..MaxIndex] only
	//the function handles the spaces between letters represented by a dot '.'
	//example given hook= fi....arr...es.., and MaxPos= 0, function will return fire, fissfwarrt but NOT fisswwatrrt (if all letters exist in rack)
	//then if hook= arr...es.., MaxPos= 3 function will return array, arranges, barr, traarr, .. but NOT arrange and so on 
	
	//function handles max length of the word based on CurrPosOnBoard and Index of pattern in range [0..MaxIndex]
	//example given hook= arr...es.., MaxIndex= 2, CurrPosOnBoard= 5 (this means that a row on the board may be= fi..arr...es..)
	//then if arr is placed at pos 0, MaxLength will be 10
	//at pos 1, MaxLength = 11, at pos 2, MaxLength = 12, at Pos 3 is invalid as it will exceed MaxIndex =2

	//the function returs a vector of possible words and a vector of possible positions on board of each word
	//Example if given hook = "e......t", maxindex = 2, currentPosOnBoard = 4
	//function will return enter, { 4 } which means that enter can be placed at position 4 only
	//but elephant, { 2,4 }
	//as the e on board can be the first letter or the third letter in the word elephant, then pos on board will be 4 or 2 respectively
	vector<pair<string, vector<int>>> ContainsHookWithRackAtPos(string hook, string rack, int MaxIndex, int CurrPosOnBoard);

	//Function to check whether a string (Word)is in the dictionary
	//returns true if it was found
	//returns false if it wasnot found
	bool CheckWordInDict(string Word);
};