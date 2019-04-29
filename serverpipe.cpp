#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>
 #include<unistd.h>

#define BUFSIZE 512
#define MESSAGE L"TestMessage"
 
DWORD WINAPI InstanceThread(LPVOID); 
VOID GetAnswerToRequest(LPTSTR, LPTSTR, LPDWORD); 

int _tmain(VOID) 
{ 
   BOOL   fConnected = FALSE; 
   DWORD  dwThreadId = 0; 
   HANDLE hPipe = INVALID_HANDLE_VALUE, hThread = NULL; 
   LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\newpipe"); 
 
// The main loop creates an instance of the named pipe and 
// then waits for a client to connect to it. When the client 
// connects, a thread is created to handle communications 
// with that client, and this loop is free to wait for the
// next client connect request. It is an infinite loop.
 
    
 _tprintf( TEXT("\nPipe Server: Main thread awaiting client connection on %s\n"), lpszPipename);
    

HANDLE hnamedPipe = INVALID_HANDLE_VALUE;

hnamedPipe = CreateNamedPipe(
    lpszPipename,
    PIPE_ACCESS_DUPLEX,
    PIPE_TYPE_MESSAGE|
    PIPE_READMODE_MESSAGE|
    PIPE_WAIT,
    PIPE_UNLIMITED_INSTANCES,
    1024,
    1024,
    NMPWAIT_USE_DEFAULT_WAIT,
    NULL);

if(hnamedPipe == INVALID_HANDLE_VALUE)
{
       printf("Failed");
}
   printf("waiting for client");

    while(!ConnectNamedPipe(hnamedPipe,NULL))
    {
    }

   printf("Connected\n");
char*chResponse[5]={"0,0:00,Fatma,Youssef,000000000000000000,\0","5,0.01,000,000,HYTRELL,0000000000,1,2,1,\0","1,00.02,123,898,HYRKOFK,FATMAKL,07,01,0,\0","1,00.02,123,898,GUNSUNL,FORK000,10,10,1,\0","-1,\0"};
    for (int i=0;i<5;i++){
printf("You entered: %s \n",chResponse[i]);
    DWORD cbResponse,cbWritten;
    cbResponse = strlen(chResponse[i]);
    printf("%d",cbResponse);
    if(!WriteFile(
    hnamedPipe,
    chResponse[i],
    cbResponse,
    &cbWritten,
    NULL))
    {
        wprintf(L"failure w/err 0x%08lx\n",GetLastError);
    }
    printf("Send Bytes\n");
    // sleep(5);
  // sleep(5);
}
DisconnectNamedPipe(hPipe);
 //CloseHandle( hPipe );
//}
}