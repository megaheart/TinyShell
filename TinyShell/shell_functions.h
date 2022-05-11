#pragma once
#include<iostream>
#include <windows.h>

void initializeCmds();
TCHAR** strSplit(TCHAR* str, int& length);
int executeCommand(TCHAR** cmdParts, int partCount);
