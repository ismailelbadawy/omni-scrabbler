#include "GameManager.h"



enum Opponent {
Human,
AI
};


GameManager::GameManager()
{

}


int GameManager::InitGame()
{
    // get mode , if AI init server 
 gui=new GUI();

 gui->initConnection();
 char*message="0,RedArmy,Opponent,00000000000000000000,\0";
 gui->Send(message);
 return 0;

}

void GameManager::Play()
{
if (turn == true){

}
else{
    
   if (Comm::GetGame().OpponentPlayedTiles.MoveType==0) {

   }
   else{

   }
    }

}

// char* GameManager::ConvertMessage(){

// }
GameManager::~GameManager(){

}


