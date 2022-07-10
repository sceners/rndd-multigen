#include "pefunc.h"

DWORD RVAToOffset(IMAGE_NT_HEADERS* pNtHdr, DWORD dwRVA)
{
    WORD wSections;
    PIMAGE_SECTION_HEADER pSectionHdr;
    pSectionHdr=IMAGE_FIRST_SECTION(pNtHdr);
    wSections=pNtHdr->FileHeader.NumberOfSections;
    for(int i=0; i<wSections; i++)
    {
        if(pSectionHdr->VirtualAddress<=dwRVA)
            if((pSectionHdr->VirtualAddress+pSectionHdr->Misc.VirtualSize)>dwRVA)
            {
                dwRVA-=pSectionHdr->VirtualAddress;
                dwRVA+=pSectionHdr->PointerToRawData;
                return dwRVA;
            }
        pSectionHdr++;
    }
    return -1;
}

bool ReadInFile(FILEINFO* pFileInfo)
{
    DWORD dwReadBytes;
    BOOL Result=false;
    HANDLE hFile=CreateFile(pFileInfo->FilePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if(hFile==INVALID_HANDLE_VALUE)
        return Result;
    pFileInfo->dwFileSize=GetFileSize(hFile, 0);
    pFileInfo->lpFile=(BYTE*)malloc(pFileInfo->dwFileSize);
    if(pFileInfo->lpFile)
    {
        Result=ReadFile(hFile, pFileInfo->lpFile, pFileInfo->dwFileSize, &dwReadBytes, 0);
        if(!Result)
            free(pFileInfo->lpFile);
    }
    CloseHandle(hFile);
    return Result;
}

bool FillFileData(FILEINFO* pFileInfo)
{
    IMAGE_DOS_HEADER *pdh;
    IMAGE_NT_HEADERS *pnth;
    IMAGE_SECTION_HEADER *psh;
    pdh=(IMAGE_DOS_HEADER*)((DWORD)pFileInfo->lpFile);
    if(pdh->e_magic!=IMAGE_DOS_SIGNATURE)
        return false;
    pnth=(IMAGE_NT_HEADERS*)((DWORD)pFileInfo->lpFile+pdh->e_lfanew);
    if(IsBadReadPtr(pnth, 4))
        return false;
    if(pnth->Signature!=IMAGE_NT_SIGNATURE)
        return false;
    if(pnth->FileHeader.Machine!=IMAGE_FILE_MACHINE_I386)
        return false;
    pFileInfo->pnth=pnth;
    psh=(IMAGE_SECTION_HEADER*)((DWORD)(pnth)+pnth->FileHeader.SizeOfOptionalHeader+sizeof(IMAGE_FILE_HEADER)+sizeof(DWORD));
    pFileInfo->psh=psh;
    pFileInfo->dwNumberOfSections=pnth->FileHeader.NumberOfSections;
    pFileInfo->dwEntrypoint=((unsigned int)pnth->OptionalHeader.AddressOfEntryPoint);
    if(pnth->FileHeader.Characteristics&IMAGE_FILE_DLL)
        pFileInfo->bIsDll=true;
    else
        pFileInfo->bIsDll=false;
    if(pFileInfo->bIsDll)
        return false;
    pFileInfo->dwImageBase=pnth->OptionalHeader.ImageBase;
    pFileInfo->dwSizeOfImage=pnth->OptionalHeader.SizeOfImage;
    int counter=0;
    while((unsigned int)pFileInfo->dwEntrypoint>=psh[counter].VirtualAddress)
        counter++;
    pFileInfo->dwEntrySectionRawAddr=RVAToOffset(pnth, psh[counter-1].VirtualAddress);
    pFileInfo->dwEntrySectionRawSize=psh[counter-1].SizeOfRawData;
    pFileInfo->dwEntrySectionVirtualAddr=psh[counter-1].VirtualAddress;

    return true;
}

void FreeFileInfoStruct(FILEINFO* fileinfo)
{
    if(fileinfo->lpFile)
        free(fileinfo->lpFile);
    free(fileinfo);
}
