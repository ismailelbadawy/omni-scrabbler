#pragma once
#include<iostream>
#include"easywsclient.hpp"
#include "Structs.cpp"
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
using namespace std;
using easywsclient::WebSocket;


class Comm{
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

 static TilesStruct rack;
 static ChallengeReject rejected;
 static GameState game;
 static TimesOnly timesonly;
 static CountTime counter;
 static OpponentPlayState playstate;
 static EndState endstate;
 static string CurrentState;
 static std::vector<uint8_t> T;
 static GameState BufferToGameState(const std::vector<uint8_t>& message);
 static std::vector<uint8_t> PlayToBuffer(Play Move);
 static std::vector<uint8_t> ExchangeToBuffer(TilesStruct Tiles);
 static std::vector<uint8_t> PassToBuffer();
 static TimesOnly Time(const std::vector<uint8_t>& message);
 static ChallengeReject Reject_Case(const std::vector<uint8_t>& message);
 static OpponentPlayState Play_Agent(const std::vector<uint8_t>& message);
 static EndState EndGame(const std::vector<uint8_t>& message);
 static CountTime CountWithTimes(const std::vector<uint8_t>& message);
 static void StringToAscii(std::vector<uint8_t>& T, std::string name);

public:
static WebSocket::pointer ws;
 Comm();
 static void RecieveFromServer(const std::vector<uint8_t>& message);
 static void ThinkingThread();
 static TilesStruct GetRack();
 static ChallengeReject GetRejected();
 static GameState GetGame();
 static TimesOnly GetTimesOnly();
 static CountTime GetCounter();
 static OpponentPlayState GetPlayState();
 static EndState GetEndState();
 static string GetCurrentState();
 static WebSocket::pointer GetWs();
~Comm();

};