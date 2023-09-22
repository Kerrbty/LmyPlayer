// CommonDef.h ͨ�õ�windows�궨�� 
// 
#ifndef _ZXLY_COMMAND_DEFINES_HEADER_H_
#define _ZXLY_COMMAND_DEFINES_HEADER_H_
#ifdef __cplusplus
#define EXTERN_C   extern "C"
#else  // __cplusplus 
#define EXTERN_C
#endif // __cplusplus 

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#ifdef _UNICODE
#define _tstring std::wstring
#else
#define _tstring std::string
#endif

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
#ifndef INOUT
#define INOUT
#endif

#ifdef _DEBUG
#define CHECK_MEMORY   // У�黺�����������,DEBUG�汾Ĭ�ϴ� 
#define MEMORY_ADD_SIZE   0x100   // �����Ϊ 0x100 �ֽ��ڲ����ƻ����򣬵����Լ�⵽��� 
#endif // _DEBUG

// �ڴ��������� 
#define HEAPMEMEOY      0x01
#define MALLOCMEMEOY    0x02

// ��־λ 
#define MEMOEY_FLAGS    0xA5
#ifndef MEMORY_ADD_SIZE // ����������岿�� 
#define MEMORY_ADD_SIZE     0x00
#endif 


#pragma pack(push,1)
typedef struct _dynamic_memory 
{
    SIZE_T heap_size;
    unsigned int  heap_type;
    // ��Ȼ����heap������ heap_type ֮��
    // ����˼��ʵ�����������������ĩβ�ֽ� 
#ifdef CHECK_MEMORY
    // ����һЩ����������ڿ���������ʱ����԰�����ĳ��ȼӴ������ھ�׼����� 
    unsigned char heap[4+MEMORY_ADD_SIZE];
#else
    // �������ֽ�Ϊ�˵��� ReAllocMemory ʱ���ڴ��ɴ�С������ʱ���β�ǲ���0Ϊ��β�ģ���ʱ����Ҫ���TEXT('\0') 
    unsigned char heap[2+MEMORY_ADD_SIZE];
#endif
}dynamic_memory, *pdynamic_memory;
#pragma pack(pop)

#ifdef CHECK_MEMORY
#include <crtdbg.h>
// ����ڴ��Ƿ���� 
static __forceinline VOID ASSERT_MEMORY_OVERFLOW(LPVOID _addr, const char* filename, int iline, const char* message) 
{
    if (_addr != NULL)
    {
        pdynamic_memory lpBuf = (pdynamic_memory)CONTAINING_RECORD(_addr, dynamic_memory, heap);
        unsigned char* lpcheckbuf = lpBuf->heap + lpBuf->heap_size;
        // �ֽڱȽ�û�д�С������ 
        if (lpcheckbuf[0] != 0x00 || lpcheckbuf[1] != 0x00 ||
            lpcheckbuf[2] != MEMOEY_FLAGS || lpcheckbuf[3] != MEMOEY_FLAGS)
        {
            // ���������,��һʱ����Բ��� 
            // (���������ϵͳHeap����,�漰���ܶ�API��������쳣,���Ե������ȴ�C��������) 
#ifdef _DEBUG
            _CrtDbgReport(_CRT_ASSERT,
                filename, 
                iline,
                NULL,
                message);
            if (IsDebuggerPresent())
            {
                // �е������򴥷��ϵ� 
                DebugBreak();
            }
#else
            char linenumber[24];
            sprintf(linenumber, "Lines: %u", iline);;
            int ret = MessageBoxA(NULL, 
                filename, 
                linenumber, 
                MB_TASKMODAL|MB_ICONHAND|MB_ABORTRETRYIGNORE|MB_SETFOREGROUND);
            switch(ret)
            {
            case IDABORT:
                if (IsDebuggerPresent())
                {
                    DebugBreak();
                }
                else
                {
                    ExitProcess(EXIT_FAILURE);
                }
                break;
            case IDRETRY:
                break;
            case IDIGNORE:
                break;
            default:
                break;
            }
#endif
        }
    }
}
#else // CHECK_MEMORY
#define ASSERT_MEMORY_OVERFLOW(...)
#endif // CHECK_MEMORY
#define ASSERT_HEAP(addr)  ASSERT_MEMORY_OVERFLOW(addr, __FILE__, __LINE__, "Memory Overflow ( Build: "__DATE__" / "__TIME__" )\r\n\r\n\t\t o(�s���t)o")
#define ASSERT_HEAP_WITH_MSG(addr, msg)  ASSERT_MEMORY_OVERFLOW(addr, __FILE__, __LINE__, msg)


// �����ڴ�,��ȫ���� 
static __forceinline LPVOID AllocMemory(SIZE_T _nsize) 
{
    pdynamic_memory lpBuf = (pdynamic_memory)
        HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, _nsize+sizeof(dynamic_memory)); 
    if (lpBuf == NULL)
    {
        // �ڴ�����ʧ�ܣ������������� 
        lpBuf = (pdynamic_memory)malloc(_nsize+sizeof(dynamic_memory));
        if (lpBuf != NULL)
        {
            lpBuf->heap_type = MALLOCMEMEOY;
            memset(lpBuf->heap, 0, _nsize+sizeof(lpBuf->heap));
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        lpBuf->heap_type = HEAPMEMEOY; 
    }
    lpBuf->heap_size = _nsize;
#ifdef CHECK_MEMORY
    // ���4���ֽڣ�ǰ�����ֽ���Ϊ���ַ��Ľ�����(�Է�һЩ��������ӽ�����),�������ַ�����У����� 
    memset(lpBuf->heap+_nsize, 0, sizeof(wchar_t));
    memset(lpBuf->heap+_nsize+sizeof(wchar_t), MEMOEY_FLAGS, 2);
#endif
    // �����ڴ�й©���ڷ���ǰ���������б��¼��������Ҫ��ʱ����ɨ����Щû���ͷ� 
    // ... 
    return lpBuf->heap;
}

// �ͷ��ڴ� 
#define FreeMemory(addr)   {FreeMemory_stub(addr); addr = NULL;} 
static __forceinline VOID FreeMemory_stub(LPVOID _addr)
{
    if (_addr != NULL)
    {
        ASSERT_HEAP_WITH_MSG(_addr, "FreeMemory( ) to check Overflow"); // ����������������ͷ��ڴ��Զ���� 
        pdynamic_memory lpBuf = (pdynamic_memory)CONTAINING_RECORD(_addr, dynamic_memory,heap);
        switch(lpBuf->heap_type)
        {
        case HEAPMEMEOY:
            HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, lpBuf);
            break;
        case MALLOCMEMEOY:
            free(lpBuf);
            break;
        }
    }
}

// ԭ������������������ڴ� ��Ҳ�����������ڴ�_addrΪNULL) 
static __forceinline LPVOID ReAllocMemory(LPVOID _addr, SIZE_T _nsize)
{
    if (_addr == NULL)
    {
        return AllocMemory(_nsize);
    }

    // ����ϵͳ������������ 
    pdynamic_memory lpNewBuf = NULL;
    pdynamic_memory lpBuf = (pdynamic_memory)CONTAINING_RECORD(_addr, dynamic_memory,heap);
    switch(lpBuf->heap_type)
    {
    case HEAPMEMEOY:
        lpNewBuf = (pdynamic_memory)HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, lpBuf, _nsize+sizeof(dynamic_memory));
        break;
    case MALLOCMEMEOY:
        lpNewBuf = (pdynamic_memory)realloc(lpBuf, _nsize+sizeof(dynamic_memory));
        if (lpNewBuf != NULL && _nsize > lpNewBuf->heap_size)
        {
            memset(lpNewBuf->heap+lpNewBuf->heap_size, 0, _nsize-lpNewBuf->heap_size);
        }
        break;
    }

    // ϵͳ��������ʧ�ܣ������Լ����뿽�� 
    if (lpNewBuf == NULL)
    {
        LPBYTE tmpbuf = (LPBYTE)AllocMemory(_nsize);
        if (tmpbuf)
        {
            lpNewBuf = (pdynamic_memory)CONTAINING_RECORD(tmpbuf, dynamic_memory,heap);
        }

        if (lpNewBuf)
        {
            // ��ԭ�������ݿ������������ͷ��ڴ� 
            memcpy(lpNewBuf->heap, _addr, lpBuf->heap_size>_nsize?_nsize:lpBuf->heap_size);
            // �ͷ�֮ǰ��buffer 
            FreeMemory(_addr);
        }
        else
        {
            FreeMemory(_addr);
            return NULL;
        }
    }

    lpBuf = lpNewBuf;
    lpBuf->heap_size = _nsize;
#ifdef CHECK_MEMORY
    // ���4���ֽڣ�ǰ�����ֽ���Ϊ���ַ��Ľ�����(�Է�һЩ��������ӽ�����),�������ַ�����У����� 
    memset(lpBuf->heap+_nsize, 0, sizeof(wchar_t));
    memset(lpBuf->heap+_nsize+sizeof(wchar_t), MEMOEY_FLAGS, 2);
#endif
    return lpBuf->heap;
}

// ���԰�ı����ļ� 
static inline BOOL BackFileW(LPCWSTR lpFileName)
{
    BOOL bret = FALSE;
    if (lpFileName == NULL)
    {
        return bret;
    }
    LPWSTR lpBackName = (LPWSTR)AllocMemory((wcslen(lpFileName)+5)*sizeof(WCHAR));
    if (lpBackName)
    {
        wsprintfW(lpBackName, L"%s.bak", lpFileName);
        bret = CopyFileW(lpFileName, lpBackName, TRUE);
        FreeMemory(lpBackName);
    }
    return bret;
}

static inline BOOL BackFileA(LPCSTR lpFileName)
{
    BOOL bret = FALSE;
    if (lpFileName == NULL)
    {
        return bret;
    }
    LPSTR lpBackName = (LPSTR)AllocMemory((strlen(lpFileName)+5)*sizeof(CHAR));
    if (lpBackName)
    {
        wsprintfA(lpBackName, "%s.bak", lpFileName);
        bret = CopyFileA(lpFileName, lpBackName, TRUE);
        FreeMemory(lpBackName);
    }
    return bret;
}

#define DeleteHandle(_handle) {if(_handle != INVALID_HANDLE_VALUE) { CloseHandle(_handle); _handle = INVALID_HANDLE_VALUE;}} // �رվ�� 

// ���ļ� 
#define FileOpenA(_name, _openf, _sharef, _createf)  CreateFileA(_name, _openf, _sharef, NULL, _createf, FILE_ATTRIBUTE_NORMAL, NULL)  
#define FileOpenW(_name, _openf, _sharef, _createf)  CreateFileW(_name, _openf, _sharef, NULL, _createf, FILE_ATTRIBUTE_NORMAL, NULL)

#ifdef _UNICODE
#define FileOpen FileOpenW
#define BackFile BackFileW
#else /* _UNICODE */
#define FileOpen FileOpenA 
#define BackFile BackFileA
#endif /* _UNICODE */


#if _MSC_VER < 1400 
EXTERN_C BOOL WINAPI GetFileSizeEx(HANDLE, PLARGE_INTEGER);
EXTERN_C BOOL WINAPI SetFilePointerEx(HANDLE, LARGE_INTEGER, PLARGE_INTEGER,DWORD);
#endif /* _MSC_VER */ 


#endif /* _ZXLY_COMMAND_DEFINES_HEADER_H_ */ 