#include "Node.h"
#include <string>
#include <fstream>
#include <set>

class GADDAG {
    private:
	    Node* RootNode;
		
	    void SetRoot(Node *NewNode) {RootNode = NewNode;}

        //add new word to GADDAG
        void add(string NewWord);

        //recursive function to get all possible combinations
        void ContainsHookWithRackRecursive(Node* CurrentNode, set<string> &SetOfPossibleWords, string letters, string rack, string hook);

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
	    Node* GetRoot() {return RootNode;}

        //Function to be called to return a vector of all possible words 
        //it has parameters: hook (word on board to add letters to it) and rack (available letters)
        //if hook= ay and rack= persl --> vector={play, player, plays} 
        //hook can be sent as "" or " " to get all combinations based on the rack only (for the first move in the game)
	    vector<string> ContainsHookWithRack(string hook, string rack);
};