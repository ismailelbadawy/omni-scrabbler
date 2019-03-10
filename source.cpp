#pragma once
#include<iostream>
#include<vector>

#include<map>
#include"Move Generator/MoveGenerate.h"
#include"Models/Board.h"
#include"Models/Play.h"
#include"Models/Rack.h"
#include"Models/Tile.h"
#include"Models/Bag.h"
#include"GADDAG/GADDAG.h"

using namespace std;
string 	GADDAG_PATH = "assets/Dict.txt";
string 	BAG_PATH = "assets/letters.txt";
Board 	BOARD;
Bag   	BAG(BAG_PATH);
Rack  	RACK;
GADDAG	DAG(GADDAG_PATH);
void main(){

}