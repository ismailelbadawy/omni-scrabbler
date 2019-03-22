#include <iostream>
#include<vector>
#include"Rack.h"

using namespace std;


class WordPossibility{
    string word_;
    vector<int> positions_;
    vector<Rack> remainingracks_;

public:
WordPossibility();
WordPossibility(string word);
void SetPosition(int position);
void SetWord(string word);
void AddRacks(string possibleRack);
void AddPositions(int position);
string GetWord();
vector <int> GetPositions();
int GetPosition(int i);
vector <Rack> GetRemainingRacks();
~WordPossibility();

};