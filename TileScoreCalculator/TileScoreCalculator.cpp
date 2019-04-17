#include "TileScoreCalculator.h"


TileScoreCalculator::TileScoreCalculator()
{
    scoreMap_['a'] = 1;
    scoreMap_['b'] = 3;
    scoreMap_['c'] = 3;
    scoreMap_['d'] = 2;
    scoreMap_['e'] = 1;
    scoreMap_['f'] = 4;
    scoreMap_['g'] = 2;
    scoreMap_['h'] = 4;
    scoreMap_['i'] = 1;
    scoreMap_['j'] = 8;
    scoreMap_['k'] = 5;
    scoreMap_['l'] = 1;
    scoreMap_['m'] = 3;
    scoreMap_['n'] = 1;
    scoreMap_['o'] = 1;
    scoreMap_['p'] = 3;
    scoreMap_['q'] = 10;
    scoreMap_['r'] = 1;
    scoreMap_['s'] = 1;
    scoreMap_['t'] = 1;
    scoreMap_['u'] = 1;
    scoreMap_['v'] = 4;
    scoreMap_['w'] = 4;
    scoreMap_['x'] = 8;
    scoreMap_['y'] = 4;
    scoreMap_['z'] = 10;
    scoreMap_['?'] = 0;
}

int TileScoreCalculator::GetTileScore(char c)
{
    if(c != '?')
    {
        // Switch character to lower case.
        if(c < 97)
        {
            c += 32;
        }
    }
    if((c >= 97 && c <= 122) || c == '?')
    {
        return scoreMap_[c];
    }
    return -1;
}

TileScoreCalculator::~TileScoreCalculator(){
    
}