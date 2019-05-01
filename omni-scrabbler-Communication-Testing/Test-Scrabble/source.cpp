#include<iostream>
using namespace std;
#include"Comm.h"

#include"easywsclient.hpp"
#include<thread>
using easywsclient::WebSocket;
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include"GameManager.h"

int main(){
    std::cout<<"hello"<<std::endl;
  //  Comm* client=new Comm();
  
  // while(true)
  // {
  //  std::thread thread1(&Comm::ThinkingThread);

	// while (Comm::ws->getReadyState() != WebSocket::CLOSED) {

	// 	Comm::ws->poll();

	// 	Comm::ws->dispatchBinary(&Comm::RecieveFromServer);

	// }
  // }
GameManager manager;
manager.InitGame();

     return 0;
}


