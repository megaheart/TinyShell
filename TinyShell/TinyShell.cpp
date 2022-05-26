﻿// TinyShell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma region Header
#include <windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <io.h>
#include <fcntl.h>
#include <iomanip>  
#include "shell_functions.h"
#include "console_addon.h"
#include "lam_funcs.h"
#include <csignal>
#pragma endregion




void initialize() {
    setTextColor(WHITE);
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    std::wstring ePath = executablePath();
    SetCurrentDirectory(ePath.c_str());
}
int main()
{
    initialize();
    initializeCmds();
    
    std::wcout << L"TinyShell [Version 1.0.0]\nDeveloped by XXX, YYYY\n\n";
    while (1) {
        std::wcout << currentPath() << L"> ";
        std::wstring* cmdStr = new std::wstring;
        //fflush(stdin);
        /*while (std::wcin.eof()) {
            
        }*/
        setTextColor(WHEATWHITE);
        getline(std::wcin, *cmdStr);
        setTextColor(WHITE);
        
        /*std::wcin.clear();
        for (int i = 0; i < cmdStr->size(); i++) {
            std::wcout << std::setw(4) << (*cmdStr)[i];
        }
        std::wcout << std::endl;
        for (int i = 0; i < cmdStr->size(); i++) {
            std::wcout << std::setw(4) << i;
        }
        std::wcout << std::endl;*/
        int l;
        TCHAR **cmdParts = strSplit((TCHAR *)(*cmdStr).c_str(), l);
        executeCommand(/*(TCHAR*)(*cmdStr).c_str(), */cmdParts, l);
        //Free memory
        delete cmdStr;
        for (int i = 0; i < l; i++) {
            delete cmdParts[i];
        }
        delete cmdParts;
    }
    
}

