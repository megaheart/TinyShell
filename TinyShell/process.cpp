#include <windows.h>
#include <vector>
#include "duc_funcs.h"
#include "lam_funcs.h"
#include "process.h"


std::vector<ProcessInfo> *pInfos = new std::vector<ProcessInfo>;
std::vector<ProcessInfo> *getProcessInfos() {
	return pInfos;
}
int proc(TCHAR** cmdParts, int partCount) {
    if (partCount == 1 || std::wcscmp(cmdParts[1], L"?doc") == 0) {
        std::wcout << "proc:" << std::endl;
        std::wcout << "Description:\tUsed to react with process." << std::endl;
        std::wcout << "Usage:";

        //proc run <flag> <process-name>
        setTextColor(OCEAN);
        std::wcout << "\t\tproc run <flag> <process-name>";
        setTextColor(WHITE);
        std::wcout << " : Run <process-name> process with <flag> = \n\t\t\t\t\t\t";
        setTextColor(GREEN);
        std::wcout << "-b ";
        setTextColor(WHITE);
        std::wcout << "(background flag) or ";
        setTextColor(GREEN);
        std::wcout << "-f ";
        setTextColor(WHITE);
        std::wcout << "(foreground flag)." << std::endl;

        //proc list
        setTextColor(OCEAN);
        std::wcout << "\t\tproc list";
        setTextColor(WHITE);
        std::wcout << " : Show a list of processes." << std::endl;

        //proc idof <process-name>
        setTextColor(OCEAN);
        std::wcout << "\t\tproc idof <process-name>";
        setTextColor(WHITE);
        std::wcout << " : Get id of process whose name is <process-name>." << std::endl;

        //proc stat <process-status>
        setTextColor(OCEAN);
        std::wcout << "\t\tproc stat <process-status>";
        setTextColor(WHITE);
        std::wcout << " : Show a list of processes whose status is <process-status> (1|2) or (running|paused)." << std::endl;

        //proc kill <process-id>
        setTextColor(OCEAN);
        std::wcout << "\t\tproc kill <process-id>";
        setTextColor(WHITE);
        std::wcout << " : Terminate process whose id is <process-id>." << std::endl;

        //proc stop <process-id>
        setTextColor(OCEAN);
        std::wcout << "\t\tproc stop <process-id>";
        setTextColor(WHITE);
        std::wcout << " : Pause process whose id is <process-id>." << std::endl;

        //proc resume <process-id>
        setTextColor(OCEAN);
        std::wcout << "\t\tproc resume <process-id>";
        setTextColor(WHITE);
        std::wcout << " : Resume paused process whose id is <process-id>." << std::endl;

        std::wcout << std::endl;
        return 0;
    }
    if (std::wcscmp(cmdParts[1], L"list") == 0) {
        if (partCount != 2) {
            setTextColor(RED);
            std::wcout << L"Error: ";
            setTextColor(WHITE);
            std::wcout << "number of parameters is invalid, please type \"";
            setTextColor(OCEAN);
            std::wcout << L"help proc";
            setTextColor(WHITE);
            std::wcout << "\" to get information about ";
            setTextColor(OCEAN);
            std::wcout << L"proc list ";
            setTextColor(WHITE);
            std::wcout << "command.";
            std::wcout << std::endl << std::endl;
            return 1;
        }
        return getProcessListAll(cmdParts, partCount);
    }
    if (std::wcscmp(cmdParts[1], L"run") == 0) {
        if (partCount != 4) {
            setTextColor(RED);
            std::wcout << L"Error: ";
            setTextColor(WHITE);
            std::wcout << "number of parameters is invalid, please type \"";
            setTextColor(OCEAN);
            std::wcout << L"help proc";
            setTextColor(WHITE);
            std::wcout << "\" to get information about ";
            setTextColor(OCEAN);
            std::wcout << L"proc run ";
            setTextColor(WHITE);
            std::wcout << "command.";
            std::wcout << std::endl << std::endl;
            return 1;
        }
        return runProcess(cmdParts, partCount);
    }
    if (std::wcscmp(cmdParts[1], L"idof") == 0) {
        if (partCount != 3) {
            setTextColor(RED);
            std::wcout << L"Error: ";
            setTextColor(WHITE);
            std::wcout << "number of parameters is invalid, please type \"";
            setTextColor(OCEAN);
            std::wcout << L"help proc";
            setTextColor(WHITE);
            std::wcout << "\" to get information about ";
            setTextColor(OCEAN);
            std::wcout << L"proc idof ";
            setTextColor(WHITE);
            std::wcout << "command.";
            std::wcout << std::endl << std::endl;
            return 1;
        }
        return idofProcess(cmdParts, partCount);
    }
    if (std::wcscmp(cmdParts[1], L"stat") == 0) {
        if (partCount != 3) {
            setTextColor(RED);
            std::wcout << L"Error: ";
            setTextColor(WHITE);
            std::wcout << "number of parameters is invalid, please type \"";
            setTextColor(OCEAN);
            std::wcout << L"help proc";
            setTextColor(WHITE);
            std::wcout << "\" to get information about ";
            setTextColor(OCEAN);
            std::wcout << L"proc stat ";
            setTextColor(WHITE);
            std::wcout << "command.";
            std::wcout << std::endl << std::endl;
            return 1;
        }
        /*std::wcout << L">>> Sorry! This subcommand is being developed. :))";
        std::wcout << std::endl << std::endl;
        return 0;*/
        return procStat(cmdParts, partCount);
    }
    if (std::wcscmp(cmdParts[1], L"kill") == 0) {
        if (partCount != 3) {
            setTextColor(RED);
            std::wcout << L"Error: ";
            setTextColor(WHITE);
            std::wcout << "number of parameters is invalid, please type \"";
            setTextColor(OCEAN);
            std::wcout << L"help proc";
            setTextColor(WHITE);
            std::wcout << "\" to get information about ";
            setTextColor(OCEAN);
            std::wcout << L"proc kill ";
            setTextColor(WHITE);
            std::wcout << "command.";
            std::wcout << std::endl << std::endl;
            return 1;
        }
        DWORD process_ID = _wtoi(cmdParts[2]);
        return killProcessID(process_ID);
    }
    if (std::wcscmp(cmdParts[1], L"stop") == 0) {
        if (partCount != 3) {
            setTextColor(RED);
            std::wcout << L"Error: ";
            setTextColor(WHITE);
            std::wcout << "number of parameters is invalid, please type \"";
            setTextColor(OCEAN);
            std::wcout << L"help proc";
            setTextColor(WHITE);
            std::wcout << "\" to get information about ";
            setTextColor(OCEAN);
            std::wcout << L"proc stop ";
            setTextColor(WHITE);
            std::wcout << "command.";
            std::wcout << std::endl << std::endl;
            return 1;
        }
        DWORD process_ID = _wtoi(cmdParts[2]);
        return stopProcessID(process_ID);
    }
    if (std::wcscmp(cmdParts[1], L"resume") == 0) {
        if (partCount != 3) {
            setTextColor(RED);
            std::wcout << L"Error: ";
            setTextColor(WHITE);
            std::wcout << "number of parameters is invalid, please type \"";
            setTextColor(OCEAN);
            std::wcout << L"help proc";
            setTextColor(WHITE);
            std::wcout << "\" to get information about ";
            setTextColor(OCEAN);
            std::wcout << L"proc resume ";
            setTextColor(WHITE);
            std::wcout << "command.";
            std::wcout << std::endl << std::endl;
            return 1;
        }
        DWORD process_ID = _wtoi(cmdParts[2]);
        return resumeProcessID(process_ID);
    }
    setTextColor(RED);
    std::wcout << L"Error: ";
    setTextColor(WHITE);
    setTextColor(OCEAN);
    std::wcout << L"proc ";
    setTextColor(WHITE);
    std::wcout << "command does not include of ";
    setTextColor(RED);
    std::wcout << cmdParts[1];
    setTextColor(WHITE);
    std::wcout << " subcommand." << std::endl << std::endl;
    return 1;
}