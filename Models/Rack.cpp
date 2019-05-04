#include "Rack.h"



Rack::Rack()
{
}

Rack::Rack(vector<Tile> tiles)
{
    for (int i=0;i<7;i++)
        tiles_.push_back(tiles[i]);       
}
Rack::Rack(string rack){
    for(int i = 0; i < (int)rack.size(); i++)
    {
        Tile t;
        t.SetParams(rack[i],-1,-1,1,1);
        tiles_.push_back(t);
    }
    

}

Rack::~Rack()
{
}

void Rack::SetTile(char c,int loc)
{
    tiles_[loc].SetLetter(c);
}

char Rack::GetLetter(int loc)
{
    return (tiles_[loc].GetLetter());
}

int Rack::GetLength(){
    return tiles_.size();
}

void Rack::generateRanRack(Rack rack){
    
}

vector <Tile> Rack::GetRackTiles(){
    return tiles_;
}

void Rack::SetRackTiles(vector<Tile> Tiles){
    this->tiles_ = Tiles;
}

void Rack::RemoveTile(Tile tile){
    for (int i=0; i < (int) tiles_.size(); i++){
        if (tiles_[i].GetLetter() == tile.GetLetter()){
            tiles_.erase(tiles_.begin()+i);
            break;
        }
    }
}

void Rack::RemoveTile(char letter){
    for (int i=0; i < (int) tiles_.size(); i++){
        if (tiles_[i].GetLetter() == letter){
            tiles_.erase(tiles_.begin()+i);
            break;
        }
    }
}

Tile Rack::RemoveAndReturnTile(char letter){
    for (int i=0; i < (int) tiles_.size(); i++){
        if (tiles_[i].GetLetter() == letter){
            Tile t= tiles_[i];
            tiles_.erase(tiles_.begin()+i);
            return t;
        }
    }
}

string Rack::RackToString(){
    string RackString ="";
    for (int i=0; i< (int) tiles_.size(); i++){
        RackString += tiles_[i].GetLetter();
    }
    return RackString;
}

Tile Rack::GetTile(int pos){
    return tiles_[pos];
}