#include"GUI.h"
#define BUFSIZE 512
GUI::GUI()
{
}

int GUI::initConnection()
{
   BOOL   fConnected = FALSE; 
   DWORD  dwThreadId = 0; 
   HANDLE hPipe = INVALID_HANDLE_VALUE, hThread = NULL; 
   LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\newpipe");
_tprintf(TEXT("\nPipe Server: Connection is initiated %s\n"), lpszPipename);

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
    return 1;
}

bool ConnectNamedP=ConnectNamedPipe(hnamedPipe,NULL);
 if(ConnectNamedP==FALSE)
    {
        printf("Connection Failed");
        return 1;
    }
   printf("Connected");
   return 0;

}

int GUI::Send(char * Message)
{
    DWORD cbResponse,cbWritten;
    cbResponse = sizeof(Message);
   
    if(!WriteFile(
    hnamedPipe,
    Message,
    cbResponse,
    &cbWritten,
    NULL))
    {
        wprintf(L"failure w/err 0x%08lx\n",GetLastError);
        return 1;
    }
    printf("Send Bytes");
    return 0;
}


GUI::~GUI(){
    
}