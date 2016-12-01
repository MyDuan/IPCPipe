// IPCPipClient.cpp : ¥³¥ó¥½©`¥ë ¥¢¥×¥ê¥±©`¥·¥ç¥ó¤Î¥¨¥ó¥È¥ê ¥Ý¥¤¥ó¥È¤ò¶¨Áx¤·¤Þ¤¹¡£
//

#include "stdafx.h"
#include <Windows.h>
#define BUFSIZE 512
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hPipe;
	BOOL IsSuccess;
	TCHAR chRequest[BUFSIZE];   
    TCHAR chReply[BUFSIZE];
	DWORD cbBytesRead;
	LPTSTR lpszPipname = TEXT("\\\\.\\pipe\\Duan");
	while (true)
	{
		hPipe = CreateFile(   
            lpszPipname,   // pipe name   
            GENERIC_READ |  // read and write access   
            GENERIC_WRITE,   
            0,              // no sharing   
            NULL,           // default security attributes  
            OPEN_EXISTING,  // opens existing pipe   
            0,              // default attributes   
            NULL);          // no template file 
		if (hPipe != INVALID_HANDLE_VALUE)   
            break;
		if (!WaitNamedPipe(lpszPipname, 20000))   
		{   
				printf("Could not open pipe");   
            //return 0;  
			//system("pause");
		}   
	}
	DWORD dwMode = PIPE_READMODE_MESSAGE;   
	IsSuccess = SetNamedPipeHandleState(   
			hPipe,    // pipe handle   
			&dwMode,  // new pipe mode   
			NULL,     // don't set maximum bytes   
			NULL);    // don't set maximum time  
	
	LPTSTR lpvMessage=TEXT("Default message from client");   
    TCHAR chBuf[BUFSIZE];   
	DWORD cbWritten;
	while (true)
	{
		IsSuccess = WriteFile(   
        hPipe,                  // pipe handle   
        lpvMessage,             // message   
        (lstrlen(lpvMessage)+1)*sizeof(TCHAR), // message length   
        &cbWritten,             // bytes written   
        NULL);      
		if (IsSuccess)
		{
			while (true)
			{
				IsSuccess = ReadFile(   
				hPipe,        // handle to pipe   
				chRequest,    // buffer to receive data   
				BUFSIZE*sizeof(TCHAR), // size of buffer   
				&cbBytesRead, // number of bytes read   
				NULL);        // not overlapped I/O
				if (IsSuccess)
				{
					printf("%ls", chRequest);
					break;
				}
			}
			break;
		}
	}
	system("pause");
	return 0;
	
}

