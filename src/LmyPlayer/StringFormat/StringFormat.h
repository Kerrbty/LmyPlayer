#ifndef _FORMAT_LANGUAGE_STRING_H_
#define _FORMAT_LANGUAGE_STRING_H_
#include "../defs.h"

#define DelString(_a)  { FreeString(_a); _a = NULL; }

// ���ֽ�ת���ֽڣ��ڴ���Ҫ�����ͷ�
PWSTR MulToWide( IN LPCSTR str, UINT CodePage = CP_ACP );
PSTR WideToMul( IN LPCWSTR str, UINT CodePage = CP_ACP );

// UTF8 ת խ�ֽ�
PSTR UTF8ToANSI( INOUT PSTR str , IN size_t size);
PSTR ANSIToUTF8( INOUT PSTR str , IN size_t size);

LPSTR FindLasteSymbolA(LPSTR CommandLine, CHAR FindWchar); // �����ַ����и����ַ������ֵ�λ��
LPWSTR FindLasteSymbolW(LPWSTR CommandLine, WCHAR FindWchar); // �����ַ����и����ַ������ֵ�λ��


BOOL IsTextUTF8(const char* str, ULONG length);  // �ж��ı��Ƿ�Ϊutf-8���� 

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

    // �������ַ��� 
    void SetString(PCSTR szStr, EncodeType type = ENCODE_UNKNOW);
    void SetString(PCWSTR wszStr);

    // ��ȡָ�������ַ��� 
    PCSTR GetUTF8();
    PCSTR GetGBK();
    PCWSTR GetUTF16();

    ~CStringEncoding();
protected:
    VOID CleanString();     // ��������ַ��� 
    VOID Init();

private:
    PSTR m_utf8;        // UTF8 ���� 
    PSTR m_gbk;         // GBK ���� 
    PWSTR m_utf16;      // UTF16 ���� 
};

#endif  // _FORMAT_LANGUAGE_STRING_H_