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
	board.Probe('a',8,7);
	board.Probe('t',9,7);
	board.Probe('u',7,8);
	board.Probe('t',7,9);
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

	ofstream OutputFile;

    OutputFile.open("results.txt");

	while(true){
		 start = chrono::high_resolution_clock::now();
		 moves = movGen.Generate(&rack, board);
		 end = chrono::high_resolution_clock::now();
		 for(int i = 0; i < (int)moves.size(); i++)
		 {
			 /* code */
			 OutputFile<< moves[i].GetPlay()->GetLetters()<< " " << moves[i].GetPlay()->GetRow() << " " << moves[i].GetPlay
			 ()->GetColumn() <<  " " << moves[i].GetPlay()->GetIsHorizontal() << endl;
		 }
		 
		 moves.clear();
	}

	cout<<"hi";
}