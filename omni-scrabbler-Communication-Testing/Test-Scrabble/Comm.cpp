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
TilesStruct Comm::rack;
ChallengeReject Comm::rejected;
GameState Comm::game;
TimesOnly Comm::timesonly;
CountTime Comm::counter;
OpponentPlayState Comm::playstate;
EndState Comm::endstate;
string Comm::CurrentState;
std::vector<uint8_t> Comm::T;
WebSocket::pointer Comm::ws;

Comm::Comm(){
	Comm::ws=NULL;
     CurrentState = "INIT"; 
     #ifdef _WIN32
	 INT rc;
	 WSADATA wsaData;

	rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (rc) {
		printf("WSAStartup Failed.\n");
	}
#endif
	Comm::ws = WebSocket::from_url("ws://localhost:8080");
	assert(Comm::ws);   
}

TilesStruct Comm::GetRack(){
	return rack;
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


std::vector<uint8_t> Comm::PlayToBuffer(Play Move)
{
	std::vector<uint8_t> buffer(0);
	buffer.push_back(static_cast<uint8_t>(4));
	buffer.push_back(Move.Column - (int)'0');
	buffer.push_back(Move.Row - (int)'0');
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

	Player.Score = 0;
	Player.Opponent_Score = 0;

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

	else if (MsgType == MessageTypes::START && CurrentState == "READY")
	{
		int Order = message[1];
		game = BufferToGameState(message);

		if (Order == 1)
		{
			CurrentState = "THINKING";

		}
		else if (Order == 2)
		{
			CurrentState = "IDLE";
		}

		std::cout << " Current State = " + CurrentState << std::endl;

	}

	else if (CurrentState == "AWAIT_EXCHANGE_RESPONSE")
	{
		if (MsgType == MessageTypes::INVALID)
		{
			std::cout << "INVALID Exchange" << std::endl;
			TimesOnly Times = Time(message);

			game.Player_Time = Times.Player_Time;
			game.Total_Time = Times.Total_Time;

			CurrentState = "THINKING";
		}
		else if (MsgType == MessageTypes::EXCHANGE)
		{
			TilesStruct Tiles;

			std::cout << "EXCHANGED TILES:" << std::endl;
			for (int i = 0; i < 7; i++)
			{

				Tiles.Tiles[i] = message[1 + i];
				game.ExchangedTiles[i] = Tiles.Tiles[i];

				std::cout << int(game.ExchangedTiles[i]) << " ";
			}

			
			CurrentState = "IDLE";
		}

	}
	else if (CurrentState == "AWAIT_PLAY_RESPONSE")
	{

		if (MsgType == MessageTypes::INVALID)
		{
			TimesOnly Times = Time(message);

			game.Player_Time = Times.Player_Time;
			game.Total_Time = Times.Total_Time;

			CurrentState = "THINKING";
		}
		else if (MsgType == MessageTypes::CHALLENGE_REJECTED)
		{

			ChallengeReject Rejected;
			Rejected = Reject_Case(message);
			CurrentState = "THINKING";
		}
		else if (MsgType == MessageTypes::NO_CHALLENGE)
		{
			TilesStruct Tiles;

			for (int i = 0; i < 7; i++)
			{
				Tiles.Tiles[i] = message[1 + i];
				game.NewTilesAfterPlay[i] = Tiles.Tiles[i];
				std::cout << (int) message[1+i];
			}

			CurrentState = "IDLE";
		}
		else if (MsgType == MessageTypes::CHALLENGE_ACCEPTED)
		{
			CurrentState = "IDLE";
		}
	}
	else if (CurrentState == "IDLE")
	{
		if (MsgType == MessageTypes::PASS)
		{

			TimesOnly Times = Time(message);

			game.Player_Time = Times.Player_Time;
			game.Total_Time - Times.Total_Time;

			CurrentState = "THINKING";
		}
		else if (MsgType == MessageTypes::EXCHANGE)
		{
			CountTime CountTimes = CountWithTimes(message);
			game.OpponentExchangeCount = CountTimes.Count;
			game.Player_Time = CountTimes.Player_Time;
			game.Total_Time - CountTimes.Total_Time;


			std::cout << int(CountTimes.Count) << std::endl;

			CurrentState = "THINKING";
		}
		else if (MsgType == MessageTypes::PLAY)
		{
			game.OpponentPlayedTiles = Play_Agent(message);

			CurrentState = "AWAIT_AGENT_CHALLENGE";
		}
	}
	else if (CurrentState == "AWAIT_AGENT_CHALLENGE")
	{
		if (MsgType == MessageTypes::NO_CHALLENGE)
		{
			TimesOnly Times = Time(message);

			game.Player_Time = Times.Player_Time;
			game.Total_Time - Times.Total_Time;

			CurrentState = "THINKING";
		}

	}
	else if (CurrentState == "AWAIT_CHALLENGE_RESPONSE")
	{
		if (MsgType == MessageTypes::NO_CHALLENGE || MsgType == MessageTypes::CHALLENGE_ACCEPTED)
		{
			TimesOnly Times = Time(message);

			game.Player_Time = Times.Player_Time;
			game.Total_Time - Times.Total_Time;

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
		if (CurrentState == "THINKING")
		{
			std::vector<uint8_t> buffer;
			std::string move;
			std::cout << "what will be your next move?(PLAY/PASS/EXCHANGE)" << std::endl;
			std::cin >> move;

			if (move == "PLAY")
			{

				Play mover;
				std::cout << "enter column" << std::endl;
				std::cin >> mover.Column;
				std::cout << "enter row" << std::endl;
				std::cin >> mover.Row;
				std::cout << "enter direction" << std::endl;
				std::cin >> mover.Direction;
				std::cout << "enter titles" << std::endl;
				int count = 0;
				int x;
				while (count != 7)
				{
					std::cout << " Enter  " << count << ":";
					std::cin >> x;
					mover.Tiles[count] = static_cast<uint8_t>(x);
					count++;
				}
				std::cin >> mover.Score;

				buffer = PlayToBuffer(mover);
				ws->sendBinary(buffer);
				CurrentState = "AWAIT_PLAY_RESPONSE";
				std::cout << " Current State = " + CurrentState << std::endl;
			}
			else if (move == "EXCHANGE")
			{

				TilesStruct TilesExc;
				int count = 0;
				int x;
				while (count != 7)
				{
					std::cout << " Enter  " << count << ":";
					std::cin >> x;
					TilesExc.Tiles[count] = static_cast<uint8_t>(x);
					count++;
				}


				buffer = ExchangeToBuffer(TilesExc);
				ws->sendBinary(buffer);
				CurrentState = "AWAIT_EXCHANGE_RESPONSE";
				std::cout << " Current State = " + CurrentState << std::endl;
			}
			else if (move == "PASS")
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
Comm::~Comm(){
    delete ws; // alternatively, use unique_ptr<> if you have C++11
#ifdef _WIN32
	WSACleanup();
#endif
}




