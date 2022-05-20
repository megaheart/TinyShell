#pragma once
#include<iostream>
#include <windows.h>
#include<vector>
#include<string>
<<<<<<< HEAD
=======
typedef int (*ShellFunction)(TCHAR** cmdParts, int partCount);
typedef struct ShellCommand {
    const TCHAR* name;
    ShellFunction func;
} ShellCommand;
>>>>>>> master

void initializeCmds();
ShellCommand** getAllCommands(int &cmdsCount);
int executeCommand(/*TCHAR* cmdLine, */TCHAR** cmdParts, int partCount);
