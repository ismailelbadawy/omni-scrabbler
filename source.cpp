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
#include"Models/HumanMode.h"
#include"GADDAG/GADDAG.h"
#include "Evaluators/MidgameEvaluator.h"

#include "Integration/GameManager.h"

#include "Integration/Structs.cpp"


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


	GameManager gameManager;
	//gameManager.InitGame();
	//string Mode = gameManager.GetMode();

	string Mode = "AIMODE";

	cout << "Mode = " << Mode << endl;

	MoveGenerator movGen(board);

	map<string, double>* syn2 = new map<string, double>();
	map<char, double>* worth = new map<char, double>();
	SuperLeaveLoader loader(syn2, worth, "assets/syn2", "assets/worths");
	cout << "Loaded the rack leave map with count " << syn2->size() << endl;


	if (Mode == "AIMODE"){         //AI Mode
	 	vector<Move> moves;
		Rack rack;
		Agent AI_Agent(&board, &bag, &rack);

		Rack rack2; //for testing the other agent
		Agent AI_Agent2(&board, &bag, &rack2); //for testing the other agent

		Move chosenMove;
		bool GameOver= false; //received from server
		bool MyTurn= true; //received from server
		while(!GameOver){
			
			//get rack from server
			///////////////////////////////////////////////////////////// setting rack for testing only
			vector <Tile> bagRemLetters = bag.GetRemainingTiles();
    		vector <Tile> MyRack = rack.GetRackTiles();

    		int RackSize = MyRack.size();
    
    		for (int i=0; i< 7-RackSize; i++){
    		    if (bagRemLetters.size() > 0){
    		        srand((int)time(NULL));
    		        int randomIndex = rand() % bagRemLetters.size();
				    MyRack.push_back(bagRemLetters[randomIndex]);	
    		        bag.TakeLetter(bagRemLetters[randomIndex]);
				    bagRemLetters.erase(bagRemLetters.begin()+randomIndex);
    		    }
    		    else 
    		        break;

    		}
    		rack.SetRackTiles(MyRack);
			///////////////////////////////

			//get rack2 from server
			///////////////////////////////////////////////////////////// setting rack for testing only
			bagRemLetters = bag.GetRemainingTiles();
    		MyRack = rack2.GetRackTiles();

    		RackSize = MyRack.size();
    
    		for (int i=0; i< 7-RackSize; i++){
    		    if (bagRemLetters.size() > 0){
    		        srand((int)time(NULL));
    		        int randomIndex = rand() % bagRemLetters.size();
				    MyRack.push_back(bagRemLetters[randomIndex]);	
    		        bag.TakeLetter(bagRemLetters[randomIndex]);
				    bagRemLetters.erase(bagRemLetters.begin()+randomIndex);
    		    }
    		    else 
    		        break;

    		}
    		rack2.SetRackTiles(MyRack);
			///////////////////////////////


			if (MyTurn){
				int BagSize = (int)bag.GetRemainigLetters().size();
				moves = movGen.Generate(&rack, board, board.GetCount()==0);

				int numTilesByOpponent =3; //should be received

				if (moves.size() > 0){
					if (BagSize > 9){//MidGame
					chosenMove = AI_Agent.MidGame(moves, syn2, worth,&movGen, numTilesByOpponent); //should return best move
					
					}
					else if (BagSize > 0 && BagSize <=9){
						chosenMove = AI_Agent.PreEndGame(syn2,worth,&movGen,moves, numTilesByOpponent); //should return best move
					}
					else if (BagSize == 0){
						chosenMove = AI_Agent.EndGame(moves, syn2, worth, &movGen, rack, numTilesByOpponent);
					}
				}
				else {
					//Send Pass
					//MyTurn = false;
					continue;
				}
			
				Move PassMove = AI_Agent.GetPassMove();

				if (chosenMove.GetScore() < PassMove.GetScore()){
					//Send Pass
					//MyTurn = false;
					continue;
				}
				
				//Send chosen move to server
				//put on board for testing only and get new rack for testing
				////////////////
				Play* p = chosenMove.GetPlay();
				vector <Tile> tiles = p->GetTiles();
    			
   				for (int i=0; i< (int) tiles.size(); i++){
					int row, col; 
					tiles[i].GetIndex(row, col);
   
        			char letter= tiles[i].GetLetter();
        			int score= tiles[i].GetScore();
        

        			board.Probe(letter, row, col, score);

        			int Prevsize= rack.GetLength();
        			rack.RemoveTile(letter);
        			int Newsize= rack.GetLength();
        			if (Newsize == Prevsize){ //letter wasn't found, search for blank
        			rack.RemoveTile('?');
        			}
        			if (p->GetIsHorizontal()){
        			    col++;
        			}
        			else{
        			    row++;
        			}
    			}
				///////////////

				MyTurn = false; //received from server
				moves.clear();
			}else { //his turn
				int BagSize = (int)bag.GetRemainigLetters().size();
				moves = movGen.Generate(&rack2, board, board.GetCount()==0);

				int numTilesByOpponent =4; //should be received

				if (moves.size() > 0){
					if (BagSize > 9){//MidGame
					chosenMove = AI_Agent2.MidGame(moves, syn2, worth,&movGen, numTilesByOpponent); //should return best move
					
					}
					else if (BagSize > 0 && BagSize <=9){
						chosenMove = AI_Agent2.PreEndGame(syn2,worth,&movGen,moves, numTilesByOpponent); //should return best move
					}
					else if (BagSize == 0){
						chosenMove = AI_Agent2.EndGame(moves, syn2, worth, &movGen, rack2, numTilesByOpponent);
					}
				}
				else {
					//Send Pass
					//MyTurn = false;
					continue;
				}
			
				Move PassMove = AI_Agent2.GetPassMove();

				if (chosenMove.GetScore() < PassMove.GetScore()){
					//Send Pass
					//MyTurn = false;
					continue;
				}
				
				//send chosen move to server
				
								//put on board for testing only and get new rack for testing
				////////////////
				Play* p = chosenMove.GetPlay();
				vector <Tile> tiles = p->GetTiles();
    			
   				for (int i=0; i< (int) tiles.size(); i++){
					int row, col; 
					tiles[i].GetIndex(row, col);

        			char letter= tiles[i].GetLetter();
        			int score= tiles[i].GetScore();
        

        			board.Probe(letter, row, col, score);

        			int Prevsize= rack2.GetLength();
        			rack2.RemoveTile(letter);
        			int Newsize= rack2.GetLength();
        			if (Newsize == Prevsize){ //letter wasn't found, search for blank
        			rack2.RemoveTile('?');
        			}
        			if (p->GetIsHorizontal()){
        			    col++;
        			}
        			else{
        			    row++;
        			}
    			}
				///////////////

				MyTurn = true;
				moves.clear();
			}
			
		}	
	}
	else { //Human Mode
		gameManager.PlayHuman(&board, &bag, &movGen, syn2, worth);		
		}
		//send to GUI game is over
	
}