#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <windows.h>
#include <sys/types.h>
#include <psapi.h>
#include <tlhelp32.h>
#include "console_addon.h"
#include "shell_functions.h"
//Closing processID process
int killProcessID(DWORD process_ID) {
	// Opening excuting process with processID
	HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_ID);
	// 
	if (hprocess == NULL) {
		std::wcout << "ERROR: Failed" << std::endl;
		return 1;
	}
	// Closing hprocess
	if (!TerminateProcess(hprocess, 0)) {
		return 0;
	}
	return 1;
}
//
int stopProcessID(DWORD process_ID) {
    //
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 th32; //
	HANDLE hthread;
	//
	if (hThreadSnap == INVALID_HANDLE_VALUE) {
		std::wcout << "ERROR: CreateToolhelp32Snapshot" << GetLastError();
		return 0;
	}
	th32.dwSize = sizeof(THREADENTRY32);
	// 
	if (!Thread32First(hThreadSnap, &th32)) {
		std::wcout << "Thread32First Fail " << GetLastError(); // 
		CloseHandle(hThreadSnap);          // 
		return 0;
	}
	// D
	do {
		//
		if (th32.th32OwnerProcessID == process_ID) {
			hthread = OpenThread(THREAD_ALL_ACCESS, FALSE, th32.th32ThreadID); // 
			//
			if (SuspendThread(hthread) == -1) {
				return 0;
			}
		}
	} while (Thread32Next(hThreadSnap, &th32));	CloseHandle(hThreadSnap);
	return 1;
}
int resumeProcessID(DWORD process_ID) {
	//
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 th32; // 
	HANDLE hthread;
	//
	if (hThreadSnap == INVALID_HANDLE_VALUE) {
		std::wcout << "ERROR: CreateToolhelp32Snapshot" << GetLastError();
		return 0;
	}
	th32.dwSize = sizeof(THREADENTRY32);
	// 
	if (!Thread32First(hThreadSnap, &th32)) {
		std::wcout << "Thread32First Fail " << GetLastError(); // 
		CloseHandle(hThreadSnap);          // 
		return 0;
	}
	// 
	do {
		// 
		if (th32.th32OwnerProcessID == process_ID) {
			hthread = OpenThread(THREAD_ALL_ACCESS, FALSE, th32.th32ThreadID); // 
				// 
			if (ResumeThread(hthread) == -1) {
				return 0;
			}
		}
	} while (Thread32Next(hThreadSnap, &th32)); CloseHandle(hThreadSnap);
	return 1;
}
int cls(TCHAR** cmdParts) {
	if (wcscmp(cmdParts[0], L"cls") == 0) {
		HANDLE hConsole;
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		SMALL_RECT scrollRect;
		COORD scrollTarget;
		CHAR_INFO fill;

		// Get the number of character cells in the current buffer.
		if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		{
			return 0;
		}

		// Scroll the rectangle of the entire buffer.
		scrollRect.Left = 0;
		scrollRect.Top = 0;
		scrollRect.Right = csbi.dwSize.X;
		scrollRect.Bottom = csbi.dwSize.Y;

		// Scroll it upwards off the top of the buffer with a magnitude of the entire height.
		scrollTarget.X = 0;
		scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

		// Fill with empty spaces with the buffer's default text attribute.
		fill.Char.UnicodeChar = TEXT(' ');
		fill.Attributes = csbi.wAttributes;

		// Do the scroll
		ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

		// Move the cursor to the top left corner too.
		csbi.dwCursorPosition.X = 0;
		csbi.dwCursorPosition.Y = 0;

		SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
	}
	return 0;
}
int echo(TCHAR** cmdParts) {
	if (cmdParts[1] == NULL) {
		std::wcout << "ERROR: Echo + [message]" << std::endl;
		return 0;
	}
	int i = 0;
	while (cmdParts[++i] != NULL) {
		for (int j = 0; j < wcslen(cmdParts[i]); j++) {
			std::wcout << cmdParts[i][j];
		}
		std::wcout << " ";
	}
	std::wcout << std::endl;
	return 0;
}