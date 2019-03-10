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
Board board;
map<Tile,int> m;

void main(){
	// string InputPath = "../assets/Dict.txt";
    // GADDAG Dag(InputPath);

	// string Hook = "";
	// string Rack = "abcdefghijklmnopqrstuvwxyz";

    // vector <string> possible;
	// possible = Dag.ContainsHookWithRack(Hook,Rack);

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
	string Hook = "e......t";
	string Rack = "aaabrrcdefgdhijklsswwwrrrt";

    vector<pair<string, vector<int>>> possible;
	
	possible = Dag.ContainsHookWithRackAtPos(Hook,Rack, 2, 4);

	for (int i = 0; i < (int)possible.size(); i++) {
		cout << possible[i].first << " at positions: ";
		for (int j = 0; j <(int) possible[i].second.size(); j++)
			cout << possible[i].second[j] << " ";
		cout << endl;
	}
	bool check = Dag.CheckWordInDict("zebra");
	cout << check;

}