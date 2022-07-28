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

void openProcessInBackGround(TCHAR* s)
{
    ProcessInfo procInfo;
    DWORD processId = _wtoi(s);
    //killProcessID(processId);//xóa tiến trình có id là s
    procInfo.status = PROC_STAT_RUNNING;
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
        delete[] procInfo.name;
        return;
    }
    getProcessInfos()->push_back(procInfo);
}
HANDLE foregroundThread;
int foregroundThreadMethod(size_t size) {
    ProcessInfo procInfo = getProcessInfos()->back();
    WaitForSingleObject(procInfo.pi->hProcess, INFINITE); // INFINITE // hProcess: The handle is used to specify the process in all functions that perform operations on the process object.

    if (size == getProcessInfos()->size()) {
        CloseHandle(procInfo.pi->hThread);
        CloseHandle(procInfo.pi->hProcess);
        delete procInfo.pi;
        delete procInfo.si;
        delete[] procInfo.name;
        getProcessInfos()->pop_back();
        hForeProcess = NULL;
    }
    return 1;
}
void openProcessInForeGround(TCHAR* s)
{
    ProcessInfo procInfo;
    procInfo.pi = (LPPROCESS_INFORMATION) calloc(1, sizeof(PROCESS_INFORMATION));
    procInfo.si = (LPSTARTUPINFOW) calloc(1, sizeof(STARTUPINFOW));
    procInfo.si->cb = sizeof(STARTUPINFOW);
    procInfo.name = wcsdup(s);
    procInfo.status = PROC_STAT_RUNNING;

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
        delete[] procInfo.name;
    }
    else {
        hForeProcess = procInfo.pi->hProcess;
        //getProcessInfos()->push_back(procInfo);
        if (!SetConsoleCtrlHandler(CtrlHandler, TRUE))
        {
            setTextColor(RED);
            std::wcout << L"Error: ";
            setTextColor(WHITE);
            wprintf(L"Could not set control-C and control-Break handler.\n");
        }
        else {
            wprintf(L"Use Ctrl+C to terminate process.\n");
            wprintf(L"Use Ctrl+Break to switch process run mode background.\n\n");
        }
        getProcessInfos()->push_back(procInfo);
        size_t size = getProcessInfos()->size();
        DWORD Id;
        foregroundThread = CreateThread(NULL, 1000, (LPTHREAD_START_ROUTINE)foregroundThreadMethod, &size, 0, &Id);
        WaitForSingleObject(foregroundThread, INFINITE);
        foregroundThread = NULL;
    }
}
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
    if (fdwCtrlType == CTRL_C_EVENT) {
        if (hForeProcess != NULL) {
            TerminateProcess(hForeProcess, 0);
            ProcessInfo procInfo = getProcessInfos()->back();
            CloseHandle(procInfo.pi->hThread);
            CloseHandle(procInfo.pi->hProcess);
            delete procInfo.pi;
            delete procInfo.si;
            delete[] procInfo.name;
            getProcessInfos()->pop_back();
            hForeProcess = NULL;
            std::wcout << "Ctrl + C: Foreground program is terminated." << std::endl << std::endl;
            TerminateThread(foregroundThread, 0);
            foregroundThread = NULL;
        }
    }
    if (fdwCtrlType == CTRL_BREAK_EVENT) {
        if (hForeProcess != NULL) {
            //ProcessInfo procInfo = getProcessInfos()->back();
            hForeProcess = NULL;
            std::wcout << "Ctrl + Break: Foreground program run mode is switch to background." << std::endl << std::endl;
            TerminateThread(foregroundThread, 0);
            foregroundThread = NULL;
        }
    }
    return TRUE;
    //exit(1);
}
//chạy tiến trình trên bg hoặc fg
int runProcess(TCHAR** cmdParts, int partCount) {
    if (wcscmp(cmdParts[2], L"-f") == 0) {
        openProcessInForeGround(cmdParts[3]);
        return 0;
    }
    if (wcscmp(cmdParts[2], L"-b") == 0) {
        openProcessInBackGround(cmdParts[3]);
        return 0;
    }
    setTextColor(RED);
    std::wcout << L"Error: ";
    setTextColor(WHITE);
    std::wcout << "Invalid flag ";
    setTextColor(RED);
    std::wcout << cmdParts[2];
    setTextColor(WHITE);
    std::wcout << ". Only support background flag ";
    setTextColor(OCEAN);
    std::wcout << "-b ";
    setTextColor(WHITE);
    std::wcout << "and foreground flag ";
    setTextColor(OCEAN);
    std::wcout << "-f";
    setTextColor(WHITE);
    std::wcout << "." << std::endl << std::endl;
    return 1;
}
// in ra các tiến trình mà mình mở
int getProcessListAll(TCHAR** cmdParts, int partCount) {
    TCHAR status[12];
    wprintf(L"\n");
    wprintf(L"--------------------------------------------------\n");
    wprintf(L"%11s|%12s| %s\n\n", L"Process Id", L"Status", L"Process Name");
    std::vector<ProcessInfo>* pis = getProcessInfos();
    int processCount = getProcessInfos()->size();
    for (int i = 0; i < processCount; ++i)
    {
        DWORD dwExitCode;

        GetExitCodeProcess((*pis)[i].pi->hProcess, &dwExitCode);

        if (dwExitCode != STILL_ACTIVE) {
            TerminateProcess((*pis)[i].pi->hProcess, 0);
            CloseHandle((*pis)[i].pi->hThread);
            CloseHandle((*pis)[i].pi->hProcess);
            delete (*pis)[i].pi;
            delete (*pis)[i].si;
            delete[](*pis)[i].name;
            pis->erase(pis->begin() + i);
            processCount--;
            i--;
            continue;
        }

        switch ((*pis)[i].status)
        {
        case 1://PROC_STAT_RUNNING
            wcscpy_s(status, 12, L"Running");
            break;
        case 2://PROC_STAT_STOP
            wcscpy_s(status, 12, L"Paused");
            break;
        case 3://PROC_STAT_TERMINATED
            wcscpy_s(status, 12, L"Terminated");
            break;
        }
        wprintf(L"%11d %12s  %s\n", (*pis)[i].pi->dwProcessId, status, (*pis)[i].name);
        /*DWORD dwExitCode;

        GetExitCodeProcess((*pis)[i].pi->hProcess, &dwExitCode);*/
        /*if (dwExitCode != 259)
        {
            TerminateProcess(pi[i]->hProcess, 0);
            CloseHandle(pi[i]->hThread);
            CloseHandle(pi[i]->hProcess);
            for (int j = i; j < processCount; ++j)
            {
                status[j] = status[j + 1];
                pi[j] = pi[j + 1];
                si[j] = si[j + 1];
                cString[j] = cString[j + 1];
            }
            processCount--;
            i--;
        }
        else
        {
            const char* a = (status[i] == 0) ? "STOP" : "RUNNING ";
            wprintf(L"|   %-19d%-26d%-20p%s          %s\n", i, pi[i]->dwProcessId, pi[i]->hProcess, a, cString[i]);
        }*/
    }
    wprintf(L"\n");
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
            int count = MultiByteToWideChar(CP_UTF8, 0, buffer.c_str(), buffer.size(), NULL, 0);
            if (count + 1 > result.size()) {
                result.resize(count + 1);
            }
            MultiByteToWideChar(CP_UTF8, 0, buffer.c_str(), buffer.size(), &result[0], count);
            result[count] = '\0';
            
            setTextColor(WHEATWHITE);
            std::wcout << &result[0] << std::endl;
            setTextColor(WHITE);
            int l;
            TCHAR** localCmdParts = strSplit((TCHAR*)&result[0], l);
            int exitCode = executeCommand(/*(TCHAR*)(*cmdStr).c_str(), */localCmdParts, l);
            
            //Free memory
            for (int i = 0; i < l; i++) {
                delete[] localCmdParts[i];
            }
            delete[] localCmdParts;
            if (exitCode && l) {
                std::wcout << std::endl;
                setTextColor(RED);
                std::wcout << L"Command Error Occurs: Stop executing bat file." << std::endl;
                setTextColor(WHITE);
                std::wcout << std::endl;
                return 1;
            }
        }
        std::wcout << std::endl;
        return 0;
    }
    else {
        setTextColor(RED);
        std::wcout << L"Unable to open file: " << cmdParts[1] << std::endl;
        setTextColor(WHITE);
        return 1;
    }
}

int exit(TCHAR** cmdParts, int partCount) {
    if (partCount == 2 && std::wcscmp(cmdParts[1], L"?doc") == 0) {
        setTextColor(RED);
        std::wcout << "Command: exit" << std::endl;

        return 0;
    }
    if (partCount > 1) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid" << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    exit(0);
    return 1;
}
//tìm process theo status
int procStat(TCHAR** cmdParts, int partCount) {
    TCHAR status[12];
    int findStatus = 0;
    if (std::wcscmp(cmdParts[2], L"running") == 0 || std::wcscmp(cmdParts[2], L"1") == 0) {
        findStatus = 1;
    }
    else if (std::wcscmp(cmdParts[2], L"paused") == 0 || std::wcscmp(cmdParts[2], L"2") == 0) {
        findStatus = 2;
    }
    
    wprintf(L"\n");
    wprintf(L"--------------------------------------------------\n");
    wprintf(L"%11s|%12s| %s\n\n", L"Process Id", L"Status", L"Process Name");
    std::vector<ProcessInfo>* pis = getProcessInfos();
    int processCount = getProcessInfos()->size();
    for (int i = 0; i < processCount; ++i)
    {
        if ((*pis)[i].status == findStatus) {
            switch ((*pis)[i].status)
            {
            case 1://PROC_STAT_RUNNING
                wcscpy_s(status, 12, L"Running");
                break;
            case 2://PROC_STAT_STOP
                wcscpy_s(status, 12, L"Paused");
                break;
            case 3://PROC_STAT_TERMINATED
                wcscpy_s(status, 12, L"Terminated");
                break;
            }
            wprintf(L"%11d %12s  %s\n", (*pis)[i].pi->dwProcessId, status, (*pis)[i].name);
        }

    }
    std::wcout << std::endl;
    return 0;
}
//Tìm process 
int idofProcess(TCHAR** cmdParts, int partCount) {
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
    
    TCHAR* str = cmdParts[2];
    size_t bufferSize = std::wcslen(str) + 1;
    TCHAR* buffer = new TCHAR[bufferSize];
    bool hasResult = false;
    wcscpy_s(buffer, bufferSize, str);
    do {
        
        if (std::wcscmp(buffer, pe32.szExeFile) == 0) {
            if (!hasResult) {
                wprintf(L"----------------------------------------\n");
                wprintf(L"%12s%20s%s\n\n", L"Process ID|", L"Parent Process ID|", L" Process Name");
            }
            // Nếu pe32.szExeFile trùng với tên tiến trình thì in ra
            wprintf(L"%11d %19d  %s\n", pe32.th32ProcessID, pe32.th32ParentProcessID, pe32.szExeFile);
            hasResult = true;
            /*CloseHandle(hProcessSnap);
            delete[] buffer;
            return 0;*/
        }
    } while (Process32Next(hProcessSnap, &pe32));
    if (!hasResult) {
        std::wcout << "There aren't any processes whose name is ";
        setTextColor(OCEAN);
        std::wcout << buffer;
        setTextColor(WHITE);
        std::wcout << "." << std::endl;
   }
    std::wcout << std::endl;
    CloseHandle(hProcessSnap);
    delete[] buffer;
    return 1;
}