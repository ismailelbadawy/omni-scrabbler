#include "Comm.h"
//#include"easywsclient.hpp"
#include <assert.h>
#include<vector>
#include <stdio.h>
#include <string>
#include <algorithm>    // std::copy
#include<map>
#include <iostream>
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
using easywsclient::WebSocket;

using namespace std;
TilesStruct Comm::exchanged;
bool Comm::StartAction;
bool Comm::ActionFinished;
ServerPlay Comm::play;
Comm::States Comm::mystate;
ChallengeReject Comm::rejected;
GameState Comm::game;
TimesOnly Comm::timesonly;
CountTime Comm::counter;
OpponentPlayState Comm::playstate;
EndState Comm::endstate;
string Comm::CurrentState;
string Comm::agentmove;
std::vector<uint8_t> Comm::T;
WebSocket::pointer Comm::ws;
std::recursive_mutex Comm::g_lock;
bool Comm::simulationfinished;
bool Comm::rackreceived;
bool Comm::playconfirmed;
bool Comm::oppplayed;
bool Comm::exchconfirmed;
Comm::Comm(){
	 //start=false;
	// ChangeState=false;
	for (int i=0;i<7;i++){
		play.Tiles[i]=0;
		exchanged.Tiles[i]=0;
	}
     mystate=IDLE;
	 simulationfinished=false;
	 playconfirmed=false;
	 oppplayed=false;
	 exchconfirmed=false;
	 Comm::ws=NULL;
     CurrentState = "INIT"; 
     #ifdef _WIN32
	 INT rc;
	 WSADATA wsaData;
     rackreceived=false;
	rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (rc) {
		printf("WSAStartup Failed.\n");
	}
    #endif
	Comm::ws = WebSocket::from_url("ws://localhost:8080");
	assert(Comm::ws);   
}

void Comm::ConnectServer(){
	
	while (Comm::ws->getReadyState() != WebSocket::CLOSED) {

		Comm::ws->poll();

		Comm::ws->dispatchBinary(&Comm::RecieveFromServer);
	}
}


int* Converts(uint8_t tiles[7]){
int *newtiles=new int[7];
for (int i=0;i<7;i++){
 newtiles[i]=(int)tiles[i];
 std::cout<<"old tile: "<<tiles[i]<<" new tile: "<<newtiles[i]<<std::endl;
}
return newtiles;
}

void Comm::SetPlay(ServerPlay agentplay){
     play.Column=agentplay.Column;
	 play.Row=agentplay.Row;
	 play.Direction=agentplay.Direction;
	 //ServerPlay.Tiles=agentplay.Tiles;
	 for (int i=0;i<7;i++){
		 play.Tiles[i]=agentplay.Tiles[i];
	 }
	 play.Score=agentplay.Score;
}
void Comm::SetMove(string move){
       agentmove=move;
}
void Comm::SetExchanged(TilesStruct tiles){
     // exchanged=tiles;
	 for (int i=0;i<7;i++){
		 exchanged.Tiles[i]=tiles.Tiles[i];
	 }
}
ServerPlay Comm::GetPlay(){
	return play;
}
TilesStruct Comm::GetExchangedTiles(){  // tiles to be exchanged
	return exchanged;
}
ChallengeReject Comm::GetRejected(){
	return rejected;
}
GameState Comm::GetGame(){
	return game;
}
TimesOnly Comm::GetTimesOnly(){
	return timesonly;
}
CountTime Comm::GetCounter(){
	return counter;
}
OpponentPlayState Comm::GetPlayState(){
	return playstate;
}
EndState Comm::GetEndState(){
   return endstate;
}
string Comm::GetCurrentState(){
	return CurrentState;
}
string Comm::GetMove(){
	return agentmove;
}

std::vector<uint8_t> Comm::PlayToBuffer(ServerPlay Move)
{
	std::vector<uint8_t> buffer(0);
	buffer.push_back(static_cast<uint8_t>(4));
	buffer.push_back(Move.Column);
	buffer.push_back(Move.Row);
	buffer.push_back(Move.Direction - (int)'0');
	for (int i = 0; i < 7; i++)
		buffer.push_back(Move.Tiles[i]);

	for (int i = 0; i < 4; i++)
		buffer.push_back(static_cast<uint8_t>(0));
	//check that this function works
	for (int j = 0; j < 4; j++)
		buffer[14 - j] = Move.Score >> (j * 8);
	return buffer;
}
std::vector<uint8_t> Comm::ExchangeToBuffer(TilesStruct Tiles)
{
	std::vector<uint8_t> buffer(0);
	buffer.push_back(static_cast<uint8_t>(3));
	for (int i = 0; i < 7; i++)
		buffer.push_back(Tiles.Tiles[i]);


	return buffer;
}

std::vector<uint8_t> Comm::PassToBuffer()
{
	std::vector<uint8_t> buffer(0);
	buffer.push_back(static_cast<uint8_t>(2));

	return buffer;
}



void Comm::StringToAscii(std::vector<uint8_t>& T, std::string name)
{
	for (int i = 0; i < name.length(); i++) {
		T.push_back(int(name[i]));
	}

}

GameState Comm::BufferToGameState(const std::vector<uint8_t>& message)
{
	GameState Player;

	Player.Order = message[1];

	std::cout << "Player Tiles : ";
	for (int i = 0; i < 7; i++)
	{
		Player.Tiles[i] = message[2 + i];
		std::cout << Player.Tiles[i] << " ";

	}

	std::cout << std::endl;

	int count = 9;

	for (int row = 0; row < 15; row++)
	{
		for (int column = 0; column < 15; column++)
		{
			Player.Board[row][column] = message[count];
			count++;
		}

	}

	//check that this function works properly
	for (int j = 0; j < 4; j++)
		Player.Score = (Player.Score << 8) + message[234 + j];
	std::cout << " Your Score " << Player.Score;

	for (int j = 0; j < 4; j++)
		Player.Opponent_Score = (Player.Opponent_Score << 8) + message[238 + j];

	std::cout << " Your Opponent_Score " << Player.Opponent_Score;

	for (int j = 0; j < 4; j++)
		Player.Player_Time = (Player.Player_Time << 8) + message[242 + j];

	std::cout << " Your Player_Time " << Player.Player_Time;
	for (int j = 0; j < 4; j++)
		Player.Total_Time = (Player.Total_Time << 8) + message[246 + j];

	std::cout << " Your Total_Time " << Player.Total_Time;
	return Player;
}


TimesOnly Comm::Time(const std::vector<uint8_t>& message)
{
	TimesOnly Player;
	Player.Player_Time = 0;
	Player.Total_Time = 0;

	/// Change From int32 BE to int
	for (int j = 0; j < 4; j++)
		Player.Player_Time = (Player.Player_Time << 8) + message[1 + j];

	for (int j = 0; j < 4; j++)
		Player.Total_Time = (Player.Total_Time << 8) + message[5 + j];

	return Player;
}

ChallengeReject Comm::Reject_Case(const std::vector<uint8_t>& message)
{
	ChallengeReject Player;

	for (int i = 0; i < 7; i++)
	{
		Player.Tiles[i] = message[1 + i];

	}

	for (int j = 0; j < 4; j++)
		Player.Player_Time = (Player.Player_Time << 8) + message[9 + j];

	for (int j = 0; j < 4; j++)
		Player.Total_Time = (Player.Total_Time << 8) + message[13 + j];

	return Player;

}

CountTime Comm::CountWithTimes(const std::vector<uint8_t>& message)
{
	CountTime Player;
	Player.Count = message[1];

	Player.Player_Time = 0;
	Player.Total_Time = 0;

	for (int j = 0; j < 4; j++)
		Player.Player_Time = (Player.Player_Time << 8) + message[2 + j];

	for (int j = 0; j < 4; j++)
		Player.Total_Time = (Player.Total_Time << 8) + message[6 + j];

	return Player;


}

OpponentPlayState Comm::Play_Agent(const std::vector<uint8_t>& message)
{
	OpponentPlayState Player;
	Player.Column = message[1];
	Player.Row = message[2];
	Player.Direction = message[3];

	for (int i = 0; i < 7; i++)
	{
		Player.Tiles[i] = message[4 + i];

	}

	for (int j = 0; j < 4; j++)
		Player.Score  = (Player.Score << 8) + message[11 + j];

	for (int j = 0; j < 4; j++)
		Player.Challenge_Time = (Player.Challenge_Time << 8) + message[15 + j];

	for (int j = 0; j < 4; j++)
		Player.Player_Time = (Player.Player_Time << 8) + message[19 + j];

	for (int j = 0; j < 4; j++)
		Player.Total_Time = (Player.Total_Time << 8) + message[23 + j];

     Player.MoveType=0;
	return Player;
}

EndState Comm::EndGame(const std::vector<uint8_t>& message)
{
	EndState Player;
	Player.Reason = message[1];
	if (Player.Reason == 0)
	{
		std::cout << "All tiles are used";
	}
	else if (Player.Reason == 1)
	{
		std::cout << "Time ended";
	}
	else if (Player.Reason == 2)
	{
		std::cout << "Conenction Error";
	}
	else if (Player.Reason == 3)
	{
		std::cout << "Stop button pressed";
	}

	// Player.Score = 0;
	// Player.Opponent_Score = 0;

	for (int j = 0; j < 4; j++)
		Player.Score = (Player.Score << 8) + message[2 + j];

	for (int j = 0; j < 4; j++)
		Player.Opponent_Score = (Player.Opponent_Score << 8) + message[6 + j];

	return Player;
}




void Comm::RecieveFromServer(const std::vector<uint8_t>& message){
   std::cout << CurrentState << std::endl;
	int MsgType = message[0];
	T.push_back(0);

	if (MsgType == MessageTypes::END)  //End State
	{
		mystate=ENDCONNECTION;
		EndState End = EndGame(message);
		CurrentState = "READY";
       
	}
	if (MsgType == MessageTypes::NAME && CurrentState == "INIT")
	{
		std::string name = "Red Army";
		StringToAscii(T, name);
		CurrentState = "READY";
		std::cout << " Current State = " + CurrentState << std::endl;
		ws->sendBinary(T);
	}

	else if (MsgType == MessageTypes::START && CurrentState == "READY" )
	{
		//start=true;
		mystate=RECEIVE_RACK;
		int Order = message[1];
		game = BufferToGameState(message);
		// for (int i=0;i<15;i++){
		// 	for (int j=0;j<15;j++){
		// 		std::cout<<game.Board[i][j]<<"  "<<std::endl;
		// 	}
		// 	std::cout<<std::endl;
		// }
        rackreceived=true;
		if (Order == 1)
		{   
			mystate=THINKING;
			CurrentState = "THINKING";

		}
		else if (Order == 2)
		{
			CurrentState = "IDLE";
		}

		std::cout << " Current State = " + CurrentState << std::endl;

	}

	else if (CurrentState == "AWAIT_EXCHANGE_RESPONSE" )
	{
		if (MsgType == MessageTypes::INVALID)
		{
			std::cout << "INVALID Exchange" << std::endl;
			TimesOnly Times = Time(message);

			game.Player_Time = Times.Player_Time;
			game.Total_Time = Times.Total_Time;
            mystate=THINKING;
			CurrentState = "THINKING";
		}
		else if (MsgType == MessageTypes::EXCHANGE)
		{
            mystate=EXCHANGE_RESPONSE;
			TilesStruct Tiles;

			std::cout << "EXCHANGED TILES:" << std::endl;
			for (int i = 0; i < 7; i++)
			{

				Tiles.Tiles[i] = message[1 + i];
				game.ExchangedTiles[i] = Tiles.Tiles[i];
   
				std::cout << int(game.ExchangedTiles[i]) << " "<<std::endl;
			}
			 int*t=Converts(exchanged.Tiles);
			 CompareTilesToRack(t,game.ExchangedTiles);
			 exchconfirmed=true;
    
			CurrentState = "IDLE";
		}

	}
	else if (CurrentState == "AWAIT_PLAY_RESPONSE")
	{

		if (MsgType == MessageTypes::INVALID)
		{
			std::cout<<"invalid move"<<std::endl;
			TimesOnly Times = Time(message);

			game.Player_Time = Times.Player_Time;
			game.Total_Time = Times.Total_Time;
            mystate=THINKING;
			CurrentState = "THINKING";
		}
		else if (MsgType == MessageTypes::CHALLENGE_REJECTED)
		{

			ChallengeReject Rejected;
			Rejected = Reject_Case(message);
			mystate=THINKING;
			CurrentState = "THINKING";
		}
		else if (MsgType == MessageTypes::NO_CHALLENGE)
		{
			mystate=PLAY_RESPONSE;
			TilesStruct Tiles;
            game.Score=game.Score+play.Score;
			for (int i = 0; i < 7; i++)
			{
				Tiles.Tiles[i] = message[1 + i];
				game.NewTilesAfterPlay[i] = Tiles.Tiles[i];
				std::cout << (int) message[1+i]<<" "<<std::endl;
				
			}
			int*t=Converts(play.Tiles);
			CompareTilesToRack(t,game.NewTilesAfterPlay);
			playconfirmed=true;
			CurrentState = "IDLE";
		}
		else if (MsgType == MessageTypes::CHALLENGE_ACCEPTED)
		{
			CurrentState = "IDLE";
		}
	}
	else if (CurrentState == "IDLE")
	{
		if (MsgType == MessageTypes::PASS )
		{
            mystate=PASS_OPPONENT;
			TimesOnly Times = Time(message);
			game.Player_Time = Times.Player_Time;
			game.Total_Time = Times.Total_Time;
            game.OpponentMove.MoveType=1;
			oppplayed=true;
			mystate=THINKING;
			CurrentState = "THINKING";
		}
		else if (MsgType == MessageTypes::EXCHANGE)
		{
			mystate=EXCHANGE_OPPONENT;
			CountTime CountTimes = CountWithTimes(message);
			game.OpponentExchangeCount = CountTimes.Count;
			game.Player_Time = CountTimes.Player_Time;
			game.Total_Time = CountTimes.Total_Time;
            game.OpponentMove.MoveType=1;

			std::cout << int(CountTimes.Count) << std::endl;
            oppplayed=true;
			mystate=THINKING;
			CurrentState = "THINKING";
		}
		else if (MsgType == MessageTypes::PLAY)
		{
			mystate=PLAY_OPPONENT;
			game.OpponentMove = Play_Agent(message);
            oppplayed=true;
			CurrentState = "AWAIT_AGENT_CHALLENGE";
		}
	}
	else if (CurrentState == "AWAIT_AGENT_CHALLENGE")
	{
		if (MsgType == MessageTypes::NO_CHALLENGE)
		{
			TimesOnly Times = Time(message);

			game.Player_Time = Times.Player_Time;
			game.Total_Time = Times.Total_Time;
            mystate=THINKING;
			CurrentState = "THINKING";
		}

	}
	else if (CurrentState == "AWAIT_CHALLENGE_RESPONSE")
	{
		if (MsgType == MessageTypes::NO_CHALLENGE || MsgType == MessageTypes::CHALLENGE_ACCEPTED)
		{
			TimesOnly Times = Time(message);

			game.Player_Time = Times.Player_Time;
			game.Total_Time = Times.Total_Time;
            mystate=THINKING;
			CurrentState = "THINKING";
			

		}
		else if (MsgType == MessageTypes::CHALLENGE_REJECTED)
		{
			CurrentState = "IDLE";
		}
	}
}

void Comm::ThinkingThread(){
	while (true)
	{
		if (CurrentState == "THINKING"){
		if (simulationfinished){
			// to ensure that the  server doesn't receive the old data as it is in a separate thread.
			std::vector<uint8_t> buffer;
			simulationfinished=false;
			if (agentmove == "PLAY")
			{		
                std::cout <<"this is your play "<<std::endl;
				for (int i=0;i<7;i++){
					std::cout<<(int)play.Tiles[i]<<std::endl;
				}
				buffer = PlayToBuffer(play);
				ws->sendBinary(buffer);
				CurrentState = "AWAIT_PLAY_RESPONSE";
				std::cout << " Current State = " + CurrentState << std::endl;
			}
			else if (agentmove == "EXCHANGE")
			{
			   buffer = ExchangeToBuffer(exchanged);
				ws->sendBinary(buffer);
				CurrentState = "AWAIT_EXCHANGE_RESPONSE";
				std::cout << " Current State = " + CurrentState << std::endl;
			}
			else if (agentmove == "PASS")
			{
				std::vector<uint8_t> buffer(0);
				buffer = PassToBuffer();
				ws->sendBinary(buffer);
				CurrentState = "IDLE";
				std::cout << " Current State = " + CurrentState << std::endl;

			}
		}
		}
	}
}
void Comm::CompareTilesToRack(int oldtiles[7],int newtiles[7]){
vector<int> tempnewtiles;
	for (int i=0;i<7;i++){
        if ((int)newtiles[i]!=0){
			tempnewtiles.push_back((int)newtiles[i]);
			std::cout <<newtiles[i]<<std::endl;
			}
	}
vector<int> playedtiles;
	for (int j=0;j<7;j++){
        if ((int)oldtiles[j]!=0){
			playedtiles.push_back((int)oldtiles[j]);
			std::cout <<oldtiles[j]<<std::endl;
			}
	}
	std::cout<<"your calculated rack is:"<<std::endl;
for (int k=0;k<playedtiles.size();k++){
	for (int l=0;l<7;l++)
	{
		if (playedtiles[k]==game.Tiles[l])
		{
			std::cout<<"your condition is true: "<<std::endl;
			game.Tiles[l]=newtiles[k];
			std::cout<<game.Tiles[l]<<std::endl;
			break;
		}
	}
}	
std::cout<<"your new rack is"<<std::endl;
     for (int i=0;i<7;i++){
		 cout<<game.Tiles[i]<<std::endl;
	 }

}

Comm::~Comm(){
    delete ws; // alternatively, use unique_ptr<> if you have C++11
#ifdef _WIN32
	WSACleanup();
#endif
}




