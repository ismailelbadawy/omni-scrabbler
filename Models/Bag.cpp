#include "Bag.h"
#include "Tile.h"

Bag::Bag()
{
}

Bag::Bag(string InputPath)
{
    ifstream InputFile;

    InputFile.open(InputPath);
    if (InputFile.is_open())
    {
        string Line;

        while (getline(InputFile, Line, ' '))
        {
            char letter = Line[0];
            getline(InputFile, Line, ' ');
            int occurence = stoi(Line);
            getline(InputFile, Line, '\n');
            int score = stoi(Line);
            Tile t;
            t.SetParams(letter, -1, -1, score, 1);
            bag_.insert(pair<Tile, int>(t, occurence));
        }
    }
}
int Bag::GetOccurences(char letter)
{
    Tile tile;
    tile.SetParams(letter, 0, 0, 0, 0);

    return bag_.at(tile);
}
int Bag::GetOccurences(Tile tile)
{
    return bag_.at(tile);
}
vector<Tile> Bag::GetRemainingTiles()
{
    vector<Tile> remTiles;
    map<Tile, int>::iterator it;

    for (it = bag_.begin(); it != bag_.end(); it++)
    {
        if (it->second > 0)
        {
            remTiles.push_back(it->first);
        }
    }
    return remTiles;
}

void Bag::TakeLetter(Tile tile)
{
    //if the occurences > 0 then we can draw it.
    if (bag_.at(tile) > 0)
    {
        bag_.at(tile) -= 1;
    }
}

void Bag::TakeLetter(char letter)
{
}


void Bag::GiveTiles(vector<Tile> tiles)
{
    for (int i = 0; i < (int)tiles.size(); i++)
    {
        this->bag_[tiles.at(i)]++;
    }
}

map<Tile,int> Bag::getBag(){
    return this->bag_;
}

vector <char> Bag::GetRemainigLetters(){
    map<Tile,int> :: iterator it;
    vector <char> RemainingLetters;
    for (it = bag_.begin(); it != bag_.end(); it++) {
        Tile tile= it->first;
        char letter = tile.GetLetter();
        int occ = it->second;

        for (int occur = 0; occur < occ ; occur++){
            RemainingLetters.push_back(letter);
        }
    }
    return RemainingLetters;
}

Bag::~Bag()
{
}