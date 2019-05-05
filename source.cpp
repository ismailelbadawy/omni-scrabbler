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
	gameManager.InitGame();
	string Mode = gameManager.GetMode();

	cout << "Mode = " << Mode << endl;

	MoveGenerator movGen(board);

	map<string, double>* syn2 = new map<string, double>();
	map<char, double>* worth = new map<char, double>();
	SuperLeaveLoader loader(syn2, worth, "assets/syn2", "assets/worths");
	cout << "Loaded the rack leave map with count " << syn2->size() << endl;


	if (Mode != "HumanMode"){         //AI Mode
	 	vector<Move> moves;
		Rack rack;
		Agent AI_Agent(&board, &bag, &rack);
		Move pass = AI_Agent.GetPassMove();
		Move chosenMove;
		bool GameOver= false; //should be over when all 100 tiles are played or when we pass and opponent pass 
		while(!GameOver){
			bool MyTurn= true;
			if (MyTurn){
				auto  start = chrono::high_resolution_clock::now();
				moves = movGen.Generate(&rack, board, board.GetCount()==0);
				auto  end = chrono::high_resolution_clock::now();

				int numTilesByOpponent =3;

				int BagSize = (int)bag.GetRemainigLetters().size();
				if (BagSize > 9){//MidGame
					chosenMove = AI_Agent.MidGame(moves, syn2, worth,&movGen, numTilesByOpponent); //should return best move
					
				}
				else if (BagSize > 0 && BagSize <=9){
					chosenMove = AI_Agent.PreEndGame(syn2,worth,&movGen,moves, numTilesByOpponent); //should return best move
	
				}
				else if (BagSize == 0){
					//AI_Agent.EndGame(moves); //should return best move
				}
				moves.clear();
			}
			
		}	
	}
	else { //Human Mode
		gameManager.PlayHuman(&board, &bag, &movGen, syn2, worth);
		vector<Move> moves;	
		bool MyTurn = false;
		HumanMode Human(&board, &bag);
		//Human Mode has pointers to the board and bag to detect any changes

		Rack MyRack;
		Rack OpponentRack;

		Move chosenMove;

		moves.clear();
		
		Human.SetMyRack(MyRack);
		Human.SetOpponentRack(OpponentRack);

		bool MyMoves= true;
		bool OppMoves = true;

		Tile* boardTiles [15][15];
		board.GetTiles(boardTiles);

		//GameOver is (MyPass == true && OppPass== true) || (board.GetCount() == 100)
		while (!Human.CheckGameOver(MyMoves, OppMoves)){ //while !GameOver
			if (MyTurn){
				int BagSize = (int)bag.GetRemainigLetters().size();
				moves = movGen.Generate(&MyRack, board, board.GetCount()==0);

				if (moves.size() > 0){
					if (BagSize > 9){//MidGame
						chosenMove = Human.MidGame(moves, syn2, worth,&movGen); //should return best move
					
					}
					else if (BagSize > 0 && BagSize <=9){
						chosenMove = Human.PreEndGame(syn2,worth,&movGen,moves); //should return best move
					}
					else if (BagSize == 0){
						//Human.EndGame(moves); //should return best move
					}
				}
				else{
					AgentMove SentMove = Human.PassMoveToGui(); //set -1 paramters
					MyMoves = false;
					//Send to GUI SentMove which is pass
					MyTurn = false;
					continue;
				}
				//by here, there are moves
				Move PassMove = Human.GetPassMove();

				if (chosenMove.GetScore() < PassMove.GetScore()){ //pass was better than best move, but there are moves
					AgentMove SentMove= Human.PassMoveToGui(); //send O
					MyMoves = true;
					//Send to GUI SentMove which is pass
					MyTurn = false;
					continue;
				}
				MyMoves= true;
				OppMoves = true;

				AgentMove agentMove = Human.MoveToGui(chosenMove);
				//send agent move to GUI


				//add word to board, remove letter from rack
				Human.UpdateBoardAndRack(*chosenMove.GetPlay(),  MyRack);

				Human.SetMyRack(MyRack);
				moves.clear();
				MyTurn = false;
			}
			else { //his turn
				string FbMessage="";
				if (moves.empty()){ //not  calculated before
					moves = movGen.Generate(&OpponentRack, board, board.GetCount()==0);
					for (int i = 0; i < (int)moves.size(); i++)
					{
						moves[i].SetPenalty(0);
						moves[i].SetRackLeave(0);
						moves[i].CalculateScore();
					}
					
					std::sort(moves.begin(), moves.end());
				}
				
				int chosenMoveScore;
				if (moves.size() ==  0){ //no chosen word was found
					chosenMoveScore = 0;
					OppMoves = false;
				}
				else{
					chosenMove = moves[0];
					chosenMoveScore =  chosenMove.GetPlay()->GetScore();
					OppMoves = true;
				}
		
				//wait for opponent 0->actual move/1->pass/2->hint/3-exchange from GUI 
				int response = 3;
				
				if (response == 0){ //actual move
					//receive from GUI vector of WordGUI that has row, col, letter or each new letter on board
					vector<WordGUI> wordVector; //= haga men GUI
					
					Rack NewOpponent = OpponentRack;
					Play ActualPlay = Human.GetOpponentPlay(wordVector, NewOpponent, boardTiles);
					if (ActualPlay.GetRow() == -1 && ActualPlay.GetColumn() == -1){
						FbMessage = "NO";
						//send to GUI that word is not vertical or horizontal, or there are gaps between new tiles
						//Still Opponent turn..
						continue;
					}
				
					string EnemyRack = OpponentRack.RackToString();

					if (board.GetCount() == 0){//first move, word should touch pos 7,7
						bool found = false;
						vector <Tile> tilesVector= ActualPlay.GetTiles();
						for (int i=0; i< (int) tilesVector.size(); i++){
							int row, col;
							tilesVector[i].GetIndex(row, col);
							if (row == 7 && col ==7){
								found = true;
							}
						}
						if (!found){
							FbMessage = "NO";
							//send to GUI that move should include pos 7,7
							//still opponent trun..
							continue;
						}
					}
					
					if (!movGen.IsValidMove(ActualPlay, EnemyRack)){
						//send to GUI invalid move
						FbMessage = "NO";
						//still opponent turn..
						continue;
					}
					else{
						FbMessage ="YES";
						//send to GUI valid
					}
				
					//Move is valid-> then calculate its score, take tiles from rack, put them on board
					int OpponentScore = ActualPlay.GetScore();
					OpponentRack = NewOpponent;
					Human.AddPlayToBoard(ActualPlay, boardTiles);

					if (chosenMoveScore > OpponentScore){
						//send opponent a feedback through the GUI: you didnt choose best move, best move was
						//send best move
						AgentMove agentMove = Human.MoveToGui(chosenMove);
					}
					else if (chosenMoveScore == OpponentScore){
						//send opponent a feedback through the GUI: you chose the best move
						FbMessage = "Congratulations";
					}
					else if (chosenMoveScore < OpponentScore){
						//send opponent a feedback through the GUI: you chose a better move than the evaluated
						FbMessage = "Better";
					}

					MyMoves = true;
					OppMoves= true;
					Human.SetOpponentRack(OpponentRack);
					MyTurn= true;
					moves.clear();
				}
				else if (response == 1){//pass
					MyTurn = true;
					moves.clear();
					continue;
				}
				else if (response == 2){//hint
					//send best move to GUI and wait for opponent to play, if oppMoves = false, send no possible moves
					
					MyTurn = false;
					AgentMove agentMove = Human.MoveToGui(chosenMove);
					//no clear moves
					continue;
				}
				else if (response == 3) { // Exchange tiles
					vector<char> toBeExchanged = { 'p','i'};
					vector<int> toBeExchangedLocations;
					Rack OpRack2(OpponentRack);
					for (int i = 0; i < (int)toBeExchanged.size(); i++)
					{
						/* code */
						toBeExchangedLocations.push_back(OpRack2.GetPosition(toBeExchanged[i]));
					}
					bag.swapRack(OpponentRack,toBeExchangedLocations);
					string opponentRackToGui="";
					for (int i = 0; i < (int)OpponentRack.GetLength(); i++)
					{
						opponentRackToGui+=OpponentRack.GetRackTiles()[i].GetLetter();
					}
					
					moves.clear();
					MyTurn = true;
				}
			/// Put the rack in the appropriate container to be sent to the user.
				
			}
		}
		//send to GUI game is over
	}
}