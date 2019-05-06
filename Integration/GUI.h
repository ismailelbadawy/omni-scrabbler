#pragma once
#include<iostream>
#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>
#define BUFSIZE 512
using namespace std;
class GUI
{
public:
HANDLE hnamedPipe;
HANDLE hThread;
DWORD WINAPI InstanceThread(LPVOID); 
VOID GetAnswerToRequest(LPTSTR, LPTSTR, LPDWORD); 
TCHAR  chBuf[BUFSIZE];
BOOL   fSuccess; 
BOOL   fConnected;
DWORD  cbRead, cbToWrite, dwMode;
GUI();
int initConnection();
int Send(char *);
char* Receive();
~GUI();
};
