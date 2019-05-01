#include"Bag.h"
Bag::Bag(){

}

Bag::Bag(string InputPath){
 ifstream InputFile;

    InputFile.open(InputPath);
    if (InputFile.is_open()) 
    {
        string Line;
    
        while (getline(InputFile, Line,' ')) 
        {
			char letter = Line[0];
            getline(InputFile,Line,' ');
            int occurence = stoi(Line);
            getline(InputFile,Line,'\n');
            int score=stoi(Line);
            Tile t;
            t.SetParams(letter,-1,-1,score,1);
            bag_.insert(pair<Tile,int>(t,occurence));
        }
    
    }
}
int Bag::GetOccurences(char letter)
{
    Tile tile;
    tile.SetParams(letter,0,0,0,0);

return bag_.at(tile);
}
int Bag::GetOccurences(Tile tile){
return bag_.at(tile);
}
int Bag::GetRemainingTiles(){


return 0;
}

void Bag::TakeLetter(Tile* tile){  
}


void Bag::GiveTiles(vector<Tile> tiles){
for (int i =0;i<(int)tiles.size();i++){
       this->bag_[tiles.at(i)]++;
   }
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
}

Bag::~Bag(){

}