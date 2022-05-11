#include<iostream>
#include<vector>
#include<string>
#include <windows.h>
#include "shell_functions.h"

//Return 0 if execute command successful, otherwise return code
//Error code: 
//     1: partCount is invalid
typedef int (*ShellFunction)(TCHAR** cmdParts, int partCount);
typedef struct ShellCommand {
    const TCHAR* name;
    ShellFunction func;
} ShellCommand;

int test(TCHAR** cmdParts, int partCount) {
    if (partCount < 2) return 1;
    if (std::wcscmp(cmdParts[1], L"help") == 0) {
        std::wcout << "test:" << std::endl;
        std::wcout << "Description:\tUsed to test TinyShell's reaction." << std::endl;
        std::wcout << "Usage:\t\ttest <characters>" << std::endl;
        std::wcout << std::endl;
    }
    else std::wcout << cmdParts[1] << std::endl;
    return 0;
}

int cmdSupportCount = 1;
std::vector<ShellCommand*> cmds;
void initializeCmds() {
    cmds.push_back(new ShellCommand{ L"test", &test });
}

TCHAR** strSplit(TCHAR* str, int& length) {
    //std::wcout << str << std::endl;
    std::vector<TCHAR*> parts;
    int l = 0; //have space before
    /*if (str[0] != ' ' && str[0] != '\t') {
        parts.push_back(str);
    }*/
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == ' ' || str[i] == '\t') {
            if (l == -1) {
                str[i] = '\0';
                l = 0;
            }
        }
        else {
            if (l != -1) {
                parts.push_back(str + i);
                l = -1;
            }
        }
        i++;
    }
    length = parts.size();
    TCHAR** output = (TCHAR**)malloc(length * sizeof(TCHAR*));
    for (int i = 0; i < length; i++) {
        output[i] = parts[i];
    }
    return output;
}
int executeCommand(TCHAR** cmdParts, int partCount) {
    if (partCount == 0) return 1;
    ShellFunction func = NULL;
    for (int i = 0; i < cmds.size(); i++) {
        if (std::wcscmp(cmds[i]->name, cmdParts[0]) == 0) {
            func = cmds[i]->func;
            break;
        }
    }
    if (func == NULL) {
        std::wcout << L"'" << cmdParts[0] << L"' is not recognized as an internal or external command," << std::endl;
        std::wcout << "operable program or batch file." << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    return func(cmdParts, partCount);
}
