#pragma once
#include <windows.h>
#include <vector>
typedef struct {
	LPCWSTR name;
	LPPROCESS_INFORMATION pi;
	LPSTARTUPINFO si;
	int status;
}ProcessInfo;
#define PROC_STAT_RUNNING 1;
#define PROC_STAT_STOP 2;
#define PROC_STAT_TERMINATED 3;
std::vector<ProcessInfo> *getProcessInfos();
int proc(TCHAR** cmdParts, int partCount);
