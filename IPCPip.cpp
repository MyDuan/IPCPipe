// IPCPip.cpp : ¥³¥ó¥½©`¥ë ¥¢¥×¥ê¥±©`¥·¥ç¥ó¤Î¥¨¥ó¥È¥ê ¥Ý¥¤¥ó¥È¤ò¶¨Áx¤·¤Þ¤¹¡£
//

#include "stdafx.h"
#include <Windows.h>
#define BUFSIZE 4096

int _tmain(int argc, _TCHAR* argv[])
{
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\Duan");  
	LPTSTR lpvMessage=TEXT("Default message from Server"); 
	BOOL IsConnected, IsSuccess;
	HANDLE hPipe;
	TCHAR chRequest[BUFSIZE];   
    TCHAR chReply[BUFSIZE];
	DWORD cbBytesRead;
	DWORD cbWritten;
	while (true) {
		hPipe = CreateNamedPipe(
		lpszPipename,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE|
		PIPE_READMODE_MESSAGE|
		PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		BUFSIZE,
		BUFSIZE,
		0,
		NULL);
		if (hPipe == NULL) {
			printf("CreatPipe failed");
		}
		IsConnected = ConnectNamedPipe(hPipe, NULL);
		if (IsConnected) {
			printf("Connect pipe");  
			break;
		}
	}
	while (true)
	{
		IsSuccess = ReadFile(   
            hPipe,        // handle to pipe   
            chRequest,    // buffer to receive data   
            BUFSIZE*sizeof(TCHAR), // size of buffer   
            &cbBytesRead, // number of bytes read   
            NULL);        // not overlapped I/O
		if (IsSuccess) {
			printf("%ls", chRequest);
			IsSuccess = WriteFile(   
				 hPipe,                  // pipe handle   
				lpvMessage,             // message   
				(lstrlen(lpvMessage)+1)*sizeof(TCHAR), // message length   
				&cbWritten,             // bytes written   
				NULL);      
			if (IsSuccess)
			{
				break;
			}
		}
	}
	
	system("pause");
	return 0;
	
}

