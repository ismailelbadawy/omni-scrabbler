#include"GADDAG.h"
#include "dictionary.h"

GADDAG:: GADDAG() {
	RootNode = new Node(Node::Root);
	LoadDagHardCoded();
}

GADDAG::GADDAG(string InputPath) {
	RootNode = new Node(Node::Root);
	LoadDag(InputPath);
}

void GADDAG::add(string NewWord) {

	transform(NewWord.begin(), NewWord.end(), NewWord.begin(), ::tolower);
	vector <Node*> PrevNode;

	for (int i = 1; i <=(int) NewWord.length(); i++) {
		string Prefix = NewWord.substr(0, i);
		string Suffix = "";
		if (i !=(int) NewWord.length()) Suffix = NewWord.substr(i, NewWord.length() - (i));
		reverse(Prefix.begin(), Prefix.end());
		string AddWord = Prefix + Node::Break + Suffix + Node::EOW;

		Node * CurrentNode = RootNode;
		bool BreakFound = false;
		int j = 0;
		for (int i = 0;i<(int)AddWord.length();i++) {
			//long words can be joined back together after the break point, cutting the tree size in half.
			if (BreakFound && (int)PrevNode.size() > j) {
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



string GADDAG::GetWord(string str) {
	string Word = "";
	std::size_t IndexofBreak = str.find(Node::Break);

	for (int i = IndexofBreak - 1; i >= 0; i--)
		Word += str[i];

	for (int i = IndexofBreak + 1; i < (int)str.length(); i++)
		Word += str[i];

	return Word;
}

void GADDAG::LoadDag(string InputPath){
    ifstream InputFile;

    InputFile.open(InputPath);
    if (InputFile.is_open()) {
        string Line;
        while (getline(InputFile, Line)) {
			this->add(Line);
        }
    }
}

bool GADDAG::CheckWordInDict(string Word)
{
	transform(Word.begin(), Word.end(), Word.begin(), ::tolower);
	reverse(Word.begin(), Word.end());
	Word += ">$";
	Node* CurrentNode = RootNode;
	for (int i = 0; i <(int) Word.length(); i++)
	{
		if (CurrentNode->ContainsKey(Word[i]))
		{
			CurrentNode = CurrentNode->AT(Word[i]);
		}
		else
		{
			return false;
		}
	}
	return true;
}

void GADDAG::LoadDagHardCoded() { 
	for(int i = 0; i < 267753; i ++){
		this->add(DICTIONARY[i]);
	}
}