#include "GADDAG.h"

vector<string> GADDAG::ContainsHookWithRank(string hook, string rack) {
	transform(hook.begin(), hook.end(), hook.begin(), ::tolower);
	reverse(hook.begin(), hook.end());

	set<string> SetOfPossibleWords;
	vector<string> vectorOfPossibleWords;
	ContainsHookWithRankRecursive(RootNode, SetOfPossibleWords, "", rack, hook);
		
	set <string>::iterator k;
	for (k = SetOfPossibleWords.begin(); k != SetOfPossibleWords.end(); k++) {
		vectorOfPossibleWords.push_back(*k);
	}
	return vectorOfPossibleWords;
}

void GADDAG::add(string NewWord){

	transform(NewWord.begin(), NewWord.end(), NewWord.begin(), ::tolower);
    vector <Node*> PrevNode;
		
	for (int i = 1; i <= NewWord.length(); i++){
		string Prefix = NewWord.substr(0, i);
		string Suffix = "";
		if (i != NewWord.length()) Suffix = NewWord.substr(i, NewWord.length() - (i)); 
		reverse(Prefix.begin(), Prefix.end());
		string AddWord = Prefix+Node::Break + Suffix + Node::EOW;

		Node * CurrentNode = RootNode;
		bool BreakFound = false;
		int j = 0;
		for (int i = 0;i<AddWord.length();i++)	{
			//long words can be joined back together after the break point, cutting the tree size in half.
			if (BreakFound && PrevNode.size() > j){
				CurrentNode->AddChild(AddWord[i], PrevNode[j]);
				break;
			}

			CurrentNode = CurrentNode->AddChild(AddWord[i]);

			if (PrevNode.size() == j)
				PrevNode.push_back(CurrentNode);

			if (AddWord[i] == Node::Break)
				BreakFound = true;
			j++;
		}
	}
}

void GADDAG::ContainsHookWithRankRecursive(Node* CurrentNode, set<string> &SetOfPossibleWords, string letters, string rack, string hook) {
	if (CurrentNode == NULL) {
		string Word = GetWord(letters);
		set<string>::iterator it;
		it = SetOfPossibleWords.find(Word);
		if (it == SetOfPossibleWords.end()) { //word not found in set
			SetOfPossibleWords.insert(Word);
		}
		return;
	}

	if (hook != "" && hook != " ") {
		if (CurrentNode->get_Letter() != Node::Root)
			letters += CurrentNode->get_Letter();

		if (CurrentNode->ContainsKey(hook[0])) {
			string NewHook = hook;
			NewHook = NewHook.erase(0, 1);
			ContainsHookWithRankRecursive(CurrentNode->AT(hook[0]), SetOfPossibleWords, letters, rack, NewHook);
			}
		}
	else {
		if (CurrentNode->get_Letter() != Node::Root)
			letters += CurrentNode->get_Letter();

		vector<char> vectorOfChildrenOfCurrentNode = CurrentNode->Keys();
		for (int i = 0; i < vectorOfChildrenOfCurrentNode.size(); i++) {
			char Key = vectorOfChildrenOfCurrentNode[i];

			std::size_t FoundInRack = rack.find(Key);
			if (Key == Node::EOW || Key == Node::Break || FoundInRack != std::string::npos) {
				string NewRack = rack;
				NewRack = (Key != Node::EOW && Key != Node::Break) ? NewRack.erase(FoundInRack, 1) : NewRack;
				ContainsHookWithRankRecursive(CurrentNode->AT(Key), SetOfPossibleWords, letters, NewRack, hook);
			}
		}
    }
}

string GADDAG::GetWord(string str) {
	string Word = "";
	std::size_t IndexofBreak = str.find(Node::Break);

	for (int i = IndexofBreak - 1; i >= 0; i--)
		Word += str[i];

	for (int i = IndexofBreak + 1; i < str.length(); i++)
		Word += str[i];

	return Word;
}

void GADDAG::LoadDag(string InputPath){
    ifstream InputFile;
    vector<string> Dictionary;
    
    InputFile.open(InputPath);
    if (InputFile.is_open()) {
        string Line;
        while (getline(InputFile, Line)) {
            Dictionary.push_back(Line);
        }
    }
    Dictionary;
	for (int i = 0; i < Dictionary.size(); i++) {
		this->add(Dictionary[i]);
	}
}