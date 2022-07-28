#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include <windows.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include "console_addon.h"
#include "shell_functions.h"

int path(TCHAR** cmdParts, int partCount) {
    if (partCount > 2) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid." << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (partCount == 1) {
        setTextColor(OCEAN);
        std::wcout << L"PATH";
        setTextColor(WHITE);
        setTextColor(GRAY);
        std::wcout << L"=";
        setTextColor(WHITE);
        /*if (const char* env_p = std::getenv("PATH"))
            std::cout << "Your PATH is: " << env_p << '\n';*/
        DWORD nSize = 32767 + 1;
        TCHAR* lpBuffer = new TCHAR[nSize];
        int size = GetEnvironmentVariable(L"PATH", lpBuffer, nSize);
        lpBuffer[size] = L'\0';
        std::wcout << lpBuffer << std::endl;
        std::wcout << std::endl;
        delete[] lpBuffer;
        return 0;
    }
    if (std::wcscmp(cmdParts[1], L"?doc") == 0) {
        std::wcout << "path:" << std::endl;
        std::wcout << "Description:\tUsed to get or set environment path." << std::endl;
        std::wcout << "Usage:";
        setTextColor(OCEAN);
        std::wcout << "\t\tpath                   ";
        setTextColor(WHITE);
        std::wcout << " : To get environment path." << std::endl;
        setTextColor(OCEAN);
        std::wcout << "\t\tpath <environment path>";
        setTextColor(WHITE);
        std::wcout << " : To set environment path." << std::endl;
        std::wcout << std::endl;
    }
    else {
        DWORD nSize = 32767 + 1;
        TCHAR* lpBuffer = cmdParts[1];
        SetEnvironmentVariable(L"PATH", lpBuffer);
        std::wcout << "Environment path was changed.\n\n";
        setTextColor(OCEAN);
        std::wcout << L"PATH";
        setTextColor(WHITE);
        setTextColor(GRAY);
        std::wcout << L"=";
        setTextColor(WHITE);
        int size = GetEnvironmentVariable(L"PATH", lpBuffer, nSize);
        lpBuffer[size] = L'\0';
        std::wcout << lpBuffer << std::endl;
        std::wcout << std::endl;
        //delete lpBuffer;
    }
    return 0;
}

int addpath(TCHAR** cmdParts, int partCount) {
    if (partCount == 1) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "<environment path> parameter is required." << std::endl;
        std::wcout << std::endl;
        return 1;
        return 1;
    }
    else if (partCount > 2) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid." << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (std::wcscmp(cmdParts[1], L"?doc") == 0) {
        std::wcout << "addpath:" << std::endl;
        std::wcout << "Description:\tUsed to add environment path." << std::endl;
        std::wcout << "Usage:\t\t";
        setTextColor(OCEAN);
        std::wcout << "addpath <environment path>" << std::endl;
        setTextColor(WHITE);
        std::wcout << std::endl;
        return 0;
    }

    DWORD nSize = 32767 + 1;
    TCHAR* lpBuffer = new TCHAR[nSize];
    int size = GetEnvironmentVariable(L"PATH", lpBuffer, nSize);
    lpBuffer[size++] = ';';
    int i = 0;
    while (cmdParts[1][i] != '\0') {
        lpBuffer[size + i] = cmdParts[1][i];
        i++;
    }
    lpBuffer[size + i] = '\0';
    SetEnvironmentVariable(L"PATH", lpBuffer);

    //Print to console
    std::wcout << "Environment path was changed.\n\n";
    setTextColor(OCEAN);
    std::wcout << L"PATH";
    setTextColor(WHITE);
    setTextColor(GRAY);
    std::wcout << L"=";
    setTextColor(WHITE);
    size = GetEnvironmentVariable(L"PATH", lpBuffer, nSize);
    lpBuffer[size] = L'\0';
    std::wcout << lpBuffer << std::endl;
    std::wcout << std::endl;
    delete[] lpBuffer;
    return 0;
}

int cd(TCHAR** cmdParts, int partCount) {
    std::wstring curPath = currentPath();
    if (partCount == 1) {
        std::wcout << curPath << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (std::wcscmp(cmdParts[1], L"?doc") == 0) {
        std::wcout << "cd:" << std::endl;
        std::wcout << "Description:\tUsed to change current directory." << std::endl;
        std::wcout << "Usage:\t\t";
        setTextColor(OCEAN);
        std::wcout << "cd <path>" << std::endl;
        setTextColor(WHITE);
        std::wcout << std::endl;
        return 0;
    }
    //Recogniting dot (cd......)
    //int index = 0;
    //while (cmdParts[1][index] != '\0' && cmdParts[1][index] == '.') index++;
    //if (index > 0 && index == std::wcslen(cmdParts[1])) {
    //    int i = curPath.length() - 1;
    //    while (i > -1 && index > 0) {
    //        while (i > -1 && curPath[i] != '\\') {
    //            curPath.pop_back();
    //            i--;
    //        }
    //        if (i > -1)
    //        {
    //            curPath.pop_back();
    //            i--;
    //        }
    //        index--;
    //    }
    //    if (i == -1) {
    //        setTextColor(RED);
    //        std::wcout << L"Error: ";
    //        setTextColor(WHITE);
    //        std::wcout << "Too many dot." << std::endl;
    //        std::wcout << std::endl;
    //        return 1;
    //    }
    //    if (SetCurrentDirectory((TCHAR*)curPath.c_str())) {
    //        std::wcout << std::endl;
    //        return 0;
    //    }
    //    else {
    //        setTextColor(RED);
    //        std::wcout << L"Error: ";
    //        setTextColor(WHITE);
    //        std::wcout << L"Directory \"";
    //        setTextColor(OCEAN);
    //        std::wcout << curPath;
    //        setTextColor(WHITE);
    //        std::wcout << L"\" doesn't exist." << std::endl;
    //        std::wcout << std::endl;
    //        return 1;
    //    }
    //    return 0;
    //}
    ////cd <path>
    //index = 0;
    //while (cmdParts[1][index] != '\0' && cmdParts[1][index] != ':') index++;
    //TCHAR* path;
    //if (cmdParts[1][index] == ':') {
    //    //absolutely path
    //    path = cmdParts[1];
    //}
    //else {
    //    //relative path
    //    curPath += '\\';
    //    curPath.append(cmdParts[1]);
    //    path = (TCHAR*)curPath.c_str();
    //}
    TCHAR* path = cmdParts[1];
    if (SetCurrentDirectory(path)) {
        std::wcout /*<< L"curPath: " << curPath*/ << std::endl;
    }
    else {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << L"Directory ";
        std::wcout << L"doesn't exist." << std::endl; 
        std::wcout << std::endl;
        return 1;
    }
    return 0;
}
std::wstring time_to_string(std::filesystem::file_time_type const& ftime) {
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
    std::time_t tt = std::chrono::system_clock::to_time_t(sctp);

    struct tm lt;
    localtime_s(&lt, &tt);
    //char mbstr[20];
    TCHAR cstr[20];
    if (size_t size = std::wcsftime(cstr, sizeof(cstr), L"%H:%M %d-%m-%Y", &lt)) {
        return std::wstring(cstr);
    }
    /*std::stringstream buffer;
    buffer << std::put_time(&lt, "%H:%M %d-%m-%Y");
    std::string s = buffer.str();
    size_t size = s.length() + 1;
    size_t nSize;
    mbstowcs_s(&nSize, cstr, size, s.c_str(), size);*/

    return std::wstring();
}
std::wstring readableFileSize(uintmax_t size)
{
    std::wstringstream output;
    // Set Fixed -Point Notation
    output << std::fixed;
    // Set precision to 2 digits
    output << std::setprecision(2);
    int i;
    double mantissa = size;
    for (i = 0; mantissa >= 1024.0; ++i) {
        mantissa /= 1024.0;
    }
    mantissa = std::ceil(mantissa * 100.) / 100.;
    output << mantissa;
    /*int index = output.find_last_of('.');
    if (index > -1) {
        index = output.length() - 1;
        while (output[index] == '0')
        {
            output.pop_back();
            index--;
        }
    }*/
    output << ' ';
    output << L"BKMGTPE"[i];
    if (i > 0) {
        output << 'B';
    }
    else output << ' ';
    return output.str();
}
std::wstring readableNumber(uintmax_t size) {
    std::wstring num = std::to_wstring(size);
    for (int i = num.length() - 3; i > 0; i -= 3) {
        num.insert(num.begin() + i, L',');
    }
    return num;
}
int displayDir(std::wstring path) {
    std::filesystem::directory_iterator dirIterator;
    try {
        dirIterator = std::filesystem::directory_iterator(path);
    }
    catch (std::filesystem::filesystem_error e) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "Directory does NOT exist." << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    std::wcout << std::setw(12) << "Type ";
    std::wcout << std::setw(18) << "Last write time" << " ";
    std::wcout << std::setw(11) << "Size ";
    std::wcout << "   File name" << std::endl;
    std::wcout << L"----------------------------------------------------------------" << std::endl;
    uintmax_t folderCount, fileCount, fileTotalSize;
    folderCount = fileCount = fileTotalSize = 0;
    for (const auto& entry : dirIterator)
    {
        std::filesystem::path p(entry.path());
        if (entry.is_directory()) {
            std::wcout << std::setw(12) << "<FORDER> ";
            std::wcout << std::setw(18) << time_to_string(entry.last_write_time()) << " ";
            std::wcout << std::setw(11) << " ";
            folderCount++;
        }
        else {
            std::wcout << std::setw(12) << "<FILE> ";
            std::wcout << std::setw(18) << time_to_string(entry.last_write_time()) << " ";
            std::wcout << std::setw(10) << readableFileSize(entry.file_size()) << " ";
            fileTotalSize += entry.file_size();
            fileCount++;
        }
        std::wcout << p.filename().wstring() << std::endl;
    }
    std::wcout << std::endl;
    std::wcout << std::setw(20) << readableNumber(folderCount) << " Folder(s) " << std::endl;
    std::wcout << std::setw(20) << readableNumber(fileCount)   << " File(s)" << std::setw(13) << readableFileSize(fileTotalSize) << std::endl;
    std::wcout << std::endl; 
    return 0;
    /*std::wcout << "-------------------------------------------------------------" << std::endl;
    std::wcout << std::endl;
    std::wcout << std::endl;
    
    WIN32_FIND_DATAW data;
    _FILETIME time_lastwrite;
    _SYSTEMTIME convert_time;
    std::wstring time;
    
    HANDLE han = FindFirstFileW(path.c_str(), &data);
    FindClose(han);
    if (han != INVALID_HANDLE_VALUE) {
        do {
            time_lastwrite = data.ftLastWriteTime;
            FileTimeToSystemTime(&time_lastwrite, &convert_time);

            time = std::to_wstring(convert_time.wHour) + L':' + std::to_wstring(convert_time.wMinute) +
                L' ' + std::to_wstring(convert_time.wDay) + L'/' + std::to_wstring(convert_time.wMonth) + L'/' + std::to_wstring(convert_time.wYear);
            

            if (data.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE) {
                wprintf(L"%-30s%-15s%s\n", time.c_str(), L"<FILE> ", data.cFileName);
            }
            if (data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
                wprintf(L"%-30s%-15s%s\n", time.c_str(), L"<FOLDER> ", data.cFileName);
            }
            else
            {
                wprintf(L"%-30s%-15s%s\n", time.c_str(), L"<Unknown> ", data.cFileName);
            }
        } while (FindNextFileW(han, &data));
        FindClose(han);
        return 0;
    }
    else {
        return 1;
    }*/
}
int dir(TCHAR** cmdParts, int partCount) {
    if (partCount > 2) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid." << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    else if (partCount == 2) {
        if (std::wcscmp(cmdParts[1], L"?doc") == 0) {
            std::wcout << "dir:" << std::endl;
            std::wcout << "Description:\tShow a list of folders and files in the directory." << std::endl;
            std::wcout << "Usage:";
            setTextColor(OCEAN);
            std::wcout << "\t\tdir       ";
            setTextColor(WHITE);
            std::wcout << " : Show a list of folders and files in current directory." << std::endl;
            setTextColor(OCEAN);
            std::wcout << "\t\tdir <path>";
            setTextColor(WHITE);
            std::wcout << " : Show a list of folders and files in <path>." << std::endl;
            std::wcout << std::endl;
            return 0;
        }
        std::wstring curPath = currentPath();
        int index = 0;
        while (cmdParts[1][index] != '\0' && cmdParts[1][index] != ':') index++;
        TCHAR* path;
        if (cmdParts[1][index] == ':') {
            //absolutely path
            path = cmdParts[1];
        }
        else {
            //relative path
            curPath += '\\';
            curPath.append(cmdParts[1]);
            path = (TCHAR*)curPath.c_str();
        }
        return displayDir(path);
    }
    else {
        std::wstring curPath = currentPath();
        return displayDir(curPath);
    }
    
}

int time(TCHAR** cmdParts, int partCount) {
    if (partCount > 2) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid." << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (partCount == 2) {
        if (std::wcscmp(cmdParts[1], L"?doc") == 0) {
            std::wcout << "time:" << std::endl;
            std::wcout << "Description:\tShow current time." << std::endl;
            std::wcout << "Usage:";
            setTextColor(OCEAN);
            std::wcout << "\t\ttime";
            setTextColor(WHITE);
            std::wcout << std::endl;
            std::wcout << std::endl;
            return 0;
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
    std::wcout << "Current time: ";
    std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm lt;
    localtime_s(&lt, &tt);
    TCHAR cstr[20];
    if (size_t size = std::wcsftime(cstr, sizeof(cstr), L"%H:%M", &lt)) {
        std::wcout << cstr << std::endl << std::endl;
        return 0;
    }
    return 3;
}

int date(TCHAR** cmdParts, int partCount) {
    if (partCount > 2) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid." << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (partCount == 2) {
        if (std::wcscmp(cmdParts[1], L"?doc") == 0) {
            std::wcout << "date:" << std::endl;
            std::wcout << "Description:\tShow current date." << std::endl;
            std::wcout << "Usage:";
            setTextColor(OCEAN);
            std::wcout << "\t\tdate";
            setTextColor(WHITE);
            std::wcout << std::endl;
            std::wcout << std::endl;
            return 0;
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
    std::wcout << "Current date: ";
    std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm lt;
    localtime_s(&lt, &tt);
    TCHAR cstr[20];
    if (size_t size = std::wcsftime(cstr, sizeof(cstr), L"%d-%m-%Y", &lt)) {
        std::wcout << cstr << std::endl << std::endl;
        return 0;
    }
    return 3;
}

int help(TCHAR** cmdParts, int partCount) {
    if (partCount > 2) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid." << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (partCount == 2) {
        if (std::wcscmp(L"help", cmdParts[1])) {
            size_t count;
            ShellCommand** cmds = getAllCommands(count);
            ShellFunction func = NULL;
            for (int i = 0; i < count; i++) {
                if (std::wcscmp(cmds[i]->name, cmdParts[1]) == 0) {
                    func = cmds[i]->func;
                    break;
                }
            }
            if (func == NULL) {
                std::wcout << L"'";
                setTextColor(RED);
                std::wcout << cmdParts[1];
                setTextColor(WHITE);
                std::wcout << L"' is not recognized as an command." << std::endl;
                std::wcout << std::endl;

                return 1;
            }
            TCHAR* intermediary[2];
            intermediary[0] = cmdParts[1];
            intermediary[1] = new TCHAR[5]{ L"?doc" };
            int error = func(intermediary, 2);
            delete intermediary[1];
            return error;
        }
    }
    std::wcout << "help:" << std::endl;
    std::wcout << "Description:\tShow information about a specific command." << std::endl;
    std::wcout << "Usage:";
    setTextColor(OCEAN);
    std::wcout << "\t\thelp <command>";
    setTextColor(WHITE);
    std::wcout << std::endl;

    std::wcout << "Suppoted commands: ";
    size_t count;
    ShellCommand** cmds = getAllCommands(count);
    std::wcout << cmds[0]->name;
    for (int i = 1; i < count; i++) {
        std::wcout << ", " << cmds[i]->name;
    }

    std::wcout << std::endl;
    std::wcout << std::endl;
    
    return 0;
}
bool fileExist(const TCHAR* name) {
    std::ifstream f(name);
    return f.good();
}
int del(TCHAR** cmdParts, int partCount) {
    if (partCount > 2) {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "number of parameters is invalid." << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    if (partCount == 1 || std::wcscmp(L"?doc", cmdParts[1]) == 0) {
        /*if (fileExist(L"?doc")) {
            std::wcout << "Type ";
            setTextColor(BLUE);
            std::wcout << L"Y";
            setTextColor(WHITE);
            std::wcout << " to delete file ";
            setTextColor(OCEAN);
            std::wcout << L"?doc";
            setTextColor(WHITE);
            std::wcout << ", type ";
            setTextColor(GREEN);
            std::wcout << L"N";
            setTextColor(WHITE);
            std::wcout << " to show information about this command: ";
            std::wstring ans;
            getline(std::wcin, ans);
            if (ans.size() > 0 && ans[0] == 'Y') {
                if (_wremove(cmdParts[1]) != 0)
                {
                    setTextColor(RED);
                    std::wcout << L"Error: ";
                    setTextColor(WHITE);
                    std::wcout << "File is not deleted." << std::endl;
                    std::wcout << std::endl;
                    return 3;
                }
                else
                {
                    std::wcout << "File is deleted." << std::endl;
                    std::wcout << std::endl;
                    return 0;
                }
            }
        }*/
        std::wcout << "del:" << std::endl;
        std::wcout << "Description:\tDelete a file which locates at <path>." << std::endl;
        std::wcout << "Usage:";
        setTextColor(OCEAN);
        std::wcout << "\t\tdel <path>";
        setTextColor(WHITE);
        std::wcout << std::endl;
        std::wcout << std::endl;
        return 0;
    }
    if (_wremove(cmdParts[1]) != 0)
    {
        setTextColor(RED);
        std::wcout << L"Error: ";
        setTextColor(WHITE);
        std::wcout << "File is not deleted." << std::endl;
        std::wcout << std::endl;
        return 3;
    }
    else
    {
        std::wcout << "File is deleted." << std::endl;
        std::wcout << std::endl;
        return 0;
    }
}
int openFile(TCHAR** cmdParts, int partCount) {
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
    if (fileExist(cmdParts[1])) {
        int i = wcslen(L"explorer.exe ");
        int len = wcslen(cmdParts[1]) + i;
        TCHAR* cmd = new TCHAR[len + 1];
        wcscpy_s(cmd, 14, L"explorer.exe ");
        for (int j = 0; i < len; i++, j++) {
            cmd[i] = cmdParts[1][j];
        }
        cmd[i] = L'\0';
        _wsystem(cmd);
        delete[] cmd;
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