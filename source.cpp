#pragma once

#include<iostream>
#include<vector>
#include<map>
#include"MoveGenerator/MoveGenerator.h"
#include"Evaluators/MidgameEvaluator.h"
#include"Models/Board.h"
#include"Models/Play.h"
#include"Models/Rack.h"
#include"Models/Tile.h"
#include"Models/Bag.h"
#include"Models/Agent.h"
#include"GADDAG/GADDAG.h"
#include "Evaluators/MidgameEvaluator.h"

#include "Evaluators/PreendgameEvaluator.h"
#include "Strategy/SuperLeaveLoader.h"
#include <time.h>
#include <chrono>

using namespace std;
string 	GADDAG_PATH = "assets/Dict.txt";
string 	BAG_PATH = "assets/letters.txt";
Board 	board;
//
Rack  	RACK;
//GADDAG	DAG(GADDAG_PATH);

int main(){
	Bag bag(BAG_PATH);

	board.Probe('c',7,2,3);		//Inserting letters 'e' and 't' to the empty board
	board.Probe('a',7,3,1);
	board.Probe('t',7,4,1);
	board.Probe('a',7,5,1);
	board.Probe('s',7,6,1);
	board.Probe('t',7,7,1);
	board.Probe('r',7,8,1);
	board.Probe('o',7,9,1);
	board.Probe('p',7,10,3);
	board.Probe('h',7,11,4);
	board.Probe('i',7,12,1);
	board.Probe('c',7,13,3);

	Tile RackTiles[7];          //A simple array to carry the rack's letters

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

  vector<Move> moves;

	cout << "Loading GADDAG...\n";
	auto startDag = chrono::high_resolution_clock::now();
	MoveGenerator movGen(board);
	auto endDag = chrono::high_resolution_clock::now();
	auto diff = endDag - startDag;
	cout << "Time to load GADDAG: " << chrono::duration_cast<chrono::seconds>(diff).count() << " s" << endl;

	MidgameEvaluator* evaluator = NULL;
	
	map<string, double>* syn2 = new map<string, double>();
	map<char, double>* worth = new map<char, double>();
	SuperLeaveLoader loader(syn2, worth, "assets/syn2", "assets/worths");
	cout << "Loaded the rack leave map with count " << syn2->size() << endl;
	
	ofstream OutputFile;
	OutputFile.open("results.txt");

	//FOR TESTING OPPONENT RACK ONLY
	//PreEval.OpponentRackEstimation();

	//char c = 'y';
	bool Human = 0; //should be received from server
	if (Human == 0){//Agent Mode
		Agent AI_Agent(&board, &bag, &rack);

		bool GameOver= false; //should be over when all 100 tiles are played or when we pass and opponent pass 
		while(!GameOver){
			bool MyTurn= true;
			if (MyTurn){
				auto  start = chrono::high_resolution_clock::now();
				moves = movGen.Generate(&rack,board, board.GetCount()==0);
				auto  end = chrono::high_resolution_clock::now();

				int BagSize = 40;
				if (BagSize > 9){//MidGame
					AI_Agent.MidGame(moves, syn2, worth); //should return best move
					
				}
				else if (BagSize > 0 && BagSize <=9){
					AI_Agent.PreEndGame(syn2,&movGen,moves); //should return best move
	
				}
				else if (BagSize == 0){
					AI_Agent.EndGame(moves); //should return best move
				}
				moves.clear();
			}
			
		}	

		//PreendgameEvaluator PreEval = PreendgameEvaluator(syn2,&board,&movGen,moves);
		//cout << &moves << endl;
		//evaluator = new MidgameEvaluator(&moves, &board, syn2, worth);
		//vector<Move> * evaluatedMoves = evaluator->Evaluate();
		//Move * move = new Move();
		//PreEval.Evaluate(move);
		//cout << moves.size() << endl;

		//for(int i = 0; i < (int)evaluatedMoves->size(); i++){
			//Move * move = &(evaluatedMoves->at(i));
			//cout << "Move (" << i << ") " << move->GetPlay()->GetLetters() << "\tLeaving " << move->GetRack() << "\tHeuristic : " << move->GetHeuristic() << std::endl;
		//}

		//for(int i = 0; i < (int)moves.size(); i++){
			 /* code */
			//OutputFile<< moves[i].GetPlay()->GetLetters()<< " " << moves[i].GetPlay()->GetRow() << " " << moves[i].GetPlay()->GetColumn() <<  " " << moves[i].GetPlay()->GetIsHorizontal() <<" " <<moves[i].GetPlay()->GetScore() <<endl;
		 //}

		 // board.SimulateMove(&moves[0]);

		//  moves[0].GetPlay()->GetScore();

		  //moves.clear();

			//cout << "More moves?\n";

			//cin >> c;



		//delete evaluatedMoves;
	}
	else { //Human Mode

	}
}