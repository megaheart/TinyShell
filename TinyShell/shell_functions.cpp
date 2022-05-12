#include<iostream>
#include<vector>
#include<string>
#include <windows.h>
#include "shell_functions.h"
#include "console_addon.h"

//Return 0 if execute command successful, otherwise return code
//Error code: 
//     1: partCount is invalid
typedef int (*ShellFunction)(TCHAR** cmdParts, int partCount);
typedef struct ShellCommand {
    const TCHAR* name;
    ShellFunction func;
} ShellCommand;

int test(TCHAR** cmdParts, int partCount) {
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
        std::wcout << "Description:\tUsed to test TinyShell's reaction." << std::endl;
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
    return 0;
}

int cmdSupportCount = 1;
std::vector<ShellCommand*> cmds;
void initializeCmds() {
    cmds.push_back(new ShellCommand{ L"test", &test });
}
TCHAR* copyAndTrim(TCHAR* str, int start, int end) {
    if (start < 0 || end < start) return NULL;
    while (start <= end && (str[start] == ' ' || str[start] == '\t')) start++;
    while (start <= end && (str[end] == ' ' || str[end] == '\t')) end++;
    if(start > end) return NULL;

    TCHAR *s = (TCHAR*)malloc((end - start + 2) * sizeof(TCHAR));
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
    /*if (str[0] != ' ' && str[0] != '\t') {
        parts.push_back(str);
    }*/
    int i = 0;
    int start = -1;
    TCHAR* part;
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
        else if (str[i] == '.') {
            if (l == -1) {
                if (start == -1) {
                    l = 0;
                    start = i;
                    i++;
                    continue;
                }
                if (parts.size() == 0) {
                    part = copyAndTrim(str, start, i - 1);
                    if (part != NULL) {
                        parts.push_back(part);
                    }
                    start = i;
                    while (str[i] == '.') i++;
                    i--;
                    part = copyAndTrim(str, start, i);
                    if (part != NULL) {
                        parts.push_back(part);
                    }
                    start = -1;
                }
                
            }
            
        }
        else if (str[i] == '\"') {
            if (l == 0) {
                l = 2;
                if (str[i + 1] != '\0') {
                    start = i + 1;
                    
                }
            }
            else if(l == 2) {
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
        std::wcout << L"'";
        setTextColor(RED);
        std::wcout << cmdParts[0];
        setTextColor(WHITE);
        std::wcout << L"' is not recognized as an internal or external command," << std::endl;
        std::wcout << "operable program or batch file." << std::endl;
        std::wcout << std::endl;
        return 1;
    }
    return func(cmdParts, partCount);
}
