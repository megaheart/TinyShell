#pragma once
#include<iostream>
#include <windows.h>
#include<vector>
#include<string>


int runProcess(TCHAR** cmdParts, int partCount);//proc run <path> <mode>
int listProcess(TCHAR** cmdParts, int partCount);//proc list
int idofProcess(TCHAR** cmdParts, int partCount);//proc idof
int statProcess(TCHAR** cmdParts, int partCount);//proc stat
int exit(TCHAR** cmdParts, int partCount);//exit
int runbat(TCHAR** cmdParts, int partCount);//run bat bile




   


