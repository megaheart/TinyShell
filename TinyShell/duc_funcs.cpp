#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <windows.h>
#include <sys/types.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <fstream>
#include <codecvt>
#include <sstream>
#include <iomanip>
#include <string>
#include <io.h>
#include <fcntl.h>
#include <filesystem>
#include <chrono>
#include <ctime>
#include "console_addon.h"
#include "shell_functions.h"
#include "process.h"

int killProcessID(DWORD process_ID) {
	HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_ID);
	if (hprocess == NULL) {
		setTextColor(RED);
		std::wcout << "ERROR: Failed" << std::endl;
		setTextColor(WHITE);
		return 1;
	}
	if (!TerminateProcess(hprocess, 0)) {
		setTextColor(RED);
		std::wcout << L"Error: ";
		setTextColor(WHITE);
		wprintf(L"Cannot terminate process.\n");
		return 1;
	}
	std::vector<ProcessInfo>* pis = getProcessInfos();
	int processCount = getProcessInfos()->size();
	for (int i = 0; i < processCount; ++i)
	{
		if ((*pis)[i].pi->dwProcessId == process_ID) {
			delete (*pis)[i].pi;
			delete (*pis)[i].name;
			delete (*pis)[i].si;
			pis->erase(pis->begin() + i);
			break;
		}
		
	}
	return 1;

}
int stopProcessID(DWORD process_ID) {
	//HANDLE hthread;
	std::vector<ProcessInfo>* pis = getProcessInfos();
	int processCount = getProcessInfos()->size();
	for (int i = 0; i < processCount; ++i)
	{
		if ((*pis)[i].pi->dwProcessId == process_ID) {
			DWORD dwExitCode;

			GetExitCodeProcess((*pis)[i].pi->hProcess, &dwExitCode);

			if (dwExitCode != STILL_ACTIVE) {
				TerminateProcess((*pis)[i].pi->hProcess, 0);
				CloseHandle((*pis)[i].pi->hThread);
				CloseHandle((*pis)[i].pi->hProcess);
				delete (*pis)[i].pi;
				delete (*pis)[i].si;
				delete (*pis)[i].name;
				pis->erase(pis->begin() + i);
				processCount--;
				i--;
				

				break;
			}
			if (SuspendThread((*pis)[i].pi->hThread) == -1) {
				setTextColor(RED);
				std::wcout << "ERROR:";
				setTextColor(WHITE);
				std::wcout << " Cannot stop process (ID = ";
				setTextColor(OCEAN);
				std::wcout << process_ID;
				setTextColor(WHITE);
				std::wcout << ")." << std::endl << std::endl;
				return 1;
			}
			else {
				(*pis)[i].status = PROC_STAT_STOP;
				std::wcout << "Process (ID = ";
				setTextColor(OCEAN);
				std::wcout << process_ID;
				setTextColor(WHITE);
				std::wcout << ") is stopped." << std::endl << std::endl;
				return 0;
			}
		}

	}
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 th32;
	HANDLE hthread;
	if (hThreadSnap == INVALID_HANDLE_VALUE) {
		setTextColor(RED);
		std::wcout << "ERROR: CreateToolhelp32Snapshot" << GetLastError();
		setTextColor(WHITE);
		return 1;
	}
	th32.dwSize = sizeof(THREADENTRY32);
	if (!Thread32First(hThreadSnap, &th32)) {
		setTextColor(RED);
		std::wcout << "Thread32First Fail: " << GetLastError();
		setTextColor(WHITE);
		CloseHandle(hThreadSnap); 
		return 1;
	}
	do {
		if (th32.th32OwnerProcessID == process_ID) {
			hthread = OpenThread(THREAD_ALL_ACCESS, FALSE, th32.th32ThreadID);
			if (SuspendThread(hthread) == -1) {
				setTextColor(RED);
				std::wcout << "ERROR:";
				setTextColor(WHITE);
				std::wcout << " Cannot stop process (ID = ";
				setTextColor(OCEAN);
				std::wcout << process_ID;
				setTextColor(WHITE);
				std::wcout << ")." << std::endl << std::endl;
				CloseHandle(hThreadSnap);
				return 1;
			}
			else {

				std::wcout << "Process (ID = ";
				setTextColor(OCEAN);
				std::wcout << process_ID;
				setTextColor(WHITE);
				std::wcout << ") is stopped." << std::endl << std::endl;
				CloseHandle(hThreadSnap);
				return 0;
			}
		}
	} while (Thread32Next(hThreadSnap, &th32));	CloseHandle(hThreadSnap);
	setTextColor(RED);
	std::wcout << "ERROR:";
	setTextColor(WHITE);
	std::wcout << " Process (ID = ";
	setTextColor(OCEAN);
	std::wcout << process_ID;
	setTextColor(WHITE);
	std::wcout << ") does not exist." << std::endl << std::endl;
	return 1;
}
int resumeProcessID(DWORD process_ID) {
	std::vector<ProcessInfo>* pis = getProcessInfos();
	int processCount = getProcessInfos()->size();
	for (int i = 0; i < processCount; ++i)
	{
		if ((*pis)[i].pi->dwProcessId == process_ID) {
			DWORD dwExitCode;

			GetExitCodeProcess((*pis)[i].pi->hProcess, &dwExitCode);

			if (dwExitCode != STILL_ACTIVE) {
				TerminateProcess((*pis)[i].pi->hProcess, 0);
				CloseHandle((*pis)[i].pi->hThread);
				CloseHandle((*pis)[i].pi->hProcess);
				delete (*pis)[i].pi;
				delete (*pis)[i].si;
				delete (*pis)[i].name;
				pis->erase(pis->begin() + i);
				processCount--;
				i--;


				break;
			}
			if (ResumeThread((*pis)[i].pi->hThread) == -1) {
				setTextColor(RED);
				std::wcout << "ERROR:";
				setTextColor(WHITE);
				std::wcout << " Cannot resume process (ID = ";
				setTextColor(OCEAN);
				std::wcout << process_ID;
				setTextColor(WHITE);
				std::wcout << ")." << std::endl << std::endl;
				return 1;
			}
			else {
				(*pis)[i].status = PROC_STAT_RUNNING;
				std::wcout << "Process (ID = ";
				setTextColor(OCEAN);
				std::wcout << process_ID;
				setTextColor(WHITE);
				std::wcout << ") is resumed." << std::endl << std::endl;
				return 0;
			}
		}

	}
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 th32;
	HANDLE hthread;
	if (hThreadSnap == INVALID_HANDLE_VALUE) {
		setTextColor(RED);
		std::wcout << "ERROR: CreateToolhelp32Snapshot" << GetLastError();
		setTextColor(WHITE);
		return 0;
	}
	th32.dwSize = sizeof(THREADENTRY32);
	if (!Thread32First(hThreadSnap, &th32)) {
		setTextColor(RED);
		std::wcout << L"Thread32First Fail " << GetLastError();
		setTextColor(WHITE);
		CloseHandle(hThreadSnap);  
		return 0;
	}
	do {
		if (th32.th32OwnerProcessID == process_ID) {
			hthread = OpenThread(THREAD_ALL_ACCESS, FALSE, th32.th32ThreadID);
			if (ResumeThread(hthread) == -1) {
				setTextColor(RED);
				std::wcout << "ERROR:";
				setTextColor(WHITE);
				std::wcout << " Cannot resume process (ID = ";
				setTextColor(OCEAN);
				std::wcout << process_ID;
				setTextColor(WHITE);
				std::wcout << ")." << std::endl << std::endl;
				CloseHandle(hThreadSnap);
				return 1;
			}
			else {
				std::wcout << "Process (ID = ";
				setTextColor(OCEAN);
				std::wcout << process_ID;
				setTextColor(WHITE);
				std::wcout << ") is resumed." << std::endl << std::endl;
				CloseHandle(hThreadSnap);
				return 0;
			}
		}
	} while (Thread32Next(hThreadSnap, &th32)); CloseHandle(hThreadSnap);
	setTextColor(RED);
	std::wcout << "ERROR:";
	setTextColor(WHITE);
	std::wcout << " Process (ID = ";
	setTextColor(OCEAN);
	std::wcout << process_ID;
	setTextColor(WHITE);
	std::wcout << ") does not exist." << std::endl << std::endl;
	return 1;
}
int cls(TCHAR** cmdParts, int partsCount) {
	if (partsCount > 1 && std::wcscmp(cmdParts[1], L"?doc") == 0) {
		std::wcout << "path:" << std::endl;
		std::wcout << "Description:\tUsed to delete console." << std::endl;
		std::wcout << "Usage:";
		setTextColor(OCEAN);
		std::wcout << "\t\tcls" << std::endl;
		setTextColor(WHITE);
		std::wcout << std::endl;
		return 0;
	}
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SMALL_RECT scrollRect;
	COORD scrollTarget;
	CHAR_INFO fill;
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		return 0;
	}
	scrollRect.Left = 0;
	scrollRect.Top = 0;
	scrollRect.Right = csbi.dwSize.X;
	scrollRect.Bottom = csbi.dwSize.Y;

	scrollTarget.X = 0;
	scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);
	fill.Char.UnicodeChar = TEXT(' ');
	fill.Attributes = csbi.wAttributes;
	ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);
	csbi.dwCursorPosition.X = 0;
	csbi.dwCursorPosition.Y = 0;

	SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
	return 0;
	
}
int echo(TCHAR** cmdParts, int partsCount) {
	if (partsCount == 1 || std::wcscmp(cmdParts[1], L"?doc") == 0) {
		std::wcout << "path:" << std::endl;
		std::wcout << "Description:\tUsed to display the <message> content." << std::endl;
		std::wcout << "Usage:";
		setTextColor(OCEAN);
		std::wcout << "\t\techo <message>" << std::endl;
		setTextColor(WHITE);
		std::wcout << std::endl;
		return 0;
	}
	int i = 1;
	while (i < partsCount) {
		std::wcout << cmdParts[i];
		std::wcout << " ";
		i++;
	}
	std::wcout << std::endl;
	std::wcout << std::endl;
	return 0;
}


int read(TCHAR** cmdParts, int partCount) {
	if (partCount == 1 || std::wcscmp(cmdParts[1], L"?doc") == 0) {
		std::wcout << "path:" << std::endl;
		std::wcout << "Description:\tUsed to read text file." << std::endl;
		std::wcout << "Usage:";
		setTextColor(OCEAN);
		std::wcout << "\t\tread <path>" << std::endl;
		setTextColor(WHITE);
		std::wcout << std::endl;
		return 0;
	}
	if (partCount > 2) {
		setTextColor(RED);
		std::wcout << "Error: ";
		setTextColor(WHITE);
		std::wcout << "Too many parameters (accept only one)." << std::endl;
		return 1;
	}
	std::ifstream infile(cmdParts[1], std::ios::binary);
	std::string buffer;
	std::vector<TCHAR> result;
	if(infile.is_open()){
		while (std::getline(infile, buffer)) {
			size_t count = MultiByteToWideChar(CP_UTF8, 0, buffer.c_str(), buffer.size(), NULL, 0);
			if (count + 1 > result.size()) {
				result.resize(count + 1);
			}
			MultiByteToWideChar(CP_UTF8, 0, buffer.c_str(), buffer.size(), &result[0], count);
			result[count] = '\0';
			std::wcout << &result[0] << std::endl;
			
		}
		std::wcout << std::endl;
		return 0;
	}
    else {
		setTextColor(RED);
		std::wcout << L"Unable to open file: " << cmdParts << std::endl;
		setTextColor(WHITE);
		return 1;
	}
}