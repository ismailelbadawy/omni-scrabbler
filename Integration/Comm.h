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



 static TilesStruct exchanged;
 static ChallengeReject rejected;
 static GameState game;
 static TimesOnly timesonly;
 static CountTime counter;
 static ServerPlay play;
 static OpponentPlayState playstate;
 static EndState endstate;
 static string CurrentState;

 static std::vector<uint8_t> T;
 static GameState BufferToGameState(const std::vector<uint8_t>& message);
 static std::vector<uint8_t> PlayToBuffer(ServerPlay Move);
 static std::vector<uint8_t> ExchangeToBuffer(TilesStruct Tiles);
 static std::vector<uint8_t> PassToBuffer();
 static TimesOnly Time(const std::vector<uint8_t>& message);
 static ChallengeReject Reject_Case(const std::vector<uint8_t>& message);
 static OpponentPlayState Play_Agent(const std::vector<uint8_t>& message);
 static EndState EndGame(const std::vector<uint8_t>& message);
 static CountTime CountWithTimes(const std::vector<uint8_t>& message);
 static void StringToAscii(std::vector<uint8_t>& T, std::string name);

public:
enum States{
	RECEIVE_RACK=0,
	THINKING=1,
	PLAY_RESPONSE=2,
	EXCHANGE_RESPONSE=3,
	PLAY_OPPONENT=4,
	EXCHANGE_OPPONENT=5,
	PASS_OPPONENT=6,        // might need them for learning
	IDLE=7,
	ENDCONNECTION=8,
};
static WebSocket::pointer ws;
static bool StartAction;
static bool  ActionFinished;
static bool simulationfinished; 
static States mystate;  
static string agentmove;

 Comm();
 static void RecieveFromServer(const std::vector<uint8_t>& message);
 static void ThinkingThread();
 static TilesStruct GetExchangedTiles();
 static ChallengeReject GetRejected();
 static GameState GetGame();
 static void SetPlay(Play agentplay);
 static void SetMove(string move);
 static string GetMove();
 static Play GetPlay();
 static void SetExchanged(TilesStruct tiles);
 static TimesOnly GetTimesOnly();
 static CountTime GetCounter();
 static OpponentPlayState GetPlayState();
 static EndState GetEndState();
 static string GetCurrentState();
 static WebSocket::pointer GetWs();
 static void ConnectServer();
~Comm();

};