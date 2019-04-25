#pragma once
#include<iostream>

#include "Structs.cpp"
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

TilesStruct rack;
ChallengeReject rejected;
GameState game;
TimesOnly timesonly;
CountTime counter;
PlayState playstate;
EndState endstate;
string CurrentState;
std::vector<uint8_t> T;
static WebSocket::pointer ws;

GameState BufferToGameState(const std::vector<uint8_t>& message);
std::vector<uint8_t> PlayToBuffer(Play Move);
TimesOnly Time(const std::vector<uint8_t>& message);
ChallengeReject Reject_Case(const std::vector<uint8_t>& message);
PlayState Play_Agent(const std::vector<uint8_t>& message);
EndState EndGame(const std::vector<uint8_t>& message);
CountTime CountWithTimes(const std::vector<uint8_t>& message);
void StringToAscii(std::vector<uint8_t>& T, std::string name);

public:
Comm();
void RecieveFromServer(const std::vector<uint8_t>& message);
void ThinkingThread();
TilesStruct GetRack();
ChallengeReject GetRejected();
GameState GetGame();
TimesOnly GetTimesOnly();
CountTime GetCounter();
PlayState GetPlayState();
EndState GetEndState();
string GetCurrentState();
~Comm();

}