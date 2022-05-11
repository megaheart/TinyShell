// TinyShell.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
#include "shell_functions.h"
#pragma endregion



std::wstring currentPath() {
    TCHAR buffer[MAX_PATH] = { 0 };
    GetCurrentDirectory(MAX_PATH, buffer);
    return std::wstring(buffer);
}

std::wstring executablePath() {
    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return std::wstring(buffer).substr(0, pos);
}
//void openTextFile(char* fileName) {
//    std::wifstream myfile(fileName);
//
//    if (myfile.is_open())
//    {
//        std::wstring line;
//        myfile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
//        while (getline(myfile, line))
//        {
//            std::wcout << line << L'\n';
//        }
//        myfile.close();
//        /*std::wstringstream wss;
//        wss << myfile.rdbuf();
//        std::wcout << wss.str() << std::endl;
//        myfile.close();*/
//    }
//
//    else std::wcout << L"Unable to open file: " << fileName << std::endl;
//}
void initialize() {
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
        getline(std::wcin, *cmdStr);
        std::wcin.clear();
        int l;
        TCHAR **cmdParts = strSplit((TCHAR *)(*cmdStr).c_str(), l);
        executeCommand(cmdParts, l);

        //Free memory
        delete cmdStr;
        /*for (int i = 0; i < l; i++) {
            delete cmdParts[i];
        }*/
        delete cmdParts;
    }
    
}

