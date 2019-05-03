#include "Bag.h"
#include "Tile.h"

Bag::Bag()
{
}

Bag::Bag(string InputPath)
{
    ifstream InputFile;
    remainingTiles_=0;
    InputFile.open(InputPath);
    if (InputFile.is_open())
    {
        string Line;

        while (getline(InputFile, Line, ' '))
        {
            char letter = Line[0];
            letter = tolower(letter);
            getline(InputFile, Line, ' ');
            int occurence = stoi(Line);
            getline(InputFile, Line, '\n');
            int score = stoi(Line);
            Tile t;
            t.SetParams(letter, -1, -1, score, 1);
            bag_.insert(pair<Tile, int>(t, occurence));
              remainingTiles_+=occurence;
          
        }
    }
    this->remainingTiles_ = 100;
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
vector<Tile> Bag::GetRemainingTiles() //return actual tiles, if occ of one tile is 2, it's returned 2 times
{
    vector<Tile> remTiles;
    map<Tile, int>::iterator it;

    for (it = bag_.begin(); it != bag_.end(); it++)
    {
        for (int k=0; k < it->second; k++) 
        {
            remTiles.push_back(it->first);
        }
    }
    this->remainingTiles_ = remTiles.size();
    return remTiles;
}

void Bag::TakeLetter(Tile tile)
{
    //if the occurences > 0 then we can draw it.
    if (bag_.at(tile) > 0){
        bag_.at(tile) -= 1;
        this->remainingTiles_ --;
    }
}

void Bag::TakeLetter(const char letter)
{
    vector<Tile> remTiles;
    map<Tile, int>::iterator it;
    for (it = bag_.begin(); it != bag_.end(); it++)
    {
        Tile t = it->first;
        if (t.GetLetter()==letter )
        {
            it->second--; 
            this->remainingTiles_--;   
        }
    }

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
    this->remainingTiles_ = RemainingLetters.size();
    return RemainingLetters;
}

Bag::~Bag()
{
}