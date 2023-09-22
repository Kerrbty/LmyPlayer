#include "StringFormat.h"
#include <crtdbg.h>

// UTF8 转 GBK 
PSTR UTF8ToANSI( INOUT PSTR str , IN size_t size)
{
    LPWSTR  pElementText = MulToWide(str, CP_UTF8);
    if (pElementText)
    {
        LPSTR pResult = WideToMul(pElementText, CP_ACP);
        if (pResult)
        {
            int len = strlen(pResult);
            if (len > size)
            {
                len = size;
            }
            memcpy(str, pResult, len);
            str[len] = '\0';

            FreeString(pResult);
        }
        FreeString(pElementText);
    }
    return str;
}

// GBK 转 UTF8 
PSTR ANSIToUTF8( INOUT PSTR str , IN size_t size)
{
    LPWSTR  pElementText = MulToWide(str, CP_ACP);
    if (pElementText)
    {
        LPSTR pResult = WideToMul(pElementText, CP_UTF8);
        if (pResult)
        {
            int len = strlen(pResult);
            if (len > size)
            {
                len = size;
            }
            memcpy(str, pResult, len);
            str[len] = '\0';

            FreeString(pResult);
        }
        FreeString(pElementText);
    }
    return str;
}

// GBK转UTF16 
PWSTR MulToWide( LPCSTR str, UINT CodePage )
{
    PWSTR  pElementText;
    int    iTextLen;

    iTextLen = MultiByteToWideChar( CodePage,
        0,
        (PCHAR)str,
        -1,
        NULL,
        0 );
    
    pElementText = 
        (PWSTR)AllocMemory((iTextLen+1)*sizeof(WCHAR));
    _ASSERT(pElementText);
    if (pElementText)
    {
        MultiByteToWideChar( CodePage,
            0,
            (PCHAR)str,
            -1,
            pElementText,
            iTextLen );
    }

    return pElementText;
}

// UTF16 转 GBK 
PSTR WideToMul( LPCWSTR str, UINT CodePage )
{
    PSTR  pElementText;
    int    iTextLen;
    
    iTextLen = WideCharToMultiByte( CodePage,
        0,
        str,
        -1,
        NULL,
        0,
        NULL,
        NULL);
    
    pElementText = 
        (PSTR)AllocMemory( iTextLen+1 );
    _ASSERT(pElementText);
    if (pElementText)
    {
        WideCharToMultiByte( CodePage,
            0,
            str,
            -1,
            pElementText,
            iTextLen,
            NULL,
            NULL);
    }

    return pElementText;
}

// 查找字符串中给定字符最后出现的位置
LPSTR FindLasteSymbolA(LPSTR CommandLine, CHAR FindWchar)
{
    int Len;
    for ( Len = strlen(CommandLine) ; Len>0; Len-- )
    {
        if (CommandLine[Len] == FindWchar)
        {
            Len++;
            break;
        }
    }
    return &CommandLine[Len];
}

// 查找字符串中给定字符最后出现的位置
LPWSTR FindLasteSymbolW(LPWSTR CommandLine, WCHAR FindWchar)
{
    int Len;
    for ( Len = wcslen(CommandLine) ; Len>0; Len-- )
    {
        if (CommandLine[Len] == FindWchar)
        {
            Len++;
            break;
        }
    }
    return &CommandLine[Len];
}


BOOL IsTextUTF8(const char* str, ULONG length)
{
    DWORD nBytes=0;//UFT8可用1-6个字节编码,ASCII用一个字节
    UCHAR chr;
    BOOL bAllAscii=TRUE; //如果全部都是ASCII, 说明不是UTF-8
    for(int i=0; i<length; ++i)
    {
        chr= *(str+i);
        if( (chr&0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
            bAllAscii= FALSE;
        if(nBytes==0) //如果不是ASCII码,应该是多字节符,计算字节数
        {
            if(chr>=0x80)
            {
                if(chr>=0xFC&&chr<=0xFD)
                    nBytes=6;
                else if(chr>=0xF8)
                    nBytes=5;
                else if(chr>=0xF0)
                    nBytes=4;
                else if(chr>=0xE0)
                    nBytes=3;
                else if(chr>=0xC0)
                    nBytes=2;
                else
                    return FALSE;

                nBytes--;
            }
        }
        else //多字节符的非首字节,应为 10xxxxxx
        {
            if( (chr&0xC0) != 0x80 )
                return FALSE;

            nBytes--;
        }
    }
    if( nBytes > 0 ) //违返规则
        return FALSE;
    if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
        return FALSE;
    return TRUE;
}

VOID FreeString(LPVOID lpBuffer)
{
    FreeMemory(lpBuffer);
}

//////////////////////////////////////////////////////////////////////////
VOID CStringEncoding::Init()
{
    m_utf8 = NULL;
    m_gbk = NULL;
    m_utf16 = NULL;
}

CStringEncoding::CStringEncoding()
{
    Init();
}

CStringEncoding::CStringEncoding(PCSTR szStr, EncodeType type)
{
    Init();
    SetString(szStr, type);
}

CStringEncoding::CStringEncoding(PCWSTR wszStr)
{
    Init();
    SetString(wszStr);
}

// 设置新字符串 
void CStringEncoding::SetString(PCSTR szStr, EncodeType type)
{
    CleanString();
    if (!szStr)
    {
        return;
    }

    // 如果没有设置传入字符串格式，自己判断 
    size_t ilen = strlen(szStr);
    if (type == ENCODE_UNKNOW)
    {
        if ( IsTextUTF8(szStr, ilen) )
        {
            type = ENCODE_UTF8;
        }
        else
        {
            // 默认GBK 
            type = ENCODE_GBK;
        }
    }

    // 复制到对应的成员里面 
    char* szCopyStr = (char*)AllocMemory((ilen+1)*sizeof(char));
    if (szCopyStr)
    {
        strcpy(szCopyStr, szStr);
        switch(type)
        {
        case ENCODE_UTF8:
            m_utf8 = szCopyStr;
            break;
        case ENCODE_GBK:
            m_gbk = szCopyStr;
            break;
        default:
            FreeMemory(szCopyStr);
            break;
        }
    }
}

void CStringEncoding::SetString(PCWSTR wszStr)
{
    CleanString();
    if (wszStr)
    {
        size_t len = wcslen(wszStr);
        m_utf16 = (LPWSTR)AllocMemory( (len+1)*sizeof(WCHAR) );
        if (m_utf16)
        {
            wcscpy(m_utf16, wszStr);
        }
    }
}

// 获取指定编码字符串 
PCSTR CStringEncoding::GetUTF8()
{
    if (m_utf8)
    {
        // 已经有当前格式 
        return m_utf8;
    }

    if (m_utf16)
    {
        // 比较好的情况，转换一次 
        m_utf8 = WideToMul(m_utf16, CP_UTF8);
    }
    else if (m_gbk)
    {
        // 情况比较差 
        m_utf16 = MulToWide(m_gbk, CP_ACP);
        if (m_utf16)
        {
            m_utf8 = WideToMul(m_utf16, CP_UTF8);
        }
    }
    return m_utf8;
}

PCSTR CStringEncoding::GetGBK()
{
    if (m_gbk)
    {
        // 已经有当前格式 
        return m_gbk;
    }

    if (m_utf16)
    {
        // 比较好的情况，转换一次 
        m_gbk = WideToMul(m_utf16, CP_ACP);
    }
    else if (m_utf8)
    {
        // 情况比较差 
        m_utf16 = MulToWide(m_utf8, CP_UTF8);
        if (m_utf16)
        {
            m_gbk = WideToMul(m_utf16, CP_ACP);
        }
    }
    return m_gbk;
}

PCWSTR CStringEncoding::GetUTF16()
{
    if (m_utf16)
    {
        // 已经有当前格式 
        return m_utf16;
    }

    if (m_gbk)
    {
        m_utf16 = MulToWide(m_gbk, CP_ACP);
    }
    else if (m_utf8)
    {
        m_utf16 = MulToWide(m_utf8, CP_UTF8);
    }
    return m_utf16;
}

CStringEncoding::~CStringEncoding()
{
    CleanString();
}

// 清除数据 
VOID CStringEncoding::CleanString()
{
    if (m_utf8)
    {
        FreeMemory(m_utf8);
        m_utf8 = NULL;
    }
    if (m_gbk)
    {
        FreeMemory(m_gbk);
        m_gbk = NULL;
    }
    if (m_utf16)
    {
        FreeMemory(m_utf16);
        m_utf16 = NULL;
    }
}