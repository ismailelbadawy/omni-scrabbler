#pragma once
#include <iostream>
#include<map>
#include<vector>
#include <string>
#include <fstream>
#include"Tile.h"
#include"Play.h"

using namespace std;

class Bag
{
    map<Tile,int> bag_;
    int remainingTiles_;

    public:
    Bag();
    Bag(string InputPath);
    void Initialize();
    int GetOccurences(char letter);
    int GetOccurences(Tile tile);
    int GetRemainingTiles();
    void TakeLetter(Tile* tile);
    void GiveTiles(vector<Tile> tiles);
    vector <char> GetRemainigLetters();
    ~Bag();
};