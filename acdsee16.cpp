#include "acdsee16.h"
#include "pefunc.h"
#include <tlhelp32.h>

static unsigned int GetProcessImageBase(DWORD pid)
{
    unsigned int ImageBase=0x400000;
    HANDLE hModuleSnap;
    MODULEENTRY32 me32;
    hModuleSnap=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if(hModuleSnap==INVALID_HANDLE_VALUE)
        return ImageBase;
    me32.dwSize=sizeof(MODULEENTRY32);
    if(!Module32First(hModuleSnap, &me32))
    {
        CloseHandle(hModuleSnap);
        return ImageBase;
    }
    return (unsigned int)me32.modBaseAddr;
}

static void CopyToClipboard(const char* text)
{
    HGLOBAL hText;
    char *pText;
    int len=strlen(text);
    hText=GlobalAlloc(GMEM_DDESHARE|GMEM_MOVEABLE, len+1);
    pText=(char*)GlobalLock(hText);
    strcpy(pText, text);
    OpenClipboard(0);
    EmptyClipboard();
    if(len)
        if(!SetClipboardData(CF_OEMTEXT, hText))
            MessageBeep(MB_ICONERROR);
    CloseClipboard();
}

static DWORD GetProcessID(IN LPCSTR szExeName)
{
    DWORD dwRet=0;
    HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hSnapshot!=INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 pe= {0};
        pe.dwSize=sizeof(PROCESSENTRY32);
        BOOL bRet=Process32First(hSnapshot, &pe);
        while(bRet)
        {
            if(!_stricmp(pe.szExeFile, szExeName))
            {
                dwRet=pe.th32ProcessID;
                break;
            }
            bRet=Process32Next(hSnapshot, &pe);
        }
        CloseHandle(hSnapshot);
    }
    return dwRet;
}

static unsigned int FindPattern(BYTE* d, unsigned int size)
{
    for(unsigned int i=0; i<size; i++) //FFD?84C075??8D????C745??????????FF15????????32C0
        if(d[i]==0xFF and (d[i+1]>>4)==0x0D and d[i+2]==0x84 and d[i+3]==0xC0 and d[i+4]==0x75 and d[i+6]==0x8D and d[i+9]==0xC7 and d[i+10]==0x45 and d[i+16]==0xFF and d[i+17]==0x15)// and d[i+22]==0x32 and d[i+23]==0xC0)
            return i;
    return 0;
}

void MemPatch(HWND hwndDlg)
{
    DWORD pid=GetProcessID("ACDSee16.exe");
    if(!pid)
    {
        MessageBoxA(hwndDlg, "ACDSee16.exe is not running", "AV? Admin?", MB_ICONERROR);
        return;
    }
    HANDLE hP=OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if(!hP)
    {
        MessageBoxA(hwndDlg, "Could not open process ACDSee16.exe!", "AV? Admin?", MB_ICONERROR);
        return;
    }
    unsigned char* peheader=new unsigned char[0x1000];
    if(!ReadProcessMemory(hP, (void*)GetProcessImageBase(pid), peheader, 0x1000, 0))
    {
        MessageBoxA(hwndDlg, "ReadProcessMemory failed", "AV? Admin?", MB_ICONERROR);
        delete[] peheader;
        return;
    }
    FILEINFO a;
    memset(&a, 0, sizeof(FILEINFO));
    a.lpFile=peheader;
    if(!FillFileData(&a))
    {
        MessageBoxA(hwndDlg, "Problem in PE Header", "AV? Admin?", MB_ICONERROR);
        delete[] peheader;
        return;
    }
    unsigned int epsec_addr=a.dwEntrySectionVirtualAddr+a.dwImageBase;
    unsigned int epsec_size=a.dwEntrySectionRawSize;
    delete[] peheader;
    unsigned char* epsec=new unsigned char[epsec_size];
    if(!ReadProcessMemory(hP, (void*)epsec_addr, epsec, epsec_size, 0))
    {
        MessageBoxA(hwndDlg, "ReadProcessMemory failed", "AV? Admin?", MB_ICONERROR);
        delete[] epsec;
        return;
    }
    unsigned short patch=0x1B0;
    unsigned char patchjmp=0;
    unsigned int patch1=FindPattern(epsec, epsec_size);
    if(!patch1)
    {
        MessageBoxA(hwndDlg, "Could not find patch address", "Already patched?", MB_ICONERROR);
        delete[] epsec;
        return;
    }
    if(!WriteProcessMemory(hP, (void*)(patch1+epsec_addr+22), &patch, 2, 0) or !WriteProcessMemory(hP, (void*)(patch1+epsec_addr+5), &patchjmp, 1, 0))
    {
        MessageBoxA(hwndDlg, "WriteProcessMemory failed", "AV? Admin?", MB_ICONERROR);
        delete[] epsec;
        return;
    }
    patch1++;
    unsigned int patch2=FindPattern(epsec+patch1, epsec_size-patch1);
    if(!patch2)
    {
        MessageBoxA(hwndDlg, "Could not find patch address", "Already patched?", MB_ICONERROR);
        delete[] epsec;
        return;
    }
    if(!WriteProcessMemory(hP, (void*)(patch1+patch2+epsec_addr+22), &patch, 2, 0) or !WriteProcessMemory(hP, (void*)(patch1+patch2+epsec_addr+5), &patchjmp, 1, 0))
    {
        MessageBoxA(hwndDlg, "WriteProcessMemory failed", "AV? Admin?", MB_ICONERROR);
        delete[] epsec;
        return;
    }
    delete[] epsec;
    char code[87]="";
    memset(code, '0', 86);
    CopyToClipboard(code);
    MessageBoxA(hwndDlg, "Patch done! Offline registration code copied to clipboard!", "OK", MB_ICONINFORMATION);
}
