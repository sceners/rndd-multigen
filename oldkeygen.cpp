#include "oldkeygen.h"
#include "crc32.h"

static void encode(int len, unsigned int arg2, char* dest, unsigned int a)
{
    const char table[]="34D7GHJKLMNPQRSTVWXYZ2BC56F89";
    int ecx=a;
    int eax=arg2;

    unsigned int esi=ecx;
    ecx=10;
    int edx=eax%ecx;

    ecx=len;
    ecx--;
    int edi=edx;
    if(ecx<0)
        return;
    int ebp;
    do
    {
        edx=0;
        eax=esi;
        esi=0x1d;
        edx=eax%esi;
        eax=eax/esi;
        ebp=0x1d;
        esi=eax;
        eax=edx+ecx;
        eax+=edi;
        edx=eax%ebp;
        eax=eax/ebp;
        ecx--;
        dest[ecx+1]=table[edx];
    }
    while(ecx>-1);
}

static char* gen()
{
    const char format[]="34D7GHJKLMNPQRSTVWXYZ2BC56F89";
    static char serial[]="XXXXXX-XXXXX-XXXXXX-XXXXXXX";
    int len=strlen(serial);
    for(int i=0; i<len; i++)
    {
        if(serial[i]!='-')
        {
            int rnd=rand()%28;
            if(i==4 and (rnd==6 or rnd==3))
                rnd++;
            serial[i]=format[rnd];
        }
    }
    return serial;
}

const char* OldKeygenGenerate(int rmin, int rmax, int lang, int res4)
{
    //generate random serial
    srand(GetTickCount()+rmin+rmax);
    char* serial=gen();

    //encode custom bytes
    encode(3, 1, serial, rand()%(rmax-rmin)+rmin);
    encode(1, 2, serial+3, lang);
    encode(1,0,serial+7,0);
    encode(4,0,serial+8,res4);

    //encode default dword
    encode(7, 0, serial+0x14, 0x9C20A3);

    //calculate crc
    unsigned int crc=def_crc32((unsigned char*)serial, strlen(serial));

    //encode crc in serial
    encode(7, 0, serial+0x14, crc+0x9C20A3);
    return serial;
}
