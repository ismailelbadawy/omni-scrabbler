#include "Node.h"

Node::Node(char letter){this->Letter = letter;}

Node* Node::AT(char index){return Children.at(index);}

void Node::set_Letter(char L) {Letter = L;}
char Node::get_Letter() {return Letter;}

vector<char> Node::Keys() {
	vector<char> vectorOfChildren;
	for (unordered_map<char, Node*>::iterator it = Children.begin(); it != Children.end(); ++it) 
		vectorOfChildren.push_back(it->first);
		
	return vectorOfChildren;
}


bool Node::ContainsKey(char key) {
	if (Children.find(key) == Children.end())
		return false;
	else 
		return true;
}

Node* Node::AddChild(char letter) {
	if (!ContainsKey(letter)) {
		Node* node = letter != EOW ? new Node(letter) : NULL;
		Children.insert(pair<char, Node*>(letter, node));
		return node;
	}
		
	return Children[letter];
}

Node* Node::AddChild(char letter, Node* node){
	if (!ContainsKey(letter)) {
		Children.insert(pair<char, Node*>(letter, node));
		return node;
	}

	return Children[letter];
}