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
	board.Probe('c',7,7);		//Inserting letters 'e' and 't' to the empty board
	board.Probe('e',8,7);
	board.Probe('a',9,7);
	board.Probe('s',10,7);
	board.Probe('e',11,7);
	//double readingDictionaryTime = (end-start).count();
	Tile RackTiles[7];          //A simple array to carry the rack's letters


	RackTiles[0].SetLetter('g');
	RackTiles[1].SetLetter('f');
	RackTiles[2].SetLetter('e');
	RackTiles[3].SetLetter('d');
	RackTiles[4].SetLetter('c');
	RackTiles[5].SetLetter('b');
	RackTiles[6].SetLetter('a');
	
	Rack rack(RackTiles);
	vector<Move> moves;


	while(true){
		 start = chrono::high_resolution_clock::now();
		 moves = movGen.Generate(&rack, board);
		 end = chrono::high_resolution_clock::now();
		 for(int i = 0; i < (int)moves.size(); i++)
		 {
			 /* code */
			 cout<<moves[i].GetPlay()->GetLetters()<<endl;
		 }
		 
		 moves.clear();
	}

	cout<<"hi";
}