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
    vector<Tile> GetRemainingTiles();
    void TakeLetter(Tile tile);
    void TakeLetter(char letter);
    void GiveTiles(vector<Tile> tiles);
    map<Tile,int> getBag();
    vector <char> GetRemainigLetters();
    ~Bag();
};