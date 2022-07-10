#include "keygen.h"
#include "crc32.h"
#include "md5.h"

static const char method1[]="56AFGHJBCDEKLVZ7MNP2YTU8934QRSWX";
static const char method2[]="AD5HJK6EUVWFGLMNP2BC34QRSTXYZ789";

unsigned int base36_to_int(const char* str)
{
    int len=strlen(str);
    unsigned int res=0;
    for(int i=0; i<len; i++)
    {
        res*=36;
        if(str[i]<=0x39)
            res+=(str[i]-0x30);
        else if(str[i]<=0x59)
            res+=(str[i]-0x37);
    }
    return res;
}

inline unsigned int umulhi32(unsigned int x, unsigned int y)
{
    return (unsigned int)(((unsigned long long)x*y)>>34);
}

const char* strange_conversion(unsigned char* buffer, unsigned int size, int method)
{
    static char result[256];
    memset(result, 0, 256);

    unsigned int len=umulhi32(size*8-1, 0xCCCCCCCD)+1;
    for(unsigned int i=0,j=1,k=8,l=buffer[0]; i<len; i++)
    {
        if(k<5 and j<size)
        {
            l=(l<<8)|buffer[j];
            j++;
            k+=8;
        }
        k-=5;
        if(method==1)
            result[i]=method1[l&0x1f];
        else
            result[i]=method2[l&0x1f];
        l=l>>5;
    }
    return result;
}

char findaltchar(char a)
{
    int len=sizeof(method2);
    for(int i=0; i<len; i++)
    {
        if(a==method2[i])
            return method1[i];
    }
    return -1;
}

const char* KeygenGenerate(const char* product_id)
{
    char a[]="102371ewrhr58tifmIoigj564DSFeoipubnLOIUeEIpl89avaUOCV98845689gDKSeoiaFdNZZedajhasfj6sdf87erDF56X";
    const char* b=strange_conversion((unsigned char*)product_id, strlen(product_id), 1);
    memcpy(a+7, b, strlen(b));
    unsigned int product[4];
    md5(product, a, strlen(a)-1);
    srand(GetTickCount()^*product);
    char str1[8]="",str2[6]="Z",str3[2]="",str4[24]="",str5[2]="",str6[5]="BEK6",str7[2]="";
    for(int i=0; i<7; i++)
        str1[i]=method1[rand()%32];
    for(int i=0; i<4; i++)
        str2[i+1]=method1[rand()%32];
    str3[0]=method1[rand()%32];
    str5[0]=0x49;//method1[rand()%32];
    str7[0]=method1[rand()%32];
    char crcstr[256]="";
    crcstr[0]=str1[0];
    crcstr[1]=str1[1];
    crcstr[2]=str1[2];
    crcstr[3]=str1[3];
    crcstr[4]=str1[4];
    crcstr[5]=str1[5];
    crcstr[6]=str1[6];
    crcstr[7]=str2[0];
    crcstr[8]=str2[1];
    crcstr[9]=str2[2];
    crcstr[10]=str2[3];
    crcstr[11]=str2[4];
    crcstr[12]=str5[0];
    crcstr[13]=str6[0];
    crcstr[14]=str6[1];
    crcstr[15]=str6[2];
    crcstr[16]=str6[3];
    crcstr[17]=str7[0];
    crcstr[18]=str3[0];
    unsigned int crc=cust_crc32((unsigned char*)crcstr, 19);
    strcat(crcstr, strange_conversion((unsigned char*)&crc, 4, 1));
    unsigned int constappend=*product;
    int len=strlen(crcstr);
    memcpy(crcstr+len, &constappend, sizeof(unsigned int));
    len+=4;
    unsigned char hash[16];
    md5((unsigned int*)hash, crcstr, len);
    const char* final_cmp=strange_conversion(hash, 16, 2);

    for(int i=0; i<0x17; i++)
        str4[i]=findaltchar(final_cmp[i]);

    static char serial[]="XXXXXX-XXXXXX-XXXXXX-XXXXXX-ZXXXXX-BEK6XX-XXXXXX";

    serial[0x7]=str1[0];
    serial[0x1]=str1[1];
    serial[0x2]=str1[2];
    serial[0x3]=str1[3];
    serial[0x4]=str1[4];
    serial[0x5]=str1[5];
    serial[0x0]=str1[6];
    serial[0x1d]=str2[1];
    serial[0x1e]=str2[2];
    serial[0x1f]=str2[3];
    serial[0x20]=str2[4];
    serial[0x28]=str3[0];
    serial[0x8]=str4[0];
    serial[0x9]=str4[1];
    serial[0xa]=str4[2];
    serial[0xb]=str4[3];
    serial[0xc]=str4[4];
    serial[0xe]=str4[5];
    serial[0xf]=str4[6];
    serial[0x10]=str4[7];
    serial[0x11]=str4[8];
    serial[0x12]=str4[9];
    serial[0x13]=str4[10];
    serial[0x15]=str4[11];
    serial[0x16]=str4[12];
    serial[0x17]=str4[13];
    serial[0x18]=str4[14];
    serial[0x19]=str4[15];
    serial[0x1a]=str4[16];
    serial[0x2a]=str4[17];
    serial[0x2b]=str4[18];
    serial[0x2c]=str4[19];
    serial[0x2d]=str4[20];
    serial[0x2e]=str4[21];
    serial[0x2f]=str4[22];
    serial[0x21]=str5[0];
    serial[0x27]=str7[0];
    return serial;
}
