#include "Rack.h"



Rack::Rack()
{
}

Rack::Rack(Tile tiles[7])
{
    for (int i=0;i<7;i++)
        tiles_[i]=tiles[i];       
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