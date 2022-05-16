﻿#include<vector>
#include<string>
#include <windows.h>
#include "shell_functions.h"
#include "console_addon.h"
typedef int (*ShellFunction)(TCHAR** cmdParts, int partCount);
typedef struct ShellCommand {
    const TCHAR* name;
    ShellFunction func;
} ShellCommand;
PROCESS_INFORMATION pi[100];
STARTUPINFO si[100];
LPSTR cString[100];
HANDLE hHandless[100];
int status[100];
int n = 0;
HANDLE hForeProcess;
void openProcessInForeGround(const string& s)
{

    PROCESS_INFORMATION pi;                 // lpStartupInfo    // lpProcessInformation
    STARTUPINFO si = { sizeof(STARTUPINFO) }; // cpp string must be modified to use in c
    LPSTR cString = strdup(s.c_str());
    ZeroMemory(&si, sizeof(si)); // fill this block with zeros
    si.cb = sizeof(si);          // CreateProcess(cString, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    if (!CreateProcess(cString,  // No module name (use command line)
        NULL,     // Command line
        NULL,     // Process handle not inheritable
        NULL,     // Thread handle not inheritable
        FALSE,    // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE,
        NULL, // Use parent's environment block
        NULL, // Use parent's starting directory
        &si,  // Pointer to STARTUPINFO structure
        &pi)  // Pointer to PROCESS_INFORMATION structure
        )
    {
        wcout<<"Changing of directory or opening file not successful!\n");
        return;
    }
    WaitForSingleObject(pi.hProcess, INFINITE); // INFINITE // hProcess: The handle is used to specify the process in all functions that perform operations on the process object.
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

void openProcessInBackGround(const string& s)
{
    void kill("proc kill "+ (string)s, 3);
    ++n;
    status[n] = 1;
    si[n] = { sizeof(STARTUPINFO) };     
    pi[n];                             
    ZeroMemory(&si[n], sizeof(si[n])); 
    si[n].cb = sizeof(si[n]);
    cString[n] = strdup(s.c_str()); 
    if (!CreateProcess(cString[n],  /
        NULL,        // Command line
        NULL,        // Process handle not inheritable
        NULL,        // Thread handle not inheritable
        FALSE,       // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE,
        NULL,   // Use parent's environment block
        NULL,   // Use parent's starting directory
        &si[n], // Pointer to STARTUPINFO structure
        &pi[n]) // Pointer to PROCESS_INFORMATION structure
        )
    {
        TerminateProcess(pi[n].hProcess, 0);
        CloseHandle(pi[n].hThread);
        CloseHandle(pi[n].hProcess);
        n--;
        wprintf"Changing of directory or opening file not successful!\n");
        return;
    }
}

int exit(TCHAR** cmdParts, int partCount) {
    char* term = (char*)malloc(64 * sizeof(char));
    if (partCount > 1) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid" << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (std::wcscmp(cmdPart[0],"exit")) {
        term = combinePath(cmdParts, 1);
        std::wcout << "ERROR: Term " << term << "is not recognized for EXIT" << endl;
        std::cout << "Command: exit" << endl;
        return 0;
    }
    return 1;
}

//chạy tiến trình trên bg hoặc fg
int runProcess(TCHAR** cmdParts, int partCount) {
    
    if (std::wcscmp(cmdParts[4], "f") == 0) {
            openProcessInForeGround(cmdParts[3]);
        }
    if (std::wcscmp(cmdParts[4], "b") == 0) {
            openProcessInBackGround(cmdParts[3]);
        }
    return 0;
}

// in ra các tiến trình đang chạy ??
int getProcessListAll(TCHAR** cmdParts,int countPart) {
    if (partCount < 2) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid" << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (std::wcscmp(cmdParts[1], L"list") == 0&& (std::wcscmp(cmdParts[0], L"proc") == 0)) {
        std::wcout << "proc:" << std::endl;
        std::wcout << "Description:\tUsed to list all processes running." << std::endl;
        std::wcout << "Usage:\t\ttest <characters>" << std::endl;
        std::wcout << std::endl;
    }
    else {
        std::wcout << "Test function is executed. Print message:" << std::endl;
        for (int i = 1; i < partCount; i++) {
            std::wcout << cmdParts[i] << std::endl;
        }
        std::wcout << std::endl;
    }
    std::wcout<<"\n";
    std::wcout<<"--------------------------------------------------------------";
    std::wcout<<"| Numbers        IdProcess             hProcess           Status         Name   \n";
    for (int i = 1; i <= n; ++i)
    {
        DWORD dwExitCode;
        GetExitCodeProcess(pi[i].hProcess, &dwExitCode);
        if (dwExitCode != 259)
        {
            TerminateProcess(pi[i].hProcess, 0);
            CloseHandle(pi[i].hThread);
            CloseHandle(pi[i].hProcess);
            for (int j = i; j < n; ++j)
            {
                status[j] = status[j + 1];
                pi[j] = pi[j + 1];
                si[j] = si[j + 1];
                cString[j] = cString[j + 1];
            }
            n--;
            i--;
        }
        else
        {
            const char* a = (status[i] == 0) ? "stopping" : "Running ";
            std::wcout<<"|   %-19d%-26d%-20p%s          %s\n", i, pi[i].dwProcessId, pi[i].hProcess, a, cString[i];
        }
    }
    std::wcout << "----------------------------------------------------------------------------";
    std::wcout << "\n";
    return 0;
}

//chạy file *bat
int runBat(TCHAR** cmdParts,int countPart)
{   
    if (partCount !=2) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid" << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (std::wcscmp(cmdParts[0], L"run") == 0) {
        std::wcout << "test:" << std::endl;
        std::wcout << "Description:\tUsed to run a bat file." << std::endl;
        std::wcout << "Usage:\t\trun " << std::endl;
        std::wcout << std::endl;
    }
    else {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "The command is invalid" << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    ifstream file(cmdParts[1]);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            run(line);
        }
    }
    else
    {
        std::wcout << "File " << cmdParts[1] << " do not exist in this directory\n";
    }
    return 0;
}
void sighandler(int signum) {
   
    if (hForeProcess != NULL) {
        TerminateProcess(hForeProcess, 0);
        hForeProcess = NULL;
    }
    exit("exit",1);
}
/* Lệnh ngắt bằng Ctrl C:(méo hiểu j)
      signal(SIGINT, sighandler);
*/

//Tìm process có name là cmdParts
int findProcessID(TCHAR** cmdParts,int countPart) {
    if (partCount > 3||partCount<3) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid." << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (partCount == 3) {
        if (std::wcscmp(cmdParts[1], L"idof") == 0&& (std::wcscmp(cmdParts[0], L"proc") == 0)) {
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
        if (std::strcmp(cmdParts[2], pe32.szExeFile) == 0) {
            // Nếu pe32.szExeFile trùng với tên tiến trình thì in ra
            wprintf(L"%-50s%-20d%-20d\n", pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);
        }
    } while (Process32Next(hProcessSnap, &pe32)); CloseHandle(hProcessSnap);
    return 0;
}
