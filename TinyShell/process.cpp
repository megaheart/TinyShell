#include <windows.h>
#include <vector>
#include "process.h"


std::vector<ProcessInfo> *pInfos = new std::vector<ProcessInfo>;
std::vector<ProcessInfo> *getProcessInfos() {
	return pInfos;
}