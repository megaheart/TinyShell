#pragma once
#include<iostream>
#include <windows.h>
#include<vector>
#include<string>

void initializeCmds();
TCHAR** strSplit(TCHAR* str, int& length);
int executeCommand(TCHAR** cmdParts, int partCount);
