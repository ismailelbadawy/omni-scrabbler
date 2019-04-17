#pragma once
#include<iostream>
#include<vector>
#include"../Models/Board.h"
#include"../Models/Play.h"
#include"../Models/Rack.h"
#include"../Models/Move.h"
#include"../GADDAG/GADDAG.h"
#include"../Models/WordPossibility.h"


using namespace std;

class MoveGenerator{

	Tile* boardTiles_ [15][15];
    GADDAG *dag_;
    string rack_;
	vector<Move> moves_;
	vector<WordPossibility> rackpossibilities_;
public:    
    MoveGenerator();
    MoveGenerator(Board &board);
    ~MoveGenerator();
    vector<Move> Generate(const Rack *, Board &);

private:
    void GenerateWordsAtCols(Board &board);
    void GenerateWordsAtRows(Board &board);
	void GenerateRackWords(bool boardEmpty);
    void SetRack(Rack gameRack);
    bool CheckOtherDimension(Board board,string word,int row,int col,bool horizontal);
    void CheckWords(Board&board, vector<WordPossibility> returnedWords,int row,int col,bool horizontal);
	void LoopBoard(Board &board);
	bool WordIsTouching(Board &board, string word, int row, int column, bool horizontal);
	//Function to be called to return a vector of all possible words 
	//it has parameters: hook (word on board to add letters to it) and rack (available letters)
	//if hook= ay and rack= persl --> vector={play, player, plays} 
	//hook can be sent as "" or " " to get all combinations based on the rack only (for the first move in the game)
	vector<WordPossibility> ContainsHookWithRack(string hook, string rack);


    //recursive function to get all possible combinations
	void ContainsHookWithRackRecursive(Node* CurrentNode, vector<WordPossibility> &VectorOfPossibleWords, string letters, string rack, string hook);
    
    //recursive function to get all possible combinations such that the first letter of the hook exists at [0..MaxPos] only
	//the function handles the spaces between letters represented by a dot '.'
	//it handles maxLength of the possible words too
	void ContainsHookWithRackRecursiveAtPos(Node* CurrentNode, vector<WordPossibility> &VectorOfPossibleWords, string letters, string rack, string hook, int MaxPos, int CurrentCount, bool found, int CurrPosOnBoard);

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
	vector<WordPossibility> ContainsHookWithRackAtPos(string hook, string rack, int MaxIndex, int CurrPosOnBoard);

	// A function that calculates the single play's score depending on the newly added letters and their bonuses

	void CalculatePlayScore(Play *p);

};