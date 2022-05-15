#include<vector>
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
//void kill(string s)
//{
//    
//    int id = atoi(s.c_str());
//    int a = 1;
//    for (int i = 1; i <= n; ++i)
//    {
//        if (pi[i].dwProcessId == id)
//        {
//            TerminateProcess(pi[i].hProcess, 0);
//            CloseHandle(pi[i].hThread);
//            CloseHandle(pi[i].hProcess);
//            // cout << pi[i].hProcess << endl;
//            std::wout<<"Process %s killed\n"<<cString[i];
//            for (int j = i; j < n; ++j)
//            {
//                status[j] = status[j + 1];
//                pi[j] = pi[j + 1];
//                si[j] = si[j + 1];
//                cString[j] = cString[j + 1];
//            }
//            n--;
//            a = 0;
//            break;
//        }
//    }
//    if (a)
//    {
//        printf("Can't find process with this id = %d\n", id);
//    }
//}
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
        printf("Changing of directory or opening file not successful!\n");
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
    si[n] = { sizeof(STARTUPINFO) };     // lpStartupInfo // lpProcessInformation
    pi[n];                             // cpp string must be modified to use in c
    ZeroMemory(&si[n], sizeof(si[n])); // fill this block with zeros
    si[n].cb = sizeof(si[n]);
    cString[n] = strdup(s.c_str()); // CreateProcess(cString, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    if (!CreateProcess(cString[n],  // No module name (use command line)
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
        printf("Changing of directory or opening file not successful!\n");
        return;
    }
}

int exit(TCHAR** cmdParts, int partCount) {
    char* term = (char*)malloc(64 * sizeof(char));
    if (cmdParts[1] != NULL) {
        term = combinePath(cmdParts, 1);
        cout << "ERROR: Term " << term << "is not recognized for EXIT" << endl;
        cout << "Command: exit" << endl;
        return 0;
    }
    return 1;
}
int runProcess(TCHAR** cmdParts, int partCount) {
    if (partCount < 4) return 1;
    if ((std::wcscmp(cmdParts[1], L"run") == 0) && (std::wcscmp(cmdParts[0], L"proc") == 0) {
        std::wcout << "runProcess:" << std::endl;
            std::wcout << "Description:\tUsed to test TinyShell's reaction." << std::endl;
            std::wcout << "Usage:\t\tproc run <characters>" << std::endl;
            std::wcout << std::endl;
    }
    else std::wcout << cmdParts << std::endl;
    if (std::wcscmp(cmdParts[4], "f") == 0) {
            openProcessInForeGround(cmdParts[3]);
        }
    if (std::wcscmp(cmdParts[4], "b") == 0) {
            openProcessInBackGround(cmdParts[3]);
        }
    return 0;
}
int getProcessListAll(TCHAR** cmdParts,int countPart) {
    if (partCount < 2) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid" << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (std::wcscmp(cmdParts[1], L"list") == 0) {
        std::wcout << "test:" << std::endl;
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
    printf("\n");
    printf("--------------------------------------------------------------");
    printf("| Numbers        IdProcess             hProcess           Status         Name   \n");
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
            printf("|   %-19d%-26d%-20p%s          %s\n", i, pi[i].dwProcessId, pi[i].hProcess, a, cString[i]);
        }
    }
    printf("----------------------------------------------------------------------------");
    printf("\n");
    return 0;
}
int runBat(TCHAR** cmdParts,int countPart)
{   
    if (partCount < 2) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid" << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (std::wcscmp(cmdParts[1], L"doc") == 0) {
        std::wcout << "test:" << std::endl;
        std::wcout << "Description:\tUsed to run a bat file." << std::endl;
        std::wcout << "Usage:\t\trun <characters>" << std::endl;
        std::wcout << std::endl;
    }
    else {
        std::wcout << "Test function is executed. Print message:" << std::endl;
        for (int i = 1; i < partCount; i++) {
            std::wcout << cmdParts[i] << std::endl;
        }
        std::wcout << std::endl;
    }
    ifstream file(cmdParts[2]);
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
        cout << "File " << cmdParts[2] << " do not exist in this directory\n";
    }
    return 0;
}
void sighandler(int signum) {
   
    if (hForeProcess != NULL) {
        TerminateProcess(hForeProcess, 0);
        hForeProcess = NULL;
    }
    exit(1);
}

//Kill process

int killProcessID(TCHAR** cmdParts,int countPart) {
    if (partCount < 2) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid" << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (std::wcscmp(cmdParts[1], L"kill") == 0) {
        std::wcout << "test:" << std::endl;
        std::wcout << "Description:\tUsed to test TinyShell's reaction." << std::endl;
        std::wcout << "Usage:\t\tkillProcessID <characters>" << std::endl;
        std::wcout << std::endl;
    }
    else {
        std::wcout << "Test function is executed. Print message:" << std::endl;
        for (int i = 1; i < partCount; i++) {
            std::wcout << cmdParts[i] << std::endl;
        }
        std::wcout << std::endl;
    }
    DWORD process_id = cmdPart[2];
    // Mở tiến trình đang chạy có Process ID là...
    HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    // Nếu hProcess trả về NULL thì báo lỗi
    if (hprocess == NULL) {
        cout << "ERROR: Failed!" << endl;
        return 1;
    }
    // Đóng tiến trình hProcess
    if (!TerminateProcess(hprocess, 0)) {
        return 0;
    }
    return 1;
}
