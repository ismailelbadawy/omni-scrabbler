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


<<<<<<< HEAD
void main(){
=======
	// for (int i = 0; i < possible.size(); i++) {
	// 	cout << possible[i] << endl;
	// }
	cout<<"Hello World"<<endl;
	string InputPath = "assets/Dict.txt";
	string Bagpath = "assets/letters.txt";
	Bag bag(Bagpath);
	int a = bag.GetOccurences('a');
	GADDAG Dag(InputPath);
	//string Hook = "arr...es..";

    vector<pair<string, vector<int>>> possible;
	
//	possible = Dag.ContainsHookWithRackAtPos(Hook,Rack, 2, 4);

	for (int i = 0; i < (int)possible.size(); i++) {
		cout << possible[i].first << " at positions: ";
		for (int j = 0; j <(int) possible[i].second.size(); j++)
			cout << possible[i].second[j] << " ";
		cout << endl;
	}
	bool check = Dag.CheckWordInDict("zebra");
	cout << check;
	/////////////////////////////////////Khaled's Testing Part//////////////////////////////////////////////////////////////
	MoveGenerate movGen(InputPath);		//A move generator insatance
	board.Probe('e',7,4);		//Inserting letters 'e' and 't' to the empty board
	board.Probe('t',7,11);		//_ _ _ _ e _ _ _ _ _ _ t _ _ _---->Row 7
>>>>>>> f3cd685a8e549516d7171a69f1463fb0fd393c0e

	Tile RackTiles[7];          //A simple array to carry the rack's letters

	RackTiles[0].SetLetter['p'];
	RackTiles[1].SetLetter['n'];
	RackTiles[2].SetLetter['o'];
	RackTiles[3].SetLetter['h'];
	RackTiles[4].SetLetter['e'];
	RackTiles[5].SetLetter['l'];
	RackTiles[6].SetLetter['a'];
	
	Rack rack(RackTiles);

	movGen.set_Rack(rack);

	movGen.send_Row(board);
}