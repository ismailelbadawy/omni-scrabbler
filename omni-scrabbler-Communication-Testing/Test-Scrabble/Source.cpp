#include "easywsclient.hpp"
#include "Structs.cpp"
//#include "easywsclient.cpp" // <-- include only if you don't want compile separately
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include <assert.h>
#include<vector>
#include <stdio.h>
#include <string>
#include <algorithm>    // std::copy
#include<map>
#include <iostream>
using easywsclient::WebSocket;
static WebSocket::pointer ws = NULL;

enum MessageTypes {
	NAME = 0,
	START = 1,
	PASS = 2,
	EXCHANGE = 3,
	PLAY = 4,
	NO_CHALLENGE = 5,
	CHALLENGE = 6,
	CHALLENGE_ACCEPTED = 7,
	CHALLENGE_REJECTED = 8,
	INVALID = 9,
	END = 10,
};

void RecieveFromServer(const std::vector<uint8_t>& message);
GameState BufferToGameState(const std::vector<uint8_t>& message);
std::vector<uint8_t> PlayToBuffer(Play Move);
TimesOnly Time(const std::vector<uint8_t>& message);
ChallengeReject Reject_Case(const std::vector<uint8_t>& message);
PlayState Play_Agent(const std::vector<uint8_t>& message);
EndState EndGame(const std::vector<uint8_t>& message);
CountTime CountWithTimes(const std::vector<uint8_t>& message);


std::vector<uint8_t> T;
std::string CurrentState = "INIT";


int main()
{

#ifdef _WIN32
	INT rc;
	WSADATA wsaData;

	rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (rc) {
		printf("WSAStartup Failed.\n");
		return 1;
	}
#endif
	ws = WebSocket::from_url("ws://localhost:8080");
	assert(ws);

	while (true) {

		ws->poll();

		ws->dispatchBinary(RecieveFromServer);

		//	ws->sendBinary(T);

	}
	delete ws; // alternatively, use unique_ptr<> if you have C++11
#ifdef _WIN32
	WSACleanup();
#endif
	return 0;
}

void StringToAscii(std::vector<uint8_t>& T, std::string name)
{
	for (int i = 0; i < name.length(); i++) {
		T.push_back(int(name[i]));
	}

}

void RecieveFromServer(const std::vector<uint8_t>& message)
{
	int MsgType = message[0];
	T.push_back(0);

	if (MsgType == MessageTypes::END)  //End State
	{
		EndState End = EndGame(message);
		CurrentState = "READY";
	}

	if (MsgType == MessageTypes::NAME && CurrentState == "INIT")
	{
		std::string name = "farah";
		StringToAscii(T, name);
		CurrentState = "READY";
		std::cout << " Current State = " + CurrentState << std::endl;
		ws->sendBinary(T);
	}

	else if (MsgType == MessageTypes::START && CurrentState == "READY")
	{
		int Order = message[1];


		GameState Game = BufferToGameState(message);

		if (Order == 1)
		{
			CurrentState = "THINKING";

		}
		else if (Order == 2)
		{
			CurrentState = "IDLE";
		}

		std::cout << " Current State = " + CurrentState << std::endl;
	//	std::cout << " order = " + Order<< std::endl;


	}
	
	else if (CurrentState == "AWAIT_EXCHANGE_RESPONSE")
	{
		if (MsgType == MessageTypes::INVALID)
		{
			TimesOnly Times = Time(message);
			CurrentState = "THINKING";
		}
		else if (MsgType == MessageTypes::EXCHANGE)
		{
			TilesStruct Tiles;

			for (int i = 0; i < 7; i++)
			{
				Tiles.Tiles[i] = message[1 + i];
			}

			CurrentState = "IDLE";
		}

	}
	else if (CurrentState == "AWAIT_PLAY_RESPONSE ")
	{
		if (MsgType == MessageTypes::INVALID)
		{

			TimesOnly Times = Time(message);
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
	std::cout << "SECOND Current State = " + CurrentState << std::endl;
		if (MsgType == MessageTypes::PASS)
		{

			TimesOnly Times = Time(message);
			CurrentState = "THINKING";
		}
		else if (MsgType == MessageTypes::EXCHANGE)
		{
			CountTime CountTimes = CountWithTimes(message);
			CurrentState = "THINKING";
		}
		else if (MsgType == MessageTypes::PLAY)
		{
			PlayState Play = Play_Agent(message);

			CurrentState = "AWAIT_AGENT_CHALLENGE";
		}
	}
	else if (CurrentState == "AWAIT_AGENT_CHALLENGE")
	{
		if (MsgType == MessageTypes::NO_CHALLENGE)
		{
			TimesOnly Times = Time(message);
			CurrentState = "THINKING";
		}

	}
	else if (CurrentState == "AWAIT_CHALLENGE_RESPONSE")
	{
		if (MsgType == MessageTypes::NO_CHALLENGE || MsgType == MessageTypes::CHALLENGE_ACCEPTED)
		{
			TimesOnly Times = Time(message);
			CurrentState = "THINKING";

		}
		else if (MsgType == MessageTypes::CHALLENGE_REJECTED)
		{
			CurrentState = "IDLE";
		}
	}
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
		for (int i = 0; i < 7; i++)
		{
			std::cin >> mover.Tiles[i];
		}

		buffer = PlayToBuffer(mover);
		ws->sendBinary(buffer);
		CurrentState = "AWAIT_PLAY_RESPONSE";
		std::cout << " Current State = " + CurrentState << std::endl;
	}

	}
}



////////////////////////////// SENDING TO AGENT COMMUNICATION FROM SERVER //////////////////////////

GameState BufferToGameState(const std::vector<uint8_t>& message)
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


TimesOnly Time(const std::vector<uint8_t>& message)
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

ChallengeReject Reject_Case(const std::vector<uint8_t>& message)
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

CountTime CountWithTimes(const std::vector<uint8_t>& message)
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

PlayState Play_Agent(const std::vector<uint8_t>& message)
{
	PlayState Player;
	Player.Column = message[1];
	Player.Row = message[2];
	Player.Direction = message[3];

	for (int i = 0; i < 7; i++)
	{
		Player.Tiles[i] = message[4 + i];

	}

	for (int j = 0; j < 4; j++)
		Player.Score = (Player.Score << 8) + message[11 + j];

	for (int j = 0; j < 4; j++)
		Player.Challenge_Time = (Player.Challenge_Time << 8) + message[15 + j];

	for (int j = 0; j < 4; j++)
		Player.Player_Time = (Player.Player_Time << 8) + message[19 + j];

	for (int j = 0; j < 4; j++)
		Player.Total_Time = (Player.Total_Time << 8) + message[23 + j];


	return Player;
}

EndState EndGame(const std::vector<uint8_t>& message)
{
	EndState Player;
	Player.Reason = message[1];

	Player.Score = 0;
	Player.Opponent_Score = 0;

	for (int j = 0; j < 4; j++)
		Player.Score = (Player.Score << 8) + message[2 + j];

	for (int j = 0; j < 4; j++)
		Player.Opponent_Score = (Player.Opponent_Score << 8) + message[6 + j];

	return Player;
}


////////////////////////////// SENDING TO SERVER COMMUNICATION FROM AGENT //////////////////////////

std::vector<uint8_t> PlayToBuffer(Play Move)
{
	std::vector<uint8_t> buffer;
	buffer.push_back(4);
	buffer.push_back(Move.Column);
	buffer.push_back(Move.Row);
	buffer.push_back(Move.Direction);
	for (int i = 0; i < 7; i++)
		buffer.push_back(Move.Tiles[i]);
	//check that this function works
	for (int j = 0; j < 4; j++)
		buffer[13 - j] = Move.Score >> (j * 8);

	return buffer;
}

std::vector<uint8_t> ExchangeToBuffer(TilesStruct Tiles)
{
	std::vector<uint8_t> buffer;
	buffer.push_back(3);
	for (int i = 0; i < 7; i++)
		buffer.push_back(Tiles.Tiles[i]);

	return buffer;

}

//Pass message only send the message type

std::vector<uint8_t> Agent_No_Challenge()  // Agent Doesnt want to challenge
{
	std::vector<uint8_t> buffer;
	buffer.push_back(5);
	return buffer;

}

std::vector<uint8_t> Agent_Challenge() // Agent wants to challenge
{
	std::vector<uint8_t> buffer;
	buffer.push_back(6);
	return buffer;

}