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
#include "process.h"
#include "lam_funcs.h"
#include "duc_funcs.h"
#define MAX_CWD_LENS 128
#define MAX_BUFFER_SIZE 64
#define MAX_TOK_BUFSIZE 64
#define wcsdup _wcsdup


//HANDLE hHandless[100];
HANDLE hForeProcess = NULL;

void openProcessInForeGround(TCHAR* s)
{
    ProcessInfo procInfo;
    procInfo.pi = (LPPROCESS_INFORMATION) calloc(1, sizeof(PROCESS_INFORMATION));
    procInfo.si = (LPSTARTUPINFOW) calloc(1, sizeof(STARTUPINFOW));
    procInfo.si->cb = sizeof(STARTUPINFOW);
    procInfo.name = wcsdup(s);

    if (!CreateProcess(procInfo.name,
        NULL,     // Command line
        NULL,     // Process handle not inheritable
        NULL,     // Thread handle not inheritable
        FALSE,    // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE,
        NULL, // Use parent's environment block
        NULL, // Use parent's starting directory
        procInfo.si,  // Pointer to STARTUPINFO structure
        procInfo.pi)  // Pointer to PROCESS_INFORMATION structure
        )
    {
        std::wcout << "Changing of directory or opening file not successful!\n";
        delete procInfo.pi;
        delete procInfo.si;
        delete procInfo.name;
    }
    else {
        hForeProcess = procInfo.pi;
        getProcessInfos()->push_back(procInfo);
        WaitForSingleObject(procInfo.pi->hProcess, INFINITE); // INFINITE // hProcess: The handle is used to specify the process in all functions that perform operations on the process object.
        CloseHandle(procInfo.pi->hThread);
        CloseHandle(procInfo.pi->hProcess);
        delete procInfo.pi;
        delete procInfo.si;
        delete procInfo.name;
        hForeProcess = NULL;
    }
}

void openProcessInBackGround(TCHAR* s)
{
    ProcessInfo procInfo;
    DWORD processId = _wtoi(s);
    killProcessID(processId);//xóa tiến trình có id là s
    procInfo.status = 1;
    procInfo.pi = (LPPROCESS_INFORMATION)calloc(1, sizeof(PROCESS_INFORMATION));
    procInfo.si = (LPSTARTUPINFOW)calloc(1, sizeof(STARTUPINFOW));
    procInfo.si->cb = sizeof(STARTUPINFOW);
    procInfo.name = wcsdup(s);

    //chuyen doi kieu du lieu  cua s ve cstring[n]
    //
    if (!CreateProcess(procInfo.name,
        NULL,        // Command line
        NULL,        // Process handle not inheritable
        NULL,        // Thread handle not inheritable
        FALSE,       // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE,
        NULL,   // Use parent's environment block
        NULL,   // Use parent's starting directory
        procInfo.si, // Pointer to STARTUPINFO structure
        procInfo.pi // Pointer to PROCESS_INFORMATION structure
    ))
    {
        TerminateProcess(procInfo.pi->hProcess, 0);
        CloseHandle(procInfo.pi->hThread);
        CloseHandle(procInfo.pi->hProcess);
        wprintf(L"Changing of directory or opening file not successful!\n");
        delete procInfo.pi;
        delete procInfo.si;
        delete procInfo.name;
        return;
    }
    getProcessInfos()->push_back(procInfo);
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

        return 0;
    }
    exit(0);
    return 1;
}

//chạy tiến trình trên bg hoặc fg
int runProcess(TCHAR** cmdParts, int partCount) {
    if (wcscmp(cmdParts[2], L"-f") == 0) {
        openProcessInForeGround(cmdParts[3]);     //loi ko chuyen TCHAR ve char dc
    }
    if (wcscmp(cmdParts[2], L"-b") == 0) {
        openProcessInBackGround(cmdParts[3]);
    }
    return 0;
}

// in ra các tiến trình mà mình mở
int getProcessListAll(TCHAR** cmdParts, int partCount) {
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
int runbat(TCHAR** cmdParts, int partCount) {
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
    if (infile.is_open()) {
        while (std::getline(infile, buffer)) {
            size_t count = MultiByteToWideChar(CP_UTF8, 0, buffer.c_str(), buffer.size(), NULL, 0);
            if (count + 1 > result.size()) {
                result.resize(count + 1);
            }
            MultiByteToWideChar(CP_UTF8, 0, buffer.c_str(), buffer.size(), &result[0], count);
            result[count] = '\0';
            _wsystem(&result[0]);
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

void sighandler(int signum) {

    if (hForeProcess != NULL) {
        TerminateProcess(hForeProcess, 0);
        ProcessInfo procInfo = getProcessInfos()->back();
        CloseHandle(procInfo.pi->hThread);
        CloseHandle(procInfo.pi->hProcess);
        delete procInfo.pi;
        delete procInfo.si;
        delete procInfo.name;
        getProcessInfos()->pop_back();
        hForeProcess = NULL;
    }
    //exit(1);
}
/* Lệnh ngắt bằng Ctrl C
      signal(SIGINT, sighandler);
*/

//Tìm process 
int findProcessName(TCHAR** cmdParts, int partCount) {
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
        TCHAR* str = cmdParts[2];
        size_t bufferSize = std::wcslen(str) + 1;
        TCHAR* buffer = new TCHAR[bufferSize];

        wcscpy_s(buffer, bufferSize, str);
        if (std::wcscmp(buffer, pe32.szExeFile) == 0) {
            // Nếu pe32.szExeFile trùng với tên tiến trình thì in ra
            wprintf(L"%-50s%-20d%-20d\n", pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);
        }
    } while (Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
    return 0;
}