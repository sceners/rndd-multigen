#include "hosts.h"
#include <shlwapi.h>

bool HostsPatch(HWND hwndDlg)
{
    char hosts[256]="";
    GetWindowsDirectoryA(hosts, 256);
    PathAppendA(hosts, "system32\\drivers\\etc\\hosts");
    HANDLE hFile=CreateFileA(hosts, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if(hFile==INVALID_HANDLE_VALUE)
    {
        MessageBoxA(hwndDlg, "Could not open hosts file for reading", "Opened with admin privileges?", MB_ICONERROR);
        return false;
    }
    unsigned int size=GetFileSize(hFile, 0);
    char* data=new char[size+50];
    memset(data, 0, size+50);
    DWORD read=0;
    if(!ReadFile(hFile, data, size, &read, 0))
    {
        MessageBoxA(hwndDlg, "ReadFile error", "Cause unknown...", MB_ICONERROR);
        CloseHandle(hFile);
        delete[] data;
        return false;
    }
    CloseHandle(hFile);
    if(strstr(data, "\n127.0.0.1 acdid.acdsystems.com"))
    {
        MessageBoxA(hwndDlg, "Hosts file already patched!", "Already patched!", MB_ICONINFORMATION);
        delete[] data;
        return true;
    }
    strcat(data, "\r\n127.0.0.1 acdid.acdsystems.com");
    hFile=CreateFileA(hosts, GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if(hFile==INVALID_HANDLE_VALUE)
    {
        MessageBoxA(hwndDlg, "Could not open hosts file for writing", "Opened with admin privileges?", MB_ICONERROR);
        delete[] data;
        return false;
    }
    SetFilePointer(hFile, 0, 0, FILE_BEGIN);
    SetEndOfFile(hFile);
    DWORD written=0;
    if(!WriteFile(hFile, data, strlen(data), &written, 0))
    {
        MessageBoxA(hwndDlg, "WriteFile error", "Cause unknown...", MB_ICONERROR);
        CloseHandle(hFile);
        delete[] data;
        return false;
    }
    MessageBoxA(hwndDlg, "Hosts file patched!", "Done!", MB_ICONINFORMATION);
    CloseHandle(hFile);
    delete[] data;
    return true;
}
