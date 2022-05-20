#include<iostream>
#include<vector>
#include<string>
#include <windows.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "shell_functions.h"
#include "console_addon.h"
<<<<<<< HEAD
#include "lam_funcs.h"
=======
#include "linh_funcs.h"




>>>>>>> master

//Return 0 if execute command successful, otherwise return code
//Error code: 
//     1: partCount is invalid


int test(TCHAR** cmdParts, int partCount) {
    if (partCount < 2) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid" << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (std::wcscmp(cmdParts[1], L"?doc") == 0) {
        std::wcout << "test:" << std::endl;
        std::wcout << "Description:\tUsed to test TinyShell's reaction." << std::endl;
        std::wcout << "Usage:";
        setTextColor(OCEAN);
        std::wcout << "\t\ttest <characters>";
        setTextColor(WHITE);
        std::wcout << std::endl;
        std::wcout << std::endl;
    }
    else {
        std::wcout << "Test function is executed. Print message:" << std::endl;
        for (int i = 1; i < partCount; i++) {
            std::wcout << cmdParts[i] << std::endl;
        }
        std::wcout << std::endl;
    }
<<<<<<< HEAD
    else std::wcout << cmdParts << std::endl;
=======
>>>>>>> master
    return 0;
}
int proc(TCHAR** cmdParts, int partCount) {

    return 0;
}
std::vector<ShellCommand*> cmds;
ShellCommand** getAllCommands(int& cmdsCount) {
    cmdsCount = cmds.size();
    return &cmds[0];
}
void initializeCmds() {
    cmds.push_back(new ShellCommand{ L"test", &test });
    cmds.push_back(new ShellCommand{ L"path", &path });
    cmds.push_back(new ShellCommand{ L"addpath", &addpath });
    cmds.push_back(new ShellCommand{ L"cd", &cd });
    cmds.push_back(new ShellCommand{ L"dir", &dir });
    cmds.push_back(new ShellCommand{ L"time", &time });
    cmds.push_back(new ShellCommand{ L"date", &date });
    cmds.push_back(new ShellCommand{ L"help", &help });
    cmds.push_back(new ShellCommand{ L"del", &del });
}

int executeCommand(/*TCHAR* cmdLine, */TCHAR** cmdParts, int partCount) {
    if (partCount == 0) return 1;
    ShellFunction func = NULL;
    for (int i = 0; i < cmdParts.size(); i++) {
        if (std::wcscmp(cmds[i]->name, cmdParts[0]) == 0) {
            func = cmds[i]->func;
            break;
        }
    }
    if (func == NULL) {
        /*int len = std::wcslen(cmdLine);
        size_t len_s = len + 1;
        char* cmdline = new char[len + 1];
        wcstombs_s(&len_s, cmdline, len + 1, cmdLine, len + 1);
        std::wcout << L"'Converted string: " << cmdline << std::endl;
        int callR = system(cmdline);
        std::wcout << L"'system call return: " << callR << std::endl;
        delete cmdline;*/
        std::wcout << L"'";
        setTextColor(RED);
        std::wcout << cmdParts[0];
        setTextColor(WHITE);
        std::wcout << L"' is not recognized as an command or batch file." << std::endl;
        //std::wcout << "operable program or batch file." << std::endl;
        std::wcout << std::endl;
        
        return 1;
    }
    return func(cmdParts, partCount);
}


