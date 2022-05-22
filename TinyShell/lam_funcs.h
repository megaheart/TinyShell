#pragma once
#include<iostream>
#include <windows.h>
#include<vector>
#include<string>

int runProcess(TCHAR** cmdParts, int partCount);//proc run <path> <mode>
int getProcessListAll(TCHAR** cmdParts, int partCount);//in các tiến trình đang chạy
int idofProcess(TCHAR** cmdParts, int partCount);//proc idof
int exit(TCHAR** cmdParts, int partCount);//exit
int runbat(TCHAR** cmdParts, int partCount);//run bat bile
int findProcessByName(TCHAR** cmdParts, int partCount);//tìm tiến trình bằng tên


   


