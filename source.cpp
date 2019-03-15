#pragma once
#include<iostream>
#include<vector>
#include<map>
#include"MoveGenerator/MoveGenerate.h"
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
	MoveGenerate movGen(GADDAG_PATH);		//A move generator insatance
	//BOARD.Probe('e',7,4);		//Inserting letters 'e' and 't' to the empty board
	//board.Probe('t',7,11);		//_ _ _ _ e _ _ _ _ _ _ t _ _ _---->Row 7
auto end = chrono::high_resolution_clock::now();
	
	double readingDictionaryTime = (end-start).count();
	Tile RackTiles[7];          //A simple array to carry the rack's letters


	RackTiles[0].SetLetter('p');
	RackTiles[1].SetLetter('q');
	RackTiles[2].SetLetter('z');
	RackTiles[3].SetLetter('g');
	RackTiles[4].SetLetter('k');
	RackTiles[5].SetLetter('t');
	RackTiles[6].SetLetter('s');
	
	Rack rack(RackTiles);



	while(true){
		vector<Play> moves = movGen.Generate(&rack, board);
	}

	cout<<"hi";
}