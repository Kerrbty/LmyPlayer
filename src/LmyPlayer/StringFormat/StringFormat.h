#ifndef _FORMAT_LANGUAGE_STRING_H_
#define _FORMAT_LANGUAGE_STRING_H_
#include "../defs.h"

#define DelString(_a)  { FreeString(_a); _a = NULL; }

// 多字节转宽字节，内存需要自行释放
PWSTR MulToWide( IN LPCSTR str, UINT CodePage = CP_ACP );
PSTR WideToMul( IN LPCWSTR str, UINT CodePage = CP_ACP );

// UTF8 转 窄字节
PSTR UTF8ToANSI( INOUT PSTR str , IN size_t size);
PSTR ANSIToUTF8( INOUT PSTR str , IN size_t size);

LPSTR FindLasteSymbolA(LPSTR CommandLine, CHAR FindWchar); // 查找字符串中给定字符最后出现的位置
LPWSTR FindLasteSymbolW(LPWSTR CommandLine, WCHAR FindWchar); // 查找字符串中给定字符最后出现的位置


BOOL IsTextUTF8(const char* str, ULONG length);  // 判断文本是否为utf-8编码 

VOID FreeString(LPVOID lpBuffer);

#ifdef _UNICODE
#define FindLasteSymbol FindLasteSymbolW
#else
#define FindLasteSymbol FindLasteSymbolA
#endif

class CStringEncoding
{
public:
    enum EncodeType{
        ENCODE_UNKNOW = 0,
        ENCODE_UTF8 = 1,
        ENCODE_GBK  = 2,
    };

    CStringEncoding();
    CStringEncoding(PCSTR szStr, EncodeType type = ENCODE_UNKNOW);
    CStringEncoding(PCWSTR wszStr);

    // 设置新字符串 
    void SetString(PCSTR szStr, EncodeType type = ENCODE_UNKNOW);
    void SetString(PCWSTR wszStr);

    // 获取指定编码字符串 
    PCSTR GetUTF8();
    PCSTR GetGBK();
    PCWSTR GetUTF16();

    ~CStringEncoding();
protected:
    VOID CleanString();     // 清除现有字符串 
    VOID Init();

private:
    PSTR m_utf8;        // UTF8 编码 
    PSTR m_gbk;         // GBK 编码 
    PWSTR m_utf16;      // UTF16 编码 
};

#endif  // _FORMAT_LANGUAGE_STRING_H_