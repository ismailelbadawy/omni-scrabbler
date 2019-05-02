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