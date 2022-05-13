#pragma once
#include<iostream>
#include <windows.h>
#include<vector>
#include<string>

void initializeCmds();
int executeCommand(TCHAR** cmdParts, int partCount);
