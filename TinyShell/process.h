#pragma once
#include <windows.h>
#include <vector>
typedef struct {
	TCHAR* name;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;

}ProcessInfo;

std::vector<ProcessInfo> *getProcessInfos();
