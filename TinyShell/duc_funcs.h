#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <windows.h>
#include <sys/types.h>
#include <psapi.h>
#include <tlhelp32.h>
#include "console_addon.h"
#include "shell_functions.h"

int killProcessID(DWORD process_ID);

int stopProcessID(DWORD process_ID);

int resumeProcessID(DWORD process_ID);

int cls(TCHAR** cmdParts);

int echo(TCHAR** cmpParts);
