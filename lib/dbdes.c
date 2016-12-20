#include "ngx_core.h"

unsigned char CCC[17][28], D[17][28], K[17][48], c, ch;
int ls_count[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

int e_r[48] =
{
    32, 1 , 2 , 3 , 4 , 5 , 4 , 5 ,
    6 , 7 , 8 , 9 , 8 , 9 , 10, 11,
    12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21,
    22, 23, 24, 25, 24, 25, 26, 27,
    28, 29, 28, 29, 30, 31, 32, 1
};

int P[32] =
{
    16, 7, 20, 21, 29, 12, 28, 17,
    1 , 15, 23, 26, 5 , 18, 31, 10,
    2, 8 , 24, 14, 32, 27, 3 , 9 ,
    19, 13, 30, 6 , 22, 11, 4 , 25
};

int SSS[16][4][16] =
{
    14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
    0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, /* err on */
    4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
    15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,

    15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
    3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
    0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
    13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,

    10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
    13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
    13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
    1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,

    7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
    13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
    10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
    3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14, /* err on */

    2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
    14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6, /* err on */
    4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
    11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,

    12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
    10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
    9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
    4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,

    4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
    13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
    1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
    6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,

    13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
    1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
    7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
    2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
};

int pc_1_c[28] =
{
    57, 49, 41, 33, 25, 17, 9 ,
    1 , 58, 50, 42, 34, 26, 18,
    10, 2 , 59, 51, 43, 35, 27,
    19, 11, 3 , 60, 52, 44, 36
};

int pc_1_d[28] =
{
    63, 55, 47, 39, 31, 23, 15,
    7 , 62, 54, 46, 38, 30, 22,
    14, 6 , 61, 53, 45, 37, 29,
    21, 13, 5 , 28, 20, 12, 4
};

int pc_2[48] =
{
    14, 17, 11, 24, 1 , 5 , 3 , 28,
    15, 6 , 21, 10, 23, 19, 12, 4 ,
    26, 8 , 16, 7 , 27, 20, 13, 2 ,
    41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56,
    34, 53, 46, 42, 50, 36, 29, 32
};

int ip_tab[64] =
{
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9 , 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

int _ip_tab[64] =
{
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

char hextoasc11(int xxc)
{
    xxc &= 0x0f;

    if(xxc < 0x0a)
    {
        xxc += '0';
    }
    else
    {
        xxc += 0x37;
    }

    return (char)xxc;
}

char hexlowtoasc11(int xxc)
{
    xxc &= 0x0f;

    if(xxc < 0x0a)
    {
        xxc += '0';
    }
    else
    {
        xxc += 0x37;
    }

    return (char)xxc;
}

char hexhightoasc11(int xxc)
{
    xxc &= 0xf0;
    xxc = xxc >> 4;

    if(xxc < 0x0a)
    {
        xxc += '0';
    }
    else
    {
        xxc += 0x37;
    }

    return (char)xxc;
}

char asctohex11(char ch1, char ch2)
{
    char ch;

    if(ch1 >= 'A')
    {
        ch = (char)((ch1 - 0x37) << 4);
    }
    else
    {
        ch = (char)((ch1 - '0') << 4);
    }

    if(ch2 >= 'A')
    {
        ch |= ch2 - 0x37;
    }
    else
    {
        ch |= ch2 - '0';
    }

    return ch;
}

int aschex_to_bcdhex11(char aschex[], int len, char bcdhex[])
{
    int i, j;
    char buf[512];
    memcpy(buf, aschex, len);

    if(len % 2 == 0)
    {
        j = len / 2;
    }
    else
    {
        j = len / 2 + 1;
    }

    for(i = 0; i < j; i++)
    {
        bcdhex[i] = asctohex11(aschex[2 * i], aschex[2 * i + 1]);
    }

    return(j);
}

int bcdhex_to_aschex11(char bcdhex[], int len, char aschex[])
{
    int i;

    for(i = 0; i < len; i++)
    {
        aschex[2 * i]   = hexhightoasc11(bcdhex[i]);
        aschex[2 * i + 1] = hexlowtoasc11(bcdhex[i]);
    }

    return(len * 2);
}

int byte2int_to_bcd11(int aa, char xx[])
{
    if(aa >= 256 * 256)
    {
        return(-1);
    }

    xx[0] = aa / 256;
    xx[1] = aa % 256;
    return(0);
}

int IntToBcd11(int aa, char xx[])
{
    if(aa > 256 * 256)
    {
        return(-1);
    }

    xx[0] = aa / 256;
    xx[1] = aa % 256;
    return(0);
}

int BcdToInt11(char xx[])
{
    return(xx[0] * 256 + xx[1]);
}


void Des(unsigned char *key, unsigned char *text, unsigned char *mtext)
{
    unsigned char tmp[64];
    unsigned char keybuf[9];
    unsigned char textbuf[9];
    unsigned char mtextbuf[9];
    memset(keybuf, 0, 9);
    memset(textbuf, 0, 9);
    memset(mtextbuf, 0, 9);
    aschex_to_bcdhex11((char *)key, 16, (char *)keybuf);
    aschex_to_bcdhex11((char *)text, 16, (char *)textbuf);
    expand0(keybuf, tmp);
    setkeystar(tmp);
    encrypt0(textbuf, mtextbuf);
    bcdhex_to_aschex11((char *)mtextbuf, 8, (char *)mtext);
}

void DesEcb(unsigned char *key, unsigned char *text, unsigned char *mtext, char padType)
{
    char hexText[16+1];
    char srcTmp[16+1];
    char destTmp[16+1];
    int slen = 0;
    int fillNum = 0;//要补齐的字节数
    int rand = 0; 
    int i = 0;
  
    slen = strlen(text);
    rand = slen % 16;
    if(slen % 2 != 0)
    {
        return;
    }     
    memset(hexText, 0, sizeof(hexText));  
    memset(srcTmp, 0, sizeof(srcTmp));
    memset(destTmp, 0, sizeof(destTmp));

    if(padType == 0)
    {
        if(rand != 0)
        {
              char sText[16];
              Str2Hex(text, hexText, slen); 
            fillNum = 8 - rand / 2;
            memset(sText, 0, sizeof(sText));
            for(i = 0; i < fillNum; i++)
            {
                hexText[i] = fillNum;       
            }
            Hex2Str(hexText, sText, fillNum);
            strcat((char*)text, sText);
        }
    }
    
    for(i = 0; i < strlen(text); i += 16)
    {
        memcpy(srcTmp, &text[i], 16);
        srcTmp[16] = '\0';
        Des(key, srcTmp, destTmp);
        destTmp[16] = '\0';
        strcat(mtext, destTmp);
    }
    mtext[strlen(text)] = '\0';
}

void DesCbc(unsigned char *key, unsigned char *init, unsigned char *text, unsigned char *mtext, char padType)
{
      char hexText[255];
      char hexXor[16+1];//与明文进行异或的向量，初始时保存init
    char srcTmp[16+1];
    char destTmp[16+1];
    char hexTmp[16+1];
    int slen = 0;
    int fillNum = 0;//要补齐的字节数
    int rand = 0; 
    int i = 0;
    int j = 0;
   
    slen = strlen(text);
    rand = slen % 16;
    if(slen % 2 != 0)
    {
        return;
    }   
        
    memset(hexText, 0, sizeof(hexText));
    memset(hexXor, 0, sizeof(hexXor));
    memset(hexTmp, 0, sizeof(hexTmp));
    memset(srcTmp, 0, sizeof(srcTmp));
    memset(destTmp, 0, sizeof(destTmp)); 
    Str2Hex(text, hexText, slen);   
    Str2Hex(init, hexXor, 16); 
    
    if(padType == 0)
    {
        if(rand != 0)
        {  
            fillNum = 8 - rand / 2;
            for(i = 0; i < fillNum; i++)
            {
                hexText[slen/2+i] = 0;
            } 
        }
    }
    
    for(i = 0; i < slen/2+fillNum; i += 8)
    {
          for(j = 0; j < 8; j++)
          {
                hexTmp[j] = hexText[i+j]^hexXor[j];
          }
          Hex2Str(hexTmp, srcTmp, 8);
          srcTmp[16] = '\0';
        Des(key, srcTmp, destTmp);
        destTmp[16] = '\0';
        //strcat(mtext, destTmp);
        Str2Hex(destTmp, hexXor, 16);
    }
    memcpy(mtext, destTmp, 16);
    //mtext[slen+fillNum*2] = '\0';
}

void TDes(unsigned char *key, unsigned char *text, unsigned char *mtext)
{
    unsigned char tmp[64], tmp2[64];
    memset(tmp, 0, sizeof(tmp));
    memset(tmp2, 0, sizeof(tmp2));
    Des(key, text, tmp);
    _Des(key + 16, tmp, tmp2);
    Des(key, tmp2, mtext);
}

void TDesEcb(unsigned char *key, unsigned char *text, unsigned char *mtext, char padType)
{
    char hexText[16+1];
    char srcTmp[16+1];
    char destTmp[16+1];
    int slen = 0;
    int fillNum = 0;//要补齐的字节数
    int rand = 0; 
    int i = 0;
  
    slen = strlen(text);
    rand = slen % 16;
    if(slen % 2 != 0)
    {
        return;
    }     
    memset(hexText, 0, sizeof(hexText));  
    memset(srcTmp, 0, sizeof(srcTmp));
    memset(destTmp, 0, sizeof(destTmp));

    if(padType == 0)
    {
        if(rand != 0)
        {
              Str2Hex(text, hexText, slen); 
            fillNum = 8 - rand / 2;
            char sText[16+1];
            memset(sText, 0, sizeof(sText));
            for(i = 0; i < fillNum; i++)
            {
                hexText[i] = fillNum;      
            }
            Hex2Str(hexText, sText, fillNum);
            strcat((char*)text, sText);            
        }
    }
    
    for(i = 0; i < strlen(text); i += 16)
    {
        memcpy(srcTmp, &text[i], 16);
        srcTmp[16] = '\0';
        TDes(key, srcTmp, destTmp);
        destTmp[16] = '\0';
        strcat(mtext, destTmp);
    }
    mtext[strlen(text)] = '\0';
}

void TDesCbc(unsigned char *key, unsigned char *init, unsigned char *text, unsigned char *mtext, char padType)
{
      char hexText[255];
      char hexXor[16+1];//与明文进行异或的向量，初始时保存init
    char srcTmp[16+1];
    char destTmp[16+1];
    char hexTmp[16+1];
    int slen = 0;
    int fillNum = 0;//要补齐的字节数
    int rand = 0; 
    int i = 0;
    int j = 0;
   
    slen = strlen(text);
    rand = slen % 16;
    if(slen % 2 != 0)
    {
        return;
    }       
    memset(hexText, 0, sizeof(hexText));
    memset(hexXor, 0, sizeof(hexXor));
    memset(hexTmp, 0, sizeof(hexTmp));
    memset(srcTmp, 0, sizeof(srcTmp));
    memset(destTmp, 0, sizeof(destTmp)); 
    Str2Hex(text, hexText, strlen(text));   
    Str2Hex(init, hexXor, 16); 
    
    if(padType == 0)
    {
        if(rand != 0)
        {  
              Str2Hex(text, hexText, slen); 
            fillNum = 8 - rand / 2;
            for(i = 0; i < fillNum; i++)
            {
                hexText[slen/2+i] = fillNum;
            }       
        }
    }
    
    for(i = 0; i < slen/2+fillNum; i += 8)
    {
          for(j = 0; j < 8; j++)
          {
                hexTmp[j] = hexText[i+j]^hexXor[j];
          }
          Hex2Str(hexTmp, srcTmp, 8);
          srcTmp[16] = '\0';
        TDes(key, srcTmp, destTmp);
        destTmp[16] = '\0';
        strcat(mtext, destTmp);
        Str2Hex(destTmp, hexXor, 16);
    }
    mtext[slen+fillNum*2] = '\0';
}

void  _Des(unsigned char *key, unsigned char *text, unsigned char *mtext)
{
    unsigned char tmp[64];
    unsigned char keybuf[9];
    unsigned char textbuf[9];
    unsigned char mtextbuf[9];
    memset(keybuf, 0, 9);
    memset(textbuf, 0, 9);
    memset(mtextbuf, 0, 9);
    aschex_to_bcdhex11((char *)key, 16, (char *)keybuf);
    aschex_to_bcdhex11((char *)text, 16, (char *)textbuf);
    expand0(keybuf, tmp);
    setkeystar(tmp);
    discrypt0(textbuf, mtextbuf);
    bcdhex_to_aschex11((char *)mtextbuf, 8, (char *)mtext);
}

void _DesEcb(unsigned char *key, unsigned char *text, unsigned char *mtext)
{
      char srcTmp[16+1];
    char destTmp[16+1];
    int slen = 0;
    int i = 0;
  
    slen = strlen(text);
    if(slen % 16 != 0)
    {
        return;
    }     
     
    memset(srcTmp, 0, sizeof(srcTmp));
    memset(destTmp, 0, sizeof(destTmp));
    
    for(i = 0; i < slen; i += 16)
    {
        memcpy(srcTmp, &text[i], 16);
        srcTmp[16] = '\0';
        _Des(key, srcTmp, destTmp);
        destTmp[16] = '\0';
        strcat(mtext, destTmp);
    }
    mtext[slen] = '\0';
}

void _DesCbc(unsigned char *key, unsigned char *init, unsigned char *text, unsigned char *mtext)
{
      char tmpText[255];//解密字符串
      char hexXor[16+1];//进行异或运算的向量
    char srcTmp[16+1];//源字符串
    char destTmp[16+1];//目的字符串
    char hexTmp[16+1];//tmpText的十六进制
    char hexDest[16+1];
    int i = 0;
    int j = 0;   
    int slen = strlen(text);
    
    if(slen % 16 != 0)
    {
        return;
    }
           
    memset(tmpText, 0, sizeof(tmpText));
    memset(hexXor, 0, sizeof(hexXor));
    memset(srcTmp, 0, sizeof(srcTmp));
    memset(destTmp, 0, sizeof(destTmp));
    memset(hexTmp, 0, sizeof(hexTmp));
    memset(hexDest, 0, sizeof(hexDest));
    
    for(i = slen-16; i >= 0; i -= 16)
    {
          memcpy(srcTmp, &text[i], 16);
        srcTmp[16] = '\0';
          _Des(key, srcTmp, tmpText);
          tmpText[16] = '\0';
          Str2Hex(tmpText, hexTmp, 16);
          
          if(i > 0)
          {
              memcpy(tmpText, &text[i-16], 16);
              Str2Hex(tmpText, hexXor, 16); 
          }
          else if(i == 0)
          {
              Str2Hex(init, hexXor, 16);
          }
           
          for(j = 0; j < 8; j++)
          {
                hexDest[j] = hexTmp[j]^hexXor[j];
          }
          
        Hex2Str(hexDest, destTmp, 8);
        destTmp[16] = '\0';
        
        for(j = 0; j < 16; j++)
        {
            mtext[i+j] = destTmp[j];
        }
    }
    mtext[slen] = '\0';
}

void _TDes(unsigned char *key, unsigned char *text, unsigned char *mtext)
{
    unsigned char tmp[64], tmp2[64];
    memset(tmp, 0, sizeof(tmp));
    memset(tmp2, 0, sizeof(tmp2));
    _Des(key, text, tmp);
    Des(key + 16, tmp, tmp2);
    _Des(key, tmp2, mtext);
}

void _TDesEcb(unsigned char *key, unsigned char *text, unsigned char *mtext)
{
      char srcTmp[16+1];
    char destTmp[16+1];
    int slen = 0;
    int i = 0;
  
    slen = strlen(text);
    if(slen % 16 != 0)
    {
        return;
    }     
     
    memset(srcTmp, 0, sizeof(srcTmp));
    memset(destTmp, 0, sizeof(destTmp));
    
    for(i = 0; i < strlen(text); i += 16)
    {
        memcpy(srcTmp, &text[i], 16);
        srcTmp[16] = '\0';
        _TDes(key, srcTmp, destTmp);
        destTmp[16] = '\0';
        strcat(mtext, destTmp);
    }
    mtext[strlen(text)] = '\0';
}

void _TDesCbc(unsigned char *key, unsigned char *init, unsigned char *text, unsigned char *mtext)
{
      char tmpText[255];//解密字符串
      char hexXor[16+1];//进行异或运算的向量
    char srcTmp[16+1];//源字符串
    char destTmp[16+1];//目的字符串
    char hexTmp[16+1];//tmpText的十六进制
    char hexDest[16+1];
    int i = 0;
    int j = 0;   
    int slen = strlen(text);
    
    if(slen % 16 != 0)
    {
        return;
    }
           
    memset(tmpText, 0, sizeof(tmpText));
    memset(hexXor, 0, sizeof(hexXor));
    memset(srcTmp, 0, sizeof(srcTmp));
    memset(destTmp, 0, sizeof(destTmp));
    memset(hexTmp, 0, sizeof(hexTmp));
    memset(hexDest, 0, sizeof(hexDest));
    
    for(i = slen-16; i >= 0; i -= 16)
    {
          memcpy(srcTmp, &text[i], 16);
        srcTmp[16] = '\0';
          _TDes(key, srcTmp, tmpText);
          tmpText[16] = '\0';
          Str2Hex(tmpText, hexTmp, 16);
          
          if(i > 0)
          {
              memcpy(tmpText, &text[i-16], 16);
              Str2Hex(tmpText, hexXor, 16); 
          }
          else if(i == 0)
          {
              Str2Hex(init, hexXor, 16);
          }
           
          for(j = 0; j < 8; j++)
          {
                hexDest[j] = hexTmp[j]^hexXor[j];
          }
          
        Hex2Str(hexDest, destTmp, 8);
        destTmp[16] = '\0';
        
        for(j = 0; j < 16; j++)
        {
            mtext[i+j] = destTmp[j];
        }
    }
    mtext[slen] = '\0';
}
/****************************************
3倍3Des加密,数据的入参都是字符串,不是hex
入参:48位(24字节)key, 数据16位(8字节)text
出参: 16位(8字节)mtext
*****************************************/
void TTDes(unsigned char *key, unsigned char *text, unsigned char *mtext)
{
    unsigned char tmp[64], tmp2[64];
    memset(tmp, 0, sizeof(tmp));
    memset(tmp2, 0, sizeof(tmp2));
    Des(key, text, tmp);
    _Des(key + 16, tmp, tmp2);
    Des(key + 32, tmp2, mtext);
}

void _TTDes(unsigned char *key, unsigned char *text, unsigned char *mtext)
{
    unsigned char tmp[64], tmp2[64];
    memset(tmp, 0, sizeof(tmp));
    memset(tmp2, 0, sizeof(tmp2));
    _Des(key+32, text, tmp);    
    Des(key + 16, tmp, tmp2);
    _Des(key , tmp2, mtext);  
}

/***********************************
入参:密钥: 48位(24字节)key  
     数据: text,长度不限,每8个字节(16位)作为加密单元
     填充参数: padType 0,不填充  1,填充方式1(PKCS5Padding方式)缺几个字节,就补几 
     初始向量:init 16位(8字节) 一般为000.. 
出参:加密结果: mtext
addby gejing
modify by jiang_20150623
************************************/
void TTDesEcb(unsigned char *key, unsigned char *text, unsigned char *mtext, char padType)
{
    char hexText[16+1];
    char srcTmp[16+1];
    char destTmp[16+1];
    int slen = 0;
    int fillNum = 0;//要补齐的字节数
    int rand = 0; 
    int i = 0;
  
    slen = strlen(text);
    rand = slen % 16;
    if(slen % 2 != 0)
    {
        return;
    }     
    memset(hexText, 0, sizeof(hexText));  
    memset(srcTmp, 0, sizeof(srcTmp));
    memset(destTmp, 0, sizeof(destTmp));

    if(padType == 0)
    {
        if(rand != 0)
        {
              Str2Hex(text, hexText, slen); 
            fillNum = 8 - rand / 2;
            char sText[16];
            memset(sText, 0, sizeof(sText));
            for(i = 0; i < fillNum; i++)
            {
                hexText[i] = fillNum;      
            }
            Hex2Str(hexText, sText, fillNum);
            strcat((char*)text, sText);
        }
    }
    
    for(i = 0; i < strlen(text); i += 16)
    {
        memcpy(srcTmp, &text[i], 16);
        srcTmp[16] = '\0';
        TTDes(key, srcTmp, destTmp);
        destTmp[16] = '\0';
        strcat(mtext, destTmp);
    }
    mtext[strlen(text)] = '\0';
}

void TTDesCbc(unsigned char *key, unsigned char *init, unsigned char *text, unsigned char *mtext, char padType)
{
      char hexText[4096];
      char hexXor [16 + 1];//与明文进行异或的向量，初始时保存init
    char srcTmp [16 + 1];
    char destTmp[16 + 1];
    char hexTmp [16 + 1];
    int slen = 0;
    int fillNum = 0;//要补齐的字节数
    int rand = 0; 
    int i = 0;
    int j = 0;
   
    slen = strlen(text); 
    rand = slen % 16;      
    memset(hexText, 0, sizeof(hexText));
    memset(hexXor, 0, sizeof(hexXor));
    memset(hexTmp, 0, sizeof(hexTmp));
    memset(srcTmp, 0, sizeof(srcTmp));
    memset(destTmp, 0, sizeof(destTmp)); 
    Str2Hex(text, hexText, strlen(text));   
    Str2Hex(init, hexXor, 16);
    //memcpy(hexText, text, slen);
    //memcpy(hexXor, init, 16);
    
    if(padType == 0)
    {
        if(rand != 0)
        {  
              Str2Hex(text, hexText, slen); 
            fillNum = 8 - rand / 2;
            for(i = 0; i < fillNum; i++)
            {
                hexText[slen/2+i] = fillNum;
            }       
        }
    }
    
    for(i = 0; i < slen/2+fillNum; i += 8)
    {
          for(j = 0; j < 8; j++)
          {
                hexTmp[j] = hexText[i+j]^hexXor[j];
          }
          Hex2Str(hexTmp, srcTmp, 8);
          srcTmp[16] = '\0';
        TTDes(key, srcTmp, destTmp);
        destTmp[16] = '\0';
        strcat(mtext, destTmp);
        Str2Hex(destTmp, hexXor, 16);
    }
    mtext[slen+fillNum*2] = '\0';
}

void _TTDesEcb(unsigned char *key, unsigned char *text, unsigned char *mtext)
{
      char srcTmp[16+1];
    char destTmp[16+1];
    int slen = 0;
    int i = 0;
  
    slen = strlen(text);
    if(slen % 16 != 0)
    {
        return;
    }     
     
    memset(srcTmp, 0, sizeof(srcTmp));
    memset(destTmp, 0, sizeof(destTmp));
    
    for(i = 0; i < strlen(text); i += 16)
    {
        memcpy(srcTmp, &text[i], 16);
        srcTmp[16] = '\0';
        _TTDes(key, srcTmp, destTmp);
        destTmp[16] = '\0';
        strcat(mtext, destTmp);
    }
    mtext[strlen(text)] = '\0';
}

void _TTDesCbc(unsigned char *key, unsigned char *init, unsigned char *text, unsigned char *mtext)
{
      char tmpText[4096];//解密字符串
      char hexXor [16 + 1];//进行异或运算的向量
    char srcTmp [16 + 1];//源字符串
    char destTmp[16 + 1];//目的字符串
    char hexTmp [16 + 1];//tmpText的十六进制
    char hexDest[16 + 1];
    int i = 0;
    int j = 0;   
    int slen = strlen(text);
    
           
    memset(tmpText, 0, sizeof(tmpText));
    memset(hexXor, 0, sizeof(hexXor));
    memset(srcTmp, 0, sizeof(srcTmp));
    memset(destTmp, 0, sizeof(destTmp));
    memset(hexTmp, 0, sizeof(hexTmp));
    memset(hexDest, 0, sizeof(hexDest));
    
    for(i = slen-16; i >= 0; i -= 16)
    {
          memcpy(srcTmp, &text[i], 16);
        srcTmp[16] = '\0';
          _TTDes(key, srcTmp, tmpText);
          tmpText[16] = '\0';
          Str2Hex(tmpText, hexTmp, 16);
          
          if(i > 0)
          {
              memcpy(tmpText, &text[i-16], 16);
              Str2Hex(tmpText, hexXor, 16); 
          }
          else if(i == 0)
          {
              Str2Hex(init, hexXor, 16);
          }
           
          for(j = 0; j < 8; j++)
          {
                hexDest[j] = hexTmp[j]^hexXor[j];
          }
          
        Hex2Str(hexDest, destTmp, 8);
        destTmp[16] = '\0';
        
        for(j = 0; j < 16; j++)
        {
            mtext[i+j] = destTmp[j];
        }
    }
    mtext[slen] = '\0';
}

/*int main(int argc, char *argv[])
{
    char src[255];
    char dest[255];
    memset(src, 0, sizeof(src));
    memset(dest, 0, sizeof(dest));
    strcpy(src, "2A7FC01A59AFB078F9873B9DACCC9C7BDCBAFB5104040404");
    _TTDesEcb("12345678901234561A2B3C4D5E6F99991122334455667788", (unsigned char*)src, dest);
    printf("%s\n", dest);
    return 0;
}*/

/**************************************
base64_encode 编码 (加密)
入参:str
出参res, out_len
***************************************/
void db_base64_encode(unsigned char *str, unsigned char *res, int in_len, int *out_len )
{
    int len;
    int str_len;
    int i,j;
//定义base64编码表
    unsigned char *base64_table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//计算经过base64编码后的字符串长度
    str_len=in_len;
    if(str_len % 3 == 0)
        len=str_len/3*4;
    else
        len=(str_len/3+1)*4;
    *out_len = len;
//以3个8位字符为一组进行编码
    for(i=0,j=0;i<len-2;j+=3,i+=4)
    {
        res[i]=base64_table[str[j]>>2]; //取出第一个字符的前6位并找出对应的结果字符
        res[i+1]=base64_table[(str[j]&0x3)<<4 | (str[j+1]>>4)]; //将第一个字符的后位与第二个字符的前4位进行组合并找到对应的结果字符
        res[i+2]=base64_table[(str[j+1]&0xf)<<2 | (str[j+2]>>6)]; //将第二个字符的后4位与第三个字符的前2位组合并找出对应的结果字符
        res[i+3]=base64_table[str[j+2]&0x3f]; //取出第三个字符的后6位并找出结果字符
    }

    switch(str_len % 3)
    {
        case 1:
            res[i-2]='=';
            res[i-1]='=';
            break;
        case 2:
            res[i-1]='=';
            break;
    }
}

void db_base64_decode(unsigned char *code, unsigned char *res, int in_len, long *out_len)
{
//根据base64表，以字符找到对应的十进制数据
    int table[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,62,0,0,0,63,52,53,54,55,56,57,58,59,60,61,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,0,0,0,0,0,0,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};
    long len;
    long str_len;
    int i,j;

//计算解码后的字符串长度
    len=in_len;
//判断编码后的字符串后是否有=
    if(strstr(code,"=="))
        str_len=len/4*3-2;
    else if(strstr(code,"="))
        str_len=len/4*3-1;
    else
        str_len=len/4*3; //??

    *out_len = str_len;

//以4个字符为一位进行解码
    for(i=0,j=0;i < len-2;j+=3,i+=4)
    {
        res[j]=((unsigned char)table[code[i]])<<2 | (((unsigned char)table[code[i+1]])>>4); //取出第一个字符对应base64表的十进制数的前6>位与第二个字符对应base64表的十进制数的后2位进行组合
        res[j+1]=(((unsigned char)table[code[i+1]])<<4) | (((unsigned char)table[code[i+2]])>>2); //取出第二个字符对应base64表的十进制数的后4位与第三个字符对应bas464表的十进制数的后4位进行组合
        res[j+2]=(((unsigned char)table[code[i+2]])<<6) | ((unsigned char)table[code[i+3]]); //取出第三个字符对应base64表的十进制数的后2位与第4个字符进行组合
    }
}
/*****************************************
*
*
*****************************************/
void Date3DesBase64(int in_len, char *in_data, int *out_len, char *out_data,char *key)
{   
    int len_base = 0;
    char sFinaclNetData[256];
    char sFinaclNetDataHex[512];
    char sResultTDes[256];
    char sResultTDesHex[256];
    char sResultBase[256];   
    memset(sFinaclNetData, 0x00, sizeof(sFinaclNetData));
    memset(sResultTDes, 0x00, sizeof(sResultTDes));
    memset(sResultTDesHex, 0x00, sizeof(sResultTDesHex));
    memset(sResultBase, 0x00, sizeof(sResultBase));
    memcpy(sFinaclNetData, in_data, in_len);

    Hex2Str(sFinaclNetData, sFinaclNetDataHex, in_len);
    TDesEcb(key, sFinaclNetDataHex, sResultTDes, 1); // 这个函数的入参是hex的字符串
    Str2Hex(sResultTDes, sResultTDesHex, in_len*2);
    db_base64_encode(sResultTDesHex, sResultBase, in_len, &len_base);        
    *out_len = len_base;
    memcpy(out_data, sResultBase, len_base);    
}

void   encrypt0(unsigned char *text, unsigned char *mtext)
{
    unsigned char ll[64], rr[64], LL[64], RR[64];
    unsigned char tmp[64];
    int i, j;
    ip(text, ll, rr);

    for(i = 1; i < 17; i++)
    {
        F(i, ll, rr, LL, RR);

        for(j = 0; j < 32; j++)
        {
            ll[j] = LL[j];
            rr[j] = RR[j];
        }
    }

    _ip(tmp, rr, ll);
    compress0(tmp, mtext);
}


void   discrypt0(unsigned char *mtext, unsigned char *text)
{
    unsigned char ll[64], rr[64], LL[64], RR[64];
    unsigned char tmp[64];
    int i, j;
    ip(mtext, ll, rr);

    for(i = 16; i > 0; i--)
    {
        F(i, ll, rr, LL, RR);

        for(j = 0; j < 32; j++)
        {
            ll[j] = LL[j];
            rr[j] = RR[j];
        }
    }

    _ip(tmp, rr, ll);
    compress0(tmp, text);
}


void expand0(unsigned char *in, unsigned char *out)
{
    int divide;
    int i, j;

    for(i = 0; i < 8; i++)
    {
        divide = 0x80;

        for(j = 0; j < 8; j++)
        {
            *out++ = (in[i] / divide) & 1;
            divide /= 2;
        }
    }
}


void compress0(unsigned char *out, unsigned char *in)
{
    int times;
    int i, j;

    for(i = 0; i < 8; i++)
    {
        times = 0x80;
        in[i] = 0;

        for(j = 0; j < 8; j++)
        {
            in[i] += (*out++) * times;
            times /= 2;
        }
    }
}

void compress016(unsigned char *out, unsigned char *in)
{
    int times;
    int i, j;

    for(i = 0; i < 16; i++)
    {
        times = 0x8;
        in[i] = '0';

        for(j = 0; j < 4; j++)
        {
            in[i] += (*out++) * times;
            times /= 2;
        }
    }
}

void setkeystar(unsigned char *bits)
{
    int i, j;

    for(i = 0; i < 28; i++)
    {
        CCC[0][i] = bits[pc_1_c[i] - 1];
    }

    for(i = 0; i < 28; i++)
    {
        D[0][i] = bits[pc_1_d[i] - 1];
    }

    for(j = 0; j < 16; j++)
    {
        LS(CCC[j], CCC[j + 1], ls_count[j]);
        LS(D[j], D[j + 1], ls_count[j]);
        son(CCC[j + 1], D[j + 1], K[j + 1]);
    }
}


void LS(unsigned char *bits, unsigned char *buffer, int count)
{
    int i;

    for(i = 0; i < 28; i++)
    {
        buffer[i] = bits[(i + count) % 28];
    }
}

void son(unsigned char *cc, unsigned char *dd, unsigned char *kk)
{
    int i;
    char buffer[56];

    for(i = 0; i < 28; i++)
    {
        buffer[i] = *cc++;
    }

    for(i = 28; i < 56; i++)
    {
        buffer[i] = *dd++;
    }

    for(i = 0; i < 48; i++)
    {
        *kk++ = buffer[pc_2[i] - 1];
    }
}


void ip(unsigned char *text, unsigned char *ll, unsigned char *rr)
{
    int i;
    unsigned char buffer[64];
    expand0(text, buffer);

    for(i = 0; i < 32; i++)
    {
        ll[i] = buffer[ip_tab[i] - 1];
    }

    for(i = 0; i < 32; i++)
    {
        rr[i] = buffer[ip_tab[i + 32] - 1];
    }
}

void _ip(unsigned char *text, unsigned char *ll, unsigned char *rr)
{
    int i;
    char tmp[64];

    for(i = 0; i < 32; i++)
    {
        tmp[i] = ll[i];
    }

    for(i = 32; i < 64; i++)
    {
        tmp[i] = rr[i - 32];
    }

    for(i = 0; i < 64; i++)
    {
        text[i] = tmp[_ip_tab[i] - 1];
    }
}



void F(int n, unsigned char *ll, unsigned char *rr, unsigned char *LL, unsigned char *RR)
{
    int i;
    unsigned char buffer[64], tmp[64];

    for(i = 0; i < 48; i++)
    {
        buffer[i] = rr[e_r[i] - 1];
    }

    for(i = 0; i < 48; i++)
    {
        buffer[i] = (buffer[i] + K[n][i]) & 1;
    }

    s_box(buffer, tmp);

    for(i = 0; i < 32; i++)
    {
        buffer[i] = tmp[P[i] - 1];
    }

    for(i = 0; i < 32; i++)
    {
        RR[i] = (buffer[i] + ll[i]) & 1;
    }

    for(i = 0; i < 32; i++)
    {
        LL[i] = rr[i];
    }
}

void s_box(unsigned char *aa, unsigned char *bb)
{
    int i, j, k, m;
    int y, z;
    unsigned char ss[8];
    m = 0;

    for(i = 0; i < 8; i++)
    {
        j = 6 * i;
        y = aa[j] * 2 + aa[j + 5];
        z = aa[j + 1] * 8 + aa[j + 2] * 4 + aa[j + 3] * 2 + aa[j + 4];
        ss[i] = SSS[i][y][z];
        y = 0x08;

        for(k = 0; k < 4; k++)
        {
            bb[m++] = (ss[i] / y) & 1;
            y /= 2;
        }
    }
}
void  xor11(char *a,  char *b,  char *out, int bytes)
{
    char abuf[9];
    char bbuf[9];
    char outbuf[9];
    char *aa, *bb, *outout;
    memset(abuf, 0, 9);
    memset(bbuf, 0, 9);
    memset(outbuf, 0, 9);
    aschex_to_bcdhex11(a, 16, abuf);
    aschex_to_bcdhex11(b, 16, bbuf);
    aa = abuf;
    bb = bbuf;
    outout = outbuf;

    while(bytes--)
    {
        *outout++ = (*aa++) ^ (*bb++);
    }

    bcdhex_to_aschex11(outbuf, 8, out);
}


#define  KEY "1A2B3C4D5E6F9999"

/*int  main(int argc, char *argv[])
{
    unsigned char r[17];
    unsigned char r1[17];
    unsigned char r2[17];

    unsigned char src[256];
    unsigned char sHex[512];
    unsigned char sDest[512];
    int i;
    int nSrcLen;
    int nHexLen;

    memset(r,0,17);
    memset(r2,0,17);
    memset(src, 0, sizeof(src));
    memset(sHex, 0, sizeof(sHex));
    memset(sDest, 0, sizeof(sDest));

    if (argc < 3)
    {
        printf("Usage:%s  src 0|1\n", argv[0]);
        return 1;
    }

    strcpy(src, argv[1]);
    nSrcLen = strlen(src);
    printf("nSrcLen=%d  src[%s]\n", nSrcLen, src);

    if ( argv[2][0] == '0')
    {
        for(i=0; i< nSrcLen; i++)
            sprintf(sHex+2*i, "%02x", src[i]);

        nHexLen = 2*nSrcLen;
        printf("nHexLen=%d  sHex[%s]\n", nHexLen, sHex);
    }else if ( argv[2][0] == '1')
    {
        nHexLen = nSrcLen;
        memcpy(sHex, src, nHexLen);
    }

    printf("Des KEY ==%s== \n", KEY);
    for(i=0; i<nHexLen; i+=16)
    {
        printf("---------i=%d----------\n", i);
        memset(r, 0, sizeof(r));
        memset(r1, 0, sizeof(r1));

        if ( (nHexLen-i) < 16 )
        {
            memset(r, '0', 16);
            memcpy(r, sHex+i, nHexLen-i);
        }else
            memcpy(r, sHex+i, 16);
        printf("Des ==%s== \n",r);

        if (argv[2][0] == '0' )
            Des(KEY, r, r1);
        else if (argv[2][0] == '1' )
            _Des(KEY, r, r1);

        printf("Des result ==%s== \n",r1);

        memcpy(sDest+i, r1, 16);

        //memset(r2, 0, sizeof(r2));
        //_Des(KEY, r1, r2);
        //printf("Des r2 ==%s== \n",r2);

    }
    printf("sDest[%s]\n", sDest);




    return 0;
}*/

/*
int main (void)
{
  char *data = "646F4D5A969CFDA5";
  int dataLen = strlen (data);
  char data1[30];
  memset (data1,0x00,30);
  int dataLen1 = 0;
  DB_DES (data,dataLen,data1,&dataLen1,1);
  printf ("data1 = %s",data1);
}


void Str2Hex( char *sSrc, char *sDest, int nSrcLen )
{
    int i, nHighBits, nLowBits;

    for( i = 0; i < nSrcLen; i += 2 )
    {
        nHighBits = sSrc[i];
        nLowBits  = sSrc[i + 1];

        if( nHighBits > 0x39 )
            nHighBits -= 0x37;
        else
            nHighBits -= 0x30;

        if( i == nSrcLen - 1 )
            nLowBits = 0;
        else if( nLowBits > 0x39 )
            nLowBits -= 0x37;
        else
            nLowBits -= 0x30;

        sDest[i / 2] = (nHighBits << 4) | (nLowBits & 0x0f);
    }
    return ;
}

*/


/*modify by yuchao 2014/09/01*/
/*
int main(int argc,char * argv[])
{
        int inlen = 0;
        int outlen = 0;
        int flag = 0;
        char inbuf[9] = {'\0'};
        char outbuf[9] = {'\0'};
        if(argc < 5){
            return -1;
        }
        inlen = atoi(argv[1]);
        outlen = atoi(argv[3]);
        flag = atoi(argv[4]);
        memcpy(inbuf,argv[0],sizeof(argv[0])-1);
        DB_DES(&inbuf[0],inlen,&outbuf[0],&outlen,flag);
        return 0;
}
*/
#if 0
int  DB_DES(char *inbuf, int inlen, char *outbuf, int *outlen, int flag)
{
    unsigned char r[17];
    unsigned char r1[17];
    unsigned char r2[17];
    unsigned char src[256];
    unsigned char sHex[512];
    unsigned char sDest[512];
    int i;
    int nSrcLen;
    int nHexLen;
    memset(r, 0, 17);
    memset(r2, 0, 17);
    memset(src, 0, sizeof(src));
    memset(sHex, 0, sizeof(sHex));
    memset(sDest, 0, sizeof(sDest));

    if(inlen > 255)
    {
        return -1;
    }

    memcpy(src, inbuf, inlen);
    nSrcLen = strlen(src);

    if(flag == 0)   /* Encrypt */
    {
        for(i = 0; i < nSrcLen; i++)
        {
            sprintf(sHex + 2 * i, "%02x", src[i]);
        }

        nHexLen = 2 * nSrcLen;
    }
    else if(flag == 1)     /* DisEncrypt */
    {
        nHexLen = nSrcLen;
        memcpy(sHex, src, nHexLen);
    }

    for(i = 0; i < nHexLen; i += 16)
    {
        memset(r, 0, sizeof(r));
        memset(r1, 0, sizeof(r1));

        if((nHexLen - i) < 16)
        {
            memset(r, '0', 16);
            memcpy(r, sHex + i, nHexLen - i);
        }
        else
        {
            memcpy(r, sHex + i, 16);
        }

        if(flag == 0)
        {
            Des(KEY, r, r1);
        }
        else if(flag == 1)
        {
            _Des(KEY, r, r1);
        }

        memcpy(sDest + i, r1, 16);
        //memset(r2, 0, sizeof(r2));
        //_Des(KEY, r1, r2);
        //printf("Des r2 ==%s== \n",r2);
    }

    *outlen = strlen(sDest);
    memcpy(outbuf, sDest, *outlen);

    if(flag == 1)
    {
        memset(outbuf, 0, sizeof(outbuf));
        Str2Hex(sDest, outbuf, *outlen);
        *outlen = *outlen / 2;
    }

    return 0;
}
#endif

#if 0
void Hex2Str(char *sSrc,  char *sDest, int nSrcLen)
{
    int  i;
    char szTmp[3];

    for(i = 0; i < nSrcLen; i++)
    {
        sprintf(szTmp, "%02X", (unsigned char) sSrc[i]);
        memcpy(&sDest[i * 2], szTmp, 2);
    }

    return ;
}

void Str2Hex(char *sSrc, char *sDest, int nSrcLen)
{
    int i, nHighBits, nLowBits;

    for(i = 0; i < nSrcLen; i += 2)
    {
        nHighBits = sSrc[i];
        nLowBits  = sSrc[i + 1];

        if(nHighBits > 0x39)
        {
            nHighBits -= 0x37;
        }
        else
        {
            nHighBits -= 0x30;
        }

        if(i == nSrcLen - 1)
        {
            nLowBits = 0;
        }
        else if(nLowBits > 0x39)
        {
            nLowBits -= 0x37;
        }
        else
        {
            nLowBits -= 0x30;
        }

        sDest[i / 2] = (nHighBits << 4) | (nLowBits & 0x0f);
    }

    return ;
}
#endif

int  DB_DES(char *inbuf, int inlen, char *outbuf, int *outlen, int flag)
{
    char in[32] = {0};
    char out_32[64] = {0};
    char out[32] = {0};
    int len = 0;
    char tmp[32] = {0};
    int i = 0;
    //memcpy(in, inbuf, inlen);

    if(flag == 0)   /* Encrypt */
    {
        if(inlen > 8)
        {
            return -1;
        }

        memcpy(in, inbuf, inlen);
        Hex2Str(in,  tmp, 8);
        Des(KEY, tmp, out);
    }
    else
    {
        /* DisEncrypt */
        if(inlen == 32)
        {
            memcpy(in, inbuf, 16);
            _Des(KEY, in, tmp);
            Str2Hex(tmp,  out, 16);
            memcpy(in, inbuf + 16, 16);
            _Des(KEY, in, tmp);
            Str2Hex(tmp,  out + 8, 16);
            strcpy(outbuf, out);
            *outlen = strlen(outbuf);
            return 0;
            
        }
        else if(inlen != 16)
        {
            return -1;  
        }

        memcpy(in, inbuf, inlen);
        _Des(KEY, in, tmp);
        Str2Hex(tmp,  out, 16);
    }

    strcpy(outbuf, out);
    *outlen = strlen(outbuf);
    return 0;
}


int EncPin(char *pan, int panlen, char *out)
{
    int i = 0;
    char pinblock[32] = {0};
    char panblock[32] = {0};
    char key1[64] = {0};
    char key2[64] = {0};
    char tmp[128] = {0};
    char desin[32] = {0};
    char desout[32] = {0};
    
    strcpy(key1, "D01022D6CA7398B1205A3F86C4C4B8A5");
    strcpy(key2, "BDCC2B43D38B0BFCFA2A20E1B6C611A2");
    memcpy(pinblock, "\x06\x00\x00\x00\xFF\xFF\xFF\xFF", 8);
    memset(tmp, 0x00, sizeof(tmp));

    for(i = 0; i < 12; i++)
    {
        tmp[i] = pan[panlen - 13 + i];
    }

    memset(panblock, 0x00, sizeof(panblock));
    Str2Hex(tmp, &panblock[2], 12);

    for(i = 0; i < 8; i++)
    {
        tmp[i] = pinblock[i] ^ panblock[i];
    }

    Hex2Str(tmp, desin, 8);
#if 0
    TDes(key1, desin, desout);
#else
    TDes(key2, desin, desout);
#endif
    Str2Hex(desout, out, 16);
    return 0;
}
/*
int main ( void )
{
    char out[64] = {0};
    EncPin("1234567890123456", 16, out);
    EncPin("1234567890123456789", 19, out);
    return 0;
}
*/
/*
int main(int argc, char *argv[])
{
    char buf1[256];
    char buf[512];
    int len;
    FILE *fp;
    FILE *fp1;

    memset(buf, 0, sizeof(buf));
    memset(buf1, 0, sizeof(buf1));

    if (argc < 2)
        return 1;

    if ( atoi(argv[1]) == 0)
    {
        fp = fopen("./1", "r");

        fp1 = fopen("./2", "w+");

        fgets(buf1, 256, fp) ;

        printf("len[%d]  buf1[%s]\n", strlen(buf1), buf1);
        buf1[strlen(buf1) -1] = 0;

        DB_DES(buf1, strlen(buf1), buf, &len, 0);

        fputs(buf, fp1);

    }else if ( atoi(argv[1]) == 1)
    {
        fp = fopen("./2", "r");

        fp1 = fopen("./3", "w+");

        fgets(buf1, 256, fp) ;

        printf("len[%d]  buf1[%s]\n", strlen(buf1), buf1);
        buf1[strlen(buf1)] = 0;

        DB_DES(buf1, strlen(buf1), buf, &len, 1);

        fputs(buf, fp1);
    }

    fclose(fp);
    fclose(fp1);


    //DB_DES(argv[1], strlen(argv[1]), buf, &len, 0);

    printf("len[%d]  buf[%s]\n", len, buf);

    return 0;
}
*/

