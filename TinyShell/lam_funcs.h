#pragma once
#include<iostream>
#include <windows.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <psapi.h>
#include <tlhelp32.h>
#include "console_addon.h"
#include "shell_functions.h"
#include <vector>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <assert.h>
#include <stdio.h>
#include <iomanip>
#include <sstream>
#include <string.h>
#include "duc_funcs.h"
int runProcess(TCHAR** cmdParts, int partCount);//proc run <path> <mode>
int getProcessListAll(TCHAR** cmdParts, int partCount);//in các tiến trình đang chạy
int idofProcess(TCHAR** cmdParts, int partCount);//proc idof
int exit(TCHAR** cmdParts, int partCount);//exit
int runbat(TCHAR** cmdParts, int partCount);//run bat bile
int findProcessByName(TCHAR** cmdParts, int partCount);//tìm tiến trình bằng tên
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);//Tín hiệu bàn phím Ctrl+C





