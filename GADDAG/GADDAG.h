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
        void ContainsHookWithRankRecursive(Node* CurrentNode, set<string> &SetOfPossibleWords, string letters, string rack, string hook);

        //adjust word to be added in the list of possible combinations
	    string GetWord(string str);

    public:
        //constructor
	    GADDAG(){RootNode = new Node(Node::Root);}

        //get RootNode
	    Node* GetRoot() {return RootNode;}

        //Function to be called to return a vector of all possible words 
        //it has parameters: hook (word on board to add letters to it) and rack (available letters)
        //if hook= ay and rack= persl --> vector={play, player, plays} 
	    vector<string> ContainsHookWithRank(string hook, string rack);

        //Function to read the dictionary represented as an input file 
        //in the assets folder, it takes the inputsPath and loads the GADDAG
        void LoadDag(string InputPath);
};