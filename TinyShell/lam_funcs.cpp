#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <windows.h>
#include <sys/types.h>
#include <psapi.h>
#include <tlhelp32.h>
#include "console_addon.h"
#include "shell_functions.h"
#include <vector>
#include<fstream>
#include<vector>
#include<string>
#include<cstring>
#include <assert.h>
#include <stdio.h>
#include <iomanip>
#include <sstream>
#include<string.h>
#include "lam_funcs.h"
#include "duc_funcs.h"
#define MAX_CWD_LENS 128
#define MAX_BUFFER_SIZE 64
#define MAX_TOK_BUFSIZE 64
#define strdup _strdup
LPSTARTUPINFOW si[100];
LPPROCESS_INFORMATION pi[100];
HANDLE hHandless[100];
int status[100];
int n = 0;
HANDLE hForeProcess;
LPCWSTR cString[100];
void openProcessInForeGround(char* s)
{

    wchar_t* wtext;
    mbstowcs(wtext, s, strlen(s) + 1);//Plus null
    cString[n] = strdup(wtext.c_str());
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    if (!CreateProcess(cString[n],
        NULL,     // Command line
        NULL,     // Process handle not inheritable
        NULL,     // Thread handle not inheritable
        FALSE,    // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE,
        NULL, // Use parent's environment block
        NULL, // Use parent's starting directory
        si[n],  // Pointer to STARTUPINFO structure
        pi[n])  // Pointer to PROCESS_INFORMATION structure
        )
    {
        std::wcout << "Changing of directory or opening file not successful!\n";

    }
    else {
        WaitForSingleObject(pi.hProcess, INFINITE); // INFINITE // hProcess: The handle is used to specify the process in all functions that perform operations on the process object.
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
}

void openProcessInBackGround(char* s)
{
    DWORD processId = atoi(s);
    killProcessID(processId);//xóa tiến trình có id là s
    ++n;
    status[n] = 1;
    ZeroMemory(&si[n], sizeof(si[n]));
    si[n].cb = sizeof(si[n]);

    //chuyen doi kieu du lieu  cua s ve cstring[n]
    wchar_t* wtext;
    mbstowcs(wtext, s, strlen(s) + 1);//Plus null
    cString[n] = strdup(wtext.c_str());
    //
    if (!CreateProcess(cString[n],
        NULL,        // Command line
        NULL,        // Process handle not inheritable
        NULL,        // Thread handle not inheritable
        FALSE,       // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE,
        NULL,   // Use parent's environment block
        NULL,   // Use parent's starting directory
        si[n], // Pointer to STARTUPINFO structure
        pi[n] // Pointer to PROCESS_INFORMATION structure
    ))
    {
        TerminateProcess(pi[n].hProcess, 0);
        CloseHandle(pi[n].hThread);
        CloseHandle(pi[n].hProcess);
        n--;
        wprintf(L"Changing of directory or opening file not successful!\n");
        return;
    }
}

int exit(TCHAR** cmdParts, int partCount) {
    if (partCount > 1) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid" << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (std::wcscmp(cmdParts[1], L"?doc") == 0) {
        setTextColor(RED);
        std::wcout << "Command: exit" << std::endl;
        exit(1);
        return 0;
    }

    return 1;
}

//chạy tiến trình trên bg hoặc fg
int runProcess(TCHAR** cmdParts, int partCount) {

    if (wcscmp(cmdParts[2].c_str(), "f") == 0) {
        openProcessInForeGround(cmdParts[3].c_str());     //loi ko chuyen TCHAR ve char dc
    }
    if (wcscmp(cmdParts[2].c_str(), "b") == 0) {
        openProcessInBackGround(cmdParts[3].c_str());
    }
    return 0;
}

// in ra các tiến trình đang chạy ??
int getProcessListAll(TCHAR** cmdParts, int partCount) {
    if (partCount < 2) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid" << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (std::wcscmp(cmdParts[1], L"?doc") == 0) {
        std::wcout << "proc:" << std::endl;
        std::wcout << "Description:\tUsed to list all processes running." << std::endl;
        std::wcout << "Usage:\t\tget list <characters>" << std::endl;
        std::wcout << std::endl;
    }
    else {
        std::wcout << "Test function is executed. Print message:" << std::endl;
        for (int i = 1; i < partCount; i++) {
            std::wcout << cmdParts[i] << std::endl;
        }
        std::wcout << std::endl;
    }
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32; // Cấu trúc của tiến trình khi được gọi snapshot

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // Chụp lại các tiến trình
    // Nếu hProcessSnap trả về lỗi return 0
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        std::wcout << "ERROR: CreateToolhelp32Snapshot Fail " << GetLastError() << std::endl;
        return 1;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);
    // Kiểm tra thằng đầu tiên
    if (!Process32First(hProcessSnap, &pe32)) {
        // Nếu lỗi in ra...
        std::wcout << "ERROR: Process32First Fail " << GetLastError() << std::endl;
        return 1;
    }
    wprintf(L"%-50s%-20s%-20s\n", L"Process Name", L"Process ID", L"Parent Process ID");
    wprintf(L"%-50s%-20s%-20s\n", L"----------------------------------", L"----------", L"-----------");

    do {
        wprintf(L"%-50s%-20d%-20d\n", pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);
    } while (Process32Next(hProcessSnap, &pe32)); CloseHandle(hProcessSnap);
    return 0;
}


//chạy file *bat
//int runBat(TCHAR** cmdParts,int partCount)
//{
//    string s = cmdParts.to_string();
//    ifstream file(s);
//    if (file.is_open())
//    {
//        char* line;
//        while (getline(file, line))
//        {
//            run(line,1); 
//        }
//    }
//    else
//    {
//        std::wcout << "File " << s << " do not exist in this directory\n";
//    }
//}
void sighandler(int signum) {

    if (hForeProcess != NULL) {
        TerminateProcess(hForeProcess, 0);
        hForeProcess = NULL;
    }
    exit(1);
}
/* Lệnh ngắt bằng Ctrl C
      signal(SIGINT, sighandler);
*/

//Tìm process 
int findProcessID(TCHAR** cmdParts, int partCount) {
    if (partCount < 3) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid." << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (std::wcscmp(cmdParts[1], L"?doc") == 0) {
        std::wcout << "proc idof:" << std::endl;
        std::wcout << "Description:\tPrint a process ." << std::endl;
        std::wcout << "Usage:";
        setTextColor(OCEAN);
    }
    else {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid." << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32; // Cấu trúc của tiến trình khi được gọi snap

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // Chụp lại các tiến trình
    // Nếu trả về lỗi return 0
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return 0;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Kiểm tra thằng đầu tiên
    if (!Process32First(hProcessSnap, &pe32)) {
        return 1;
    }
    wprintf(L"%-50s%-20s%-20s\n", L"Process Name", L"Process ID", L"Parent Process ID");
    wprintf(L"%-50s%-20s%-20s\n", L"----------------------------------", L"----------", L"-----------");
    do {
        char* str = cmdParts[2];
        char* buffer = new char[20];

        std::strcpy(buffer, str);
        if (std::strcmp(buffer, pe32.szExeFile.c_str()) == 0) {
            // Nếu pe32.szExeFile trùng với tên tiến trình thì in ra
            wprintf(L"%-50s%-20d%-20d\n", pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);
        }
    } while (Process32Next(hProcessSnap, &pe32)); CloseHandle(hProcessSnap);
    return 0;
}
