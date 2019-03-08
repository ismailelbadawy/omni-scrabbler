#include "GADDAG.h"

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

	for (int i = 1; i <= NewWord.length(); i++) {
		string Prefix = NewWord.substr(0, i);
		string Suffix = "";
		if (i != NewWord.length()) Suffix = NewWord.substr(i, NewWord.length() - (i));
		reverse(Prefix.begin(), Prefix.end());
		string AddWord = Prefix + Node::Break + Suffix + Node::EOW;

		Node * CurrentNode = RootNode;
		bool BreakFound = false;
		int j = 0;
		for (int i = 0;i<AddWord.length();i++) {
			//long words can be joined back together after the break point, cutting the tree size in half.
			if (BreakFound && PrevNode.size() > j) {
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

vector<string> GADDAG::ContainsHookWithRack(string hook, string rack) {
	transform(hook.begin(), hook.end(), hook.begin(), ::tolower);
	reverse(hook.begin(), hook.end());

	set<string> SetOfPossibleWords;
	vector<string> vectorOfPossibleWords;
	ContainsHookWithRackRecursive(RootNode, SetOfPossibleWords, "", rack, hook);
		
	set <string>::iterator k;
	for (k = SetOfPossibleWords.begin(); k != SetOfPossibleWords.end(); k++) {
		vectorOfPossibleWords.push_back(*k);
	}
	return vectorOfPossibleWords;
}

void GADDAG::ContainsHookWithRackRecursive(Node* CurrentNode, set<string> &SetOfPossibleWords, string letters, string rack, string hook) {
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
			ContainsHookWithRackRecursive(CurrentNode->AT(hook[0]), SetOfPossibleWords, letters, rack, NewHook);
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
				ContainsHookWithRackRecursive(CurrentNode->AT(Key), SetOfPossibleWords, letters, NewRack, hook);
			}
		}
	}
}

vector<pair<string, vector<int>>> GADDAG::ContainsHookWithRackAtPos(string hook, string rack, int pos, int CurrPosOnBoard) {
	transform(hook.begin(), hook.end(), hook.begin(), ::tolower);

	vector<pair<string, vector<int>>> VectorOfPossibleWords;

	if (RootNode->ContainsKey(hook[0])) {
		Node* newNode = RootNode->AT(hook[0]);
		ContainsHookWithRackRecursiveAtPos(newNode, VectorOfPossibleWords, "", rack, hook.erase(0,1), pos, 0, false, CurrPosOnBoard);
	}

	return VectorOfPossibleWords;
}

void GADDAG::ContainsHookWithRackRecursiveAtPos(Node* CurrentNode, vector<pair<string, vector<int>>> &VectorOfPossibleWords, string letters, string rack, string hook, int MaxPos, int CurrentCount, bool found, int CurrPosOnBoard) {
	if (found) {
		if ((CurrentNode!=NULL && CurrentNode->get_Letter() != Node::Break && CurrentNode->get_Letter()!= Node::EOW) || CurrentNode==NULL) {
			int MaxLength = 15 - CurrPosOnBoard + CurrentCount; // ----er- if currposonboard= 12 (enters)
																//currcount= 3 then maxlength= 15- 12 +3= 6 
			if (letters.length() - 1 > MaxLength)
				return;
		}
	}
	
	if (CurrentCount > MaxPos) return;

	bool NewFound=found;

	if (CurrentNode == NULL) {
		string Word = GetWord(letters);
		vector<pair<string, vector<int>>>::iterator it;
		for (it = VectorOfPossibleWords.begin(); it != VectorOfPossibleWords.end(); it++) {
			if (it->first ==  Word) break;
		}

		if (it == VectorOfPossibleWords.end()) { //word not found in Vector
			vector<int> Positions;
			Positions.push_back(CurrPosOnBoard-CurrentCount);
			VectorOfPossibleWords.push_back(pair<string, vector<int>>(Word, Positions));
		}
		else {
			it->second.push_back(CurrPosOnBoard - CurrentCount);
		}
		return;
	}
	if (hook[0] == '.') { //blank space on board
		int NewCount = CurrentCount;
		if (CurrentNode->get_Letter() != Node::Root)
			letters += CurrentNode->get_Letter();

		vector<char> vectorOfChildrenOfCurrentNode = CurrentNode->Keys();
		for (int i = 0; i < vectorOfChildrenOfCurrentNode.size(); i++) {
			NewFound = found;
			NewCount = CurrentCount;
			string NewHook = hook;
			char Key = vectorOfChildrenOfCurrentNode[i];

			if (Key == Node::Break) NewFound = true;
			if (!NewFound) NewCount++;

			if (NewFound && Key != hook[0] && hook[0]!='.') continue;

			std::size_t FoundInRack = rack.find(Key);
			if (Key == Node::EOW || Key == Node::Break || FoundInRack != std::string::npos) {
				string NewRack = rack;
				NewRack = (Key != Node::EOW && Key != Node::Break) ? NewRack.erase(FoundInRack, 1) : NewRack;
				if (Key != Node::Break && Key != Node::EOW && NewFound) NewHook.erase(0, 1);
				ContainsHookWithRackRecursiveAtPos(CurrentNode->AT(Key), VectorOfPossibleWords, letters, NewRack, NewHook, MaxPos, NewCount, NewFound, CurrPosOnBoard);
			}
		}
	}
	else if (hook!="") {
		int NewCount = CurrentCount;
		if (CurrentNode->get_Letter() != Node::Root) {
			letters += CurrentNode->get_Letter();
		}
		
		vector<char> vectorOfChildrenOfCurrentNode = CurrentNode->Keys();
		for (int i = 0; i < vectorOfChildrenOfCurrentNode.size(); i++) {
			NewFound = found;
			char Key = vectorOfChildrenOfCurrentNode[i];

			if (Key == Node::Break) NewFound = true;
			else if (Key != hook[0] && NewFound) continue;

			int NewCount = CurrentCount;
			if (!NewFound && Key != Node::Break && Key != Node::EOW) NewCount++;
			string NewHook = hook;
			if (Key == hook[0] && Key != Node::Break && Key!= Node::EOW) NewHook = NewHook.erase(0, 1);
			
			ContainsHookWithRackRecursiveAtPos(CurrentNode->AT(Key), VectorOfPossibleWords, letters, rack, NewHook, MaxPos, NewCount, NewFound, CurrPosOnBoard);
		}
	}
	else if (hook == "") {
		if (CurrentNode->get_Letter() != Node::Root)
			letters += CurrentNode->get_Letter();

		int NewCount= CurrentCount;

		vector<char> vectorOfChildrenOfCurrentNode = CurrentNode->Keys();
		for (int i = 0; i < vectorOfChildrenOfCurrentNode.size(); i++) {
			NewCount = CurrentCount;
			char Key = vectorOfChildrenOfCurrentNode[i];

			if (Key == Node::Break) NewFound = true;
			if (!NewFound) NewCount++;

			std::size_t FoundInRack = rack.find(Key);
			if (Key == Node::EOW || Key == Node::Break || FoundInRack != std::string::npos) {
				string NewRack = rack;
				NewRack = (Key != Node::EOW && Key != Node::Break) ? NewRack.erase(FoundInRack, 1) : NewRack;
				ContainsHookWithRackRecursiveAtPos(CurrentNode->AT(Key), VectorOfPossibleWords, letters, NewRack, hook, MaxPos, NewCount, NewFound, CurrPosOnBoard);
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
	for (int i = 0; i < Word.length(); i++)
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
}
bool GADDAG::CheckWordInDict(string Word)
{
	transform(Word.begin(), Word.end(), Word.begin(), ::tolower);
	reverse(Word.begin(), Word.end());
	Word += ">$";
	Node* CurrentNode = RootNode;
	for (int i = 0; i < Word.length(); i++)
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
