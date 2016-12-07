// IPCPip.cpp : ¥³¥ó¥½©`¥ë ¥¢¥×¥ê¥±©`¥·¥ç¥ó¤Î¥¨¥ó¥È¥ê ¥Ý¥¤¥ó¥È¤ò¶¨Áx¤·¤Þ¤¹¡£
//

#include "stdafx.h"
#include <Windows.h>
#include <string>
#include <iostream>

#define BUFSIZE 4096
DWORD WINAPI InstanceThread(LPVOID);
int _tmain(int argc, _TCHAR* argv[]) {
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\Duan"); 
	BOOL IsConnected, IsSuccess;
	HANDLE hPipe, hThread;
	DWORD dwThreadId;   

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
			printf("Connect pipe \n");  
			hThread = CreateThread(NULL, 0, InstanceThread, (LPVOID) hPipe, 0, &dwThreadId); 
		}
	}
	
	system("pause");
	return 0;
	
}

DWORD WINAPI InstanceThread(LPVOID lpvParam) { 

	LPTSTR lpvMessage=TEXT("Default message from Server \n"); 
	std::wstring strMessage;
	BOOL IsRSuccess = FALSE;
	BOOL IsWSuccess = FALSE;
	HANDLE hPipe = lpvParam;
	TCHAR chRequest[BUFSIZE];
	DWORD cbBytesRead;
	DWORD cbWritten;
	IsRSuccess = ReadFile(   
            hPipe,        // handle to pipe   
            chRequest,    // buffer to receive data   
            BUFSIZE*sizeof(TCHAR), // size of buffer   
            &cbBytesRead, // number of bytes read   
            NULL);        // not overlapped I/O
	while (true)
	{
		if (IsRSuccess) {
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
			printf("The Client say:\n");
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 0);
			printf("%ls\n", chRequest);
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			printf("I say:\n");
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 0);
			//std::wcin>> strMessage;
			getline(std::wcin, strMessage);
			lpvMessage = (LPTSTR)strMessage.c_str();
			IsWSuccess = WriteFile(   
				 hPipe,                  // pipe handle   
				lpvMessage,             // message   
				(lstrlen(lpvMessage)+1)*sizeof(TCHAR), // message length   
				&cbWritten,             // bytes written   
				NULL);      
		}
		if (IsWSuccess) {
			IsRSuccess = ReadFile(   
            hPipe,        // handle to pipe   
            chRequest,    // buffer to receive data   
            BUFSIZE*sizeof(TCHAR), // size of buffer   
            &cbBytesRead, // number of bytes read   
            NULL);        // not overlapped I/O
		}
	}
	return 0;
}