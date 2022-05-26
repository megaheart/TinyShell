#pragma once
#include <windows.h>
#include <vector>
typedef struct {
	LPCWSTR name;
	LPPROCESS_INFORMATION pi;
	LPSTARTUPINFO si;
	int status;
}ProcessInfo;

std::vector<ProcessInfo> *getProcessInfos();
int proc(TCHAR** cmdParts, int partCount);
