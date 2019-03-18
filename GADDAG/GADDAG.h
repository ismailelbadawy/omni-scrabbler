#pragma once
#include "Node.h"
#include <string>
#include <fstream>
#include <set>

class GADDAG {
private:
	Node* RootNode;

	void SetRoot(Node *NewNode) { RootNode = NewNode; }

	//add new word to GADDAG
	void add(string NewWord);


	//Load dag with dictionary hard coded
	void LoadDagHardCoded();

	//Function to read the dictionary represented as an input file 
	//in the assets folder, it takes the inputPath and loads the GADDAG
	void LoadDag(string InputPath);

public:
	//constructor to initialize Root and load dictionary
	GADDAG(); //uses LoadDagHardCoded Function
	GADDAG(string InputPath); //uses LoadDag function and requires the input path of dictionary

	//get RootNode
	Node* GetRoot() { return RootNode; }




	//Function to check whether a string (Word)is in the dictionary
	//returns true if it was found
	//returns false if it wasnot found
	bool CheckWordInDict(string Word);



	//adjust word to be added in the list of possible combinations
	string GetWord(string str);
};