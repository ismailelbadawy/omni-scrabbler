#pragma once
#include<iostream>
#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>
using namespace std;
class GUI
{
public:
HANDLE hnamedPipe;
DWORD WINAPI InstanceThread(LPVOID); 
VOID GetAnswerToRequest(LPTSTR, LPTSTR, LPDWORD); 
GUI();
int initConnection();
int Send(char *);
~GUI();
};
