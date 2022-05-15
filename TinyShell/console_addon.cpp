#include <windows.h>
#include <string>
#include "console_addon.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE getConsole()
{
    return HANDLE();
}
void setTextColor(int color) {
	SetConsoleTextAttribute(hConsole, color);
}
std::wstring currentPath() {
    TCHAR buffer[MAX_PATH] = { 0 };
    GetCurrentDirectory(MAX_PATH, buffer);
    return std::wstring(buffer);
}

std::wstring executablePath() {
    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return std::wstring(buffer).substr(0, pos);
}
//void openTextFile(char* fileName) {
//    std::wifstream myfile(fileName);
//
//    if (myfile.is_open())
//    {
//        std::wstring line;
//        myfile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
//        while (getline(myfile, line))
//        {
//            std::wcout << line << L'\n';
//        }
//        myfile.close();
//        /*std::wstringstream wss;
//        wss << myfile.rdbuf();
//        std::wcout << wss.str() << std::endl;
//        myfile.close();*/
//    }
//
//    else std::wcout << L"Unable to open file: " << fileName << std::endl;
//}