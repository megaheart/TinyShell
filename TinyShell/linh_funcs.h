#include<iostream>
#include<vector>
#include<string>
#include <windows.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "console_addon.h"

int path(TCHAR** cmdParts, int partCount) ;

int addpath(TCHAR** cmdParts, int partCount) ;

int cd(TCHAR** cmdParts, int partCount) ;

int dir(TCHAR** cmdParts, int partCount) ;

int time(TCHAR** cmdParts, int partCount) ;

int date(TCHAR** cmdParts, int partCount) ;

int help(TCHAR** cmdParts, int partCount) ;

int del(TCHAR** cmdParts, int partCount) ;