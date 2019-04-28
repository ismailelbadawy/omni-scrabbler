#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "MoveGenerator/MoveGenerator.h"
#include "Evaluators/MidgameEvaluator.h"
#include "Models/Board.h"
#include "Models/Play.h"
#include "Models/Rack.h"
#include "Models/Tile.h"
#include "Models/Bag.h"
#include "GADDAG/GADDAG.h"
#include <time.h>
#include <chrono>
#include "./MonteCarlo/MonteCarlo.h"

using namespace std;
string GADDAG_PATH = "assets/Dict.txt";
string BAG_PATH = "assets/letters.txt";
Board board;
//
Rack RACK;
//GADDAG	DAG(GADDAG_PATH);

int main()
{

	Bag bag(BAG_PATH);

	board.Probe('c', 7, 2, 3); //Inserting letters 'e' and 't' to the empty board
	board.Probe('a', 7, 3, 1);
	board.Probe('t', 7, 4, 1);
	board.Probe('a', 7, 5, 1);
	board.Probe('s', 7, 6, 1);
	board.Probe('t', 7, 7, 1);
	board.Probe('r', 7, 8, 1);
	board.Probe('o', 7, 9, 1);
	board.Probe('p', 7, 10, 3);
	board.Probe('h', 7, 11, 4);
	board.Probe('i', 7, 12, 1);
	board.Probe('c', 7, 13, 3);
	Tile RackTiles[7]; //A simple array to carry the rack's letters

	RackTiles[0].SetLetter('a');
	RackTiles[0].SetScore(1);
	RackTiles[1].SetLetter('w');
	RackTiles[1].SetScore(4);
	RackTiles[2].SetLetter('c');
	RackTiles[2].SetScore(3);
	RackTiles[3].SetLetter('d');
	RackTiles[3].SetScore(2);
	RackTiles[4].SetLetter('e');
	RackTiles[4].SetScore(1);
	RackTiles[5].SetLetter('h');
	RackTiles[5].SetScore(4);
	RackTiles[6].SetLetter('p');
	RackTiles[6].SetScore(3);

	vector<Tile> rackTiles(RackTiles, RackTiles + sizeof RackTiles / sizeof RackTiles[0]);
	Rack rack(rackTiles);

	//opRAck
	Rack oprack(rackTiles);

	vector<Move> moves;
	cout << "Loading GADDAG...\n";
	auto startDag = chrono::high_resolution_clock::now();
	MoveGenerator movGen(board);
	cout << "Hi world";
	ofstream OutputFile;

	OutputFile.open("results.txt");

	while (true)
	{
		auto start = chrono::high_resolution_clock::now();
		moves = movGen.Generate(&rack, board.GetCount() == 0);
		auto end = chrono::high_resolution_clock::now();

		for (int i = 0; i < (int)moves.size(); i++)
		{
			/* code */
			OutputFile << moves[i].GetPlay()->GetLetters() << " " << moves[i].GetPlay()->GetRow() << " " << moves[i].GetPlay()->GetColumn() << " " << moves[i].GetPlay()->GetIsHorizontal() << " " << moves[i].GetPlay()->GetScore() << endl;
		}
		// board.SimulateMove(&moves[0]);
		// moves[0].GetPlay()->GetScore();
		break;
		//moves.clear();
	}

	vector<Move> simVec;
	for (int i = 0; i < 10; i++)
	{
		simVec.push_back(moves.at(i));
	}

	MonteCarlo testTree(board, simVec, rack, oprack, bag);

	NodeMC *node = testTree.Simulation();
	vector<Tile> remTiles = bag.GetRemainingTiles();

	// testTree.LevelOrderTraversal(testTree.Root);
	// board.SimulateMove(&moves[0]);
	// testTree.Root->child.push_back(testTree.newNode(board));
	cout << "/////////////////////////////" << endl;
	// testTree.LevelOrderTraversal(testTree.Root); //child.push_back(board);
}