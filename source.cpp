#pragma once
#include<iostream>
#include<vector>
#include<map>
#include"MoveGenerator/MoveGenerator.h"
#include"Models/Board.h"
#include"Models/Play.h"
#include"Models/Rack.h"
#include"Models/Tile.h"
#include"Models/Bag.h"
#include"GADDAG/GADDAG.h"
#include <time.h>
#include <chrono>


using namespace std;
string 	GADDAG_PATH = "assets/Dict.txt";
string 	BAG_PATH = "assets/letters.txt";
Board 	board;
Bag   	BAG(BAG_PATH);
Rack  	RACK;
//GADDAG	DAG(GADDAG_PATH);

int main(){
	// for (int i = 0; i < possible.size(); i++) {
	// 	cout << possible[i] << endl;
	// }

	/////////////////////////////////////Khaled's Testing Part//////////////////////////////////////////////////////////////
	auto start = chrono::high_resolution_clock::now();
	MoveGenerator movGen(GADDAG_PATH);		//A move generator insatance
	auto end = chrono::high_resolution_clock::now();
	board.Probe('e',7,4);		//Inserting letters 'e' and 't' to the empty board
	board.Probe('t',7,11);		//_ _ _ _ e _ _ _ _ _ _ t _ _ _---->Row 7
    
	
	double readingDictionaryTime = (end-start).count();
	Tile RackTiles[7];          //A simple array to carry the rack's letters


	RackTiles[0].SetLetter('g');
	RackTiles[1].SetLetter('f');
	RackTiles[2].SetLetter('z');
	RackTiles[3].SetLetter('o');
	RackTiles[4].SetLetter('r');
	RackTiles[5].SetLetter('e');
	RackTiles[6].SetLetter('a');
	
	Rack rack(RackTiles);



	while(true){
		 start = chrono::high_resolution_clock::now();
		vector<Play> moves = movGen.Generate(&rack, board);
		 end = chrono::high_resolution_clock::now();
	}

	cout<<"hi";
}