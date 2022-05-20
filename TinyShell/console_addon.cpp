#include <windows.h>
#include <string>
#include<vector>
#include "console_addon.h"


HANDLE getHConsole() {
    return hConsole;
}
void setTextColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}
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
TCHAR* copyAndTrim(TCHAR* str, int start, int end) {
    if (start < 0 || end < start) return NULL;
    while (start <= end && (str[start] == ' ' || str[start] == '\t')) start++;
    while (start <= end && (str[end] == ' ' || str[end] == '\t')) end++;
    if (start > end) return NULL;

    TCHAR* s = (TCHAR*)malloc((end - start + 2) * sizeof(TCHAR));
    int i;
    for (i = 0; start + i <= end; i++) {
        s[i] = str[start + i];
    }
    s[i] = '\0';
    return s;
}
TCHAR** strSplit(TCHAR* str, int& length) {
    //std::wcout << str << std::endl;
    std::vector<TCHAR*> parts;
    int l = 0; //0: have space before, -1: no space before, 2: char in "..."

    int i = 0;
    int start = -1;
    TCHAR* part;
    //Read command
    int dotStart = -1;
    int dotEnd = -1;
    while (str[i] != '\0') {
        if (str[i] == ' ' || str[i] == '\t') {
            if (l == -1) {
                l == 0;
                break;
            }
            
        }
        else if (l == 0) {
            start = i;
            l = -1;
        }
        if (str[i] == '.') {
            if (dotStart == -1) {
                dotStart = i;
                while (str[++i] == '.');
                dotEnd = --i;
            }
        }
        i++;
    }
    if (dotEnd == i - 1 && dotStart > 0 && str[dotStart - 1] != ' ' && str[dotStart - 1] != '\t') {
        part = copyAndTrim(str, start, dotStart - 1);
        if (part != NULL) {
            parts.push_back(part);
        }
        part = copyAndTrim(str, dotStart, dotEnd);
        if (part != NULL) {
            parts.push_back(part);
        }
        start = -1;
    }
    else {
        part = copyAndTrim(str, start, i - 1);
        if (part != NULL) {
            parts.push_back(part);
        }
        start = -1;
    }
    //Read parameter
    while (str[i] != '\0') {
        if (str[i] == ' ' || str[i] == '\t') {
            if (l == -1) {
                //str[i] = '\0';
                part = copyAndTrim(str, start, i - 1);
                if (part != NULL) {
                    parts.push_back(part);
                }
                start = -1;
                l = 0;
            }
        }
        //else if (str[i] == '.' && parts.size() == 0) {
        //    part = copyAndTrim(str, start, i - 1);
        //    if (part != NULL) {
        //        parts.push_back(part);
        //    }
        //    start = i;
        //    i++;
        //    while (str[i] == '.') i++;
        //    i--;
        //    part = copyAndTrim(str, start, i);
        //    if (part != NULL) {
        //        parts.push_back(part);
        //    }
        //    start = -1;
        //    //l = 0;
        //}
        else if (str[i] == '\"') {
            if (l == 0) {
                l = 2;
                if (str[i + 1] != '\0') {
                    start = i + 1;

                }
            }
            else if (l == 2) {
                l = 0;
                //str[i] = '\0';
                part = copyAndTrim(str, start, i - 1);
                if (part != NULL) {
                    parts.push_back(part);
                }
                start = -1;
            }
            else if (l == -1) {
                l = 2;
                part = copyAndTrim(str, start, i - 1);
                if (part != NULL) {
                    parts.push_back(part);
                }
                start = i + 1;
            }
        }
        else {
            if (l == 0) {
                start = i;
                l = -1;
            }
        }
        i++;
    }
    part = copyAndTrim(str, start, i - 1);
    if (part != NULL) {
        parts.push_back(part);
    }
    length = parts.size();
    TCHAR** output = (TCHAR**)malloc(length * sizeof(TCHAR*));
    for (int i = 0; i < length; i++) {
        output[i] = parts[i];
    }
    return output;
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
