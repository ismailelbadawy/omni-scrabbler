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
	MoveGenerator movGen(board);		//A move generator insatance
	auto end = chrono::high_resolution_clock::now();
	board.Probe('c',7,7);		//Inserting letters 'e' and 't' to the empty board
	
	//double readingDictionaryTime = (end-start).count();
	Tile RackTiles[7];          //A simple array to carry the rack's letters


	RackTiles[0].SetLetter('o');
	RackTiles[1].SetLetter('t');
	RackTiles[2].SetLetter('f');
	RackTiles[3].SetLetter('p');
	RackTiles[4].SetLetter('w');
	RackTiles[5].SetLetter('q');
	RackTiles[6].SetLetter('z');
	
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