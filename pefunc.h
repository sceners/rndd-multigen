#ifndef _PEFUNC_H
#define _PEFUNC_H

#include <windows.h>
#include <stdio.h>

struct FILEINFO
{
    char FilePath[256];

    bool bIsDll;
    unsigned char* lpFile;
    unsigned int dwFileSize;
    unsigned int dwEntrypoint;
    unsigned int dwImageBase;
    unsigned int dwSizeOfImage;
    unsigned int dwEntrySectionRawAddr;
    unsigned int dwEntrySectionRawSize;
    unsigned int dwEntrySectionVirtualAddr;
    unsigned int dwNumberOfSections;

    IMAGE_NT_HEADERS* pnth;
    IMAGE_SECTION_HEADER* psh;
};

bool FillFileData(FILEINFO* pFileInfo);
DWORD RVAToOffset(IMAGE_NT_HEADERS* pNtHdr, DWORD dwRVA);
bool ReadInFile(FILEINFO* pFileInfo);
void FreeFileInfoStruct(FILEINFO* fileinfo);

#endif
