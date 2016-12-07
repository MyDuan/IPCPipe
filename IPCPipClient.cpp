// IPCPipClient.cpp : ¥³¥ó¥½©`¥ë ¥¢¥×¥ê¥±©`¥·¥ç¥ó¤Î¥¨¥ó¥È¥ê ¥Ý¥¤¥ó¥È¤ò¶¨Áx¤·¤Þ¤¹¡£
//

#include "stdafx.h"
#include <Windows.h>
#include <string>
#include <iostream>

#define BUFSIZE 4096

DWORD WINAPI InstanceThread(LPVOID);

int _tmain(int argc, _TCHAR* argv[]) {
	HANDLE hPipe, hThread;
	BOOL IsSuccess;
	TCHAR chRequest[BUFSIZE];
    TCHAR chReply[BUFSIZE];
	DWORD cbBytesRead;
	DWORD dwThreadId; 
	LPTSTR lpszPipname = TEXT("\\\\.\\pipe\\Duan");
	while (true) {
		hPipe = CreateFile(   
            lpszPipname,   // pipe name   
            GENERIC_READ |  // read and write access   
            GENERIC_WRITE,   
            0,              // no sharing   
            NULL,           // default security attributes  
            OPEN_EXISTING,  // opens existing pipe   
            0,              // default attributes   
            NULL);          // no template file 
		if (hPipe != INVALID_HANDLE_VALUE) {
			printf("Open pipe successful\n");
			hThread = CreateThread(NULL, 0, InstanceThread, (LPVOID) hPipe, 0, &dwThreadId); 
			break;
		}
		if (!WaitNamedPipe(lpszPipname, 20000)) {   
				printf("Could not open pipe\n");
				break;
		}
	}
	while (true) {

	}
/*	DWORD dwMode = PIPE_READMODE_MESSAGE;   
	IsSuccess = SetNamedPipeHandleState(   
			hPipe,    // pipe handle   
			&dwMode,  // new pipe mode   
			NULL,     // don't set maximum bytes   
			NULL);    // don't set maximum time  
*/
	
	system("pause");
	return 0;
}

DWORD WINAPI InstanceThread(LPVOID lpvParam) {

	LPTSTR lpvMessage=TEXT("Hellow I am client \n");   
	std::wstring strMessage;
    TCHAR chBuf[BUFSIZE];   
	TCHAR chRequest[BUFSIZE];
	DWORD cbWritten;
	DWORD cbBytesRead;
	BOOL IsRSuccess = FALSE;
	BOOL IsWSuccess = FALSE;
	HANDLE hPipe = lpvParam;
	IsWSuccess = WriteFile(   
        hPipe,                  // pipe handle   
        lpvMessage,             // message   
        (lstrlen(lpvMessage)+1)*sizeof(TCHAR), // message length   
        &cbWritten,             // bytes written   
        NULL);      
	while (true) {
		if (IsWSuccess) {
			IsRSuccess = ReadFile(   
			hPipe,        // handle to pipe   
			chRequest,    // buffer to receive data   
			BUFSIZE*sizeof(TCHAR), // size of buffer   
			&cbBytesRead, // number of bytes read   
			NULL);        // not overlapped I/O
		}
		if (IsRSuccess) {
				HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
				printf("The Server say:\n");
				SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 0);
				printf_s("%ls\n", chRequest);
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
	}
	return 0;
}
