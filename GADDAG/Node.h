#include <unordered_map>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class Node {
    private:
	    char Letter;
	    unordered_map<char, Node*> Children;

    public:
	    static const char Break = '>';
	    static const char EOW = '$';
	    static const char Root = ' ';

        //Constructors
	    Node() {};
	    Node(char letter);

        //setters and getters of letter
	    void set_Letter(char L);
	    char get_Letter();

        //returns pointer to the child with letter= "index"
	    Node* AT(char index);

        //return a vector containing all the children of a node
    	vector<char> Keys();

        //checks whether a node has a child with the value of "key" or not 
	    bool ContainsKey(char key);

        //Add new letter to GADDAG
	    Node* AddChild(char letter);
	    Node* AddChild(char letter, Node* node);        
};