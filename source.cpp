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

void print(){
	Tile * tiles[15][15];
	board.GetTiles(tiles);
	cout << endl <<"board" << endl;
	for (int i=0; i< 15; i++){
		for (int k=0; k < 15; k++){
			cout << tiles[i][k]->GetLetter() << " ";

		}
		cout << endl;
	}
}


int main(){
	Bag bag(BAG_PATH);


	//board.Probe('c',7,2,3);		//Inserting letters 'e' and 't' to the empty board
	//board.Probe('a',7,3,1);
	//board.Probe('t',7,4,1);
	

	Tile RackTiles[7];          //A simple array to carry the rack's letters

	RackTiles[0].SetLetter('r');
	RackTiles[0].SetScore(1);
	RackTiles[1].SetLetter('a');
	RackTiles[1].SetScore(4);
	RackTiles[2].SetLetter('v');
	RackTiles[2].SetScore(3);
	RackTiles[3].SetLetter('e');
	RackTiles[3].SetScore(2);
	RackTiles[4].SetLetter('p');
	RackTiles[4].SetScore(1);
	RackTiles[5].SetLetter('a');
	RackTiles[5].SetScore(4);
	RackTiles[6].SetLetter('m');
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
	bool Human = 1; //should be received from GUI
	if (Human == 0){//Agent Mode
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
		bool MyTurn = true;
		//cin >> MyTurn;
		HumanMode Human(&board, &bag);
		//Human Mode has pointers to the board and bag to detect any changes

		Rack MyRack;
		Rack OpponentRack;

		Move chosenMove;

		moves.clear();

		//bool GameOver = false;

		Human.SetMyRack(MyRack);
		Human.SetOpponentRack(OpponentRack);

		bool MyMoves= true;
		bool OppMoves = true;

		Tile* boardTiles [15][15];
		board.GetTiles(boardTiles);

		int counter = 0;

		//GameOver is (MyPass == true && OppPass== true) || (board.GetCount() == 100)
		while (!Human.CheckGameOver(MyMoves, OppMoves)){ //while !GameOve
		

		if (counter < 6){
			MyTurn = true;
			counter++;
		}
		else{
			print();
		}

		vector <Tile> tt;
		
		if (counter >= 6){

	
		cout << "My Rack ";
		tt = MyRack.GetRackTiles();
		for (int i=0; i< tt.size(); i++){
			cout << tt[i].GetLetter() << " ";
		} 
		cout << endl << "opponent Rack ";
		tt = OpponentRack.GetRackTiles();
		for (int i=0; i< tt.size(); i++){
			cout << tt[i].GetLetter() << " ";
		} 
		}
			if (MyTurn){
				cout << endl << "My Turn " << endl;
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
					AgentMove SentMove;
					SentMove.dir= -1;
					SentMove.row = -1;
					SentMove.col= -1;
					SentMove.score = -1;
					SentMove.dir= -1;
					SentMove.tiles = "";
					MyMoves = false;
					//Send to GUI SentMove which is pass
					cout << "PASS" << endl;
					MyTurn = false;
					continue;
				}
				//by here, there are moves
				Move PassMove = Human.GetPassMove();

				if (chosenMove.GetScore() < PassMove.GetScore()){ //pass was better than best move, but there are moves
					AgentMove SentMove;
					SentMove.dir= -1;
					SentMove.row = -1;
					SentMove.col= -1;
					SentMove.score = -1;
					SentMove.dir= -1;
					SentMove.tiles = "";
					MyMoves = true;
					//Send to GUI SentMove which is pass
					cout << "PASS";
					MyTurn = false;
					continue;
				}
				MyMoves= true;
				OppMoves = true;

				AgentMove agentMove = Human.MoveToGui(chosenMove);
				//send agent move to GUI
				cout << "Chosen Move ";
				vector <Tile> tt2= chosenMove.GetPlay()->GetTiles();
				for (int i=0; i < tt2.size(); i++){
					cout << tt2[i].GetLetter();
				}

				//add word to board, remove letter from rack
				Human.UpdateBoardAndRack(*chosenMove.GetPlay(),  MyRack);

				Human.SetMyRack(MyRack);
				moves.clear();
				MyTurn = false;
			}
			else { //his turn
			cout << endl << "His Turn " << endl;
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
				int response;
				cout << "enter response ";
				cin >> response;
				
				if (response == 0){ //actual move
					//receive from GUI vector of WordGUI that has row, col, letter or each new letter on board
					vector<WordGUI> wordVector; //= haga men GUI
					cout << "enter tiles in format row col letter " << endl;
					cout << "# to quit, any key to continue ";
					char n;
					cin >> n; 
					while (n != '#'){
						WordGUI newWord;
					
					cin >> newWord.row;
					cin >> newWord.col;
					cin >> newWord.letter;
					wordVector.push_back(newWord);
					cin >> n;
					}
					

					//newWord.row = 7;
					//newWord.col = 11;
					//newWord.letter = 'a';
					//wordVector.push_back(newWord);
					
					Rack NewOpponent = OpponentRack;
					Play ActualPlay = Human.GetOpponentPlay(wordVector, NewOpponent, boardTiles);
					if (ActualPlay.GetRow() == -1 && ActualPlay.GetColumn() == -1){
						FbMessage = "NO";
						//send to GUI that word is not vertical or horizontal, or there are gaps between new tiles
						//Still Opponent turn..
						cout << "INVALID " << endl;
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
							cout << "INVALID " << endl;
							continue;
						}
					}
					
					if (!movGen.IsValidMove(ActualPlay, EnemyRack)){
						//send to GUI invalid move
						FbMessage = "NO";
						//still opponent turn..
						cout << "INVALID " << endl;
						continue;
					}
					else{
						FbMessage ="YES";
						//send to GUI valid
						cout << "VALID " << endl;
					}
				
					//Move is valid-> then calculate its score, take tiles from rack, put them on board
					int OpponentScore = ActualPlay.GetScore();
					OpponentRack = NewOpponent;
					Human.AddPlayToBoard(ActualPlay, boardTiles);

					if (chosenMoveScore > OpponentScore){
						//send opponent a feedback through the GUI: you didnt choose best move, best move was
						//send best move
						AgentMove agentMove = Human.MoveToGui(chosenMove);
						cout << "Not best move " << endl;
						cout << "Best Move ";
				tt= chosenMove.GetPlay()->GetTiles();
				for (int i=0; i < tt.size(); i++){
					cout << tt[i].GetLetter();
				}

				cout << "at pos ";
				int ii=  chosenMove.GetPlay()->GetRow();
				int jj=  chosenMove.GetPlay()->GetColumn();
				cout << ii << " " << jj;

					}
					else if (chosenMoveScore == OpponentScore){
						//send opponent a feedback through the GUI: you chose the best move
						cout << "You got the best move " << endl;
						FbMessage = "Congratulations";
					}
					else if (chosenMoveScore < OpponentScore){
						//send opponent a feedback through the GUI: you chose a better move than the evaluated
						cout << "You got better move ";
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
					cout << "Best Move ";
				tt= chosenMove.GetPlay()->GetTiles();
					for (int i=0; i < tt.size(); i++){
						cout << tt[i].GetLetter();
					}
					cout << "waiting for player..";
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
