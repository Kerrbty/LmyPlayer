// CommonDef.h 通用的windows宏定义 
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
#define CHECK_MEMORY   // 校验缓冲区溢出开关,DEBUG版本默认打开 
#define MEMORY_ADD_SIZE   0x100   // 在溢出为 0x100 字节内不会破坏程序，但可以检测到溢出 
#endif // _DEBUG

// 内存申请类型 
#define HEAPMEMEOY      0x01
#define MALLOCMEMEOY    0x02

// 标志位 
#define MEMOEY_FLAGS    0xA5
#ifndef MEMORY_ADD_SIZE // 用于溢出缓冲部分 
#define MEMORY_ADD_SIZE     0x00
#endif 


#pragma pack(push,1)
typedef struct _dynamic_memory 
{
    SIZE_T heap_size;
    unsigned int  heap_type;
    // 虽然下面heap定义在 heap_type 之后
    // 但意思其实是在整个柔性数组的末尾字节 
#ifdef CHECK_MEMORY
    // 对于一些特殊情况，在开启检测溢出时候可以把数组的长度加大，有利于精准查溢出 
    unsigned char heap[4+MEMORY_ADD_SIZE];
#else
    // 这两个字节为了调用 ReAllocMemory 时，内存由大到小缩减的时候结尾是不以0为结尾的，这时候需要这个TEXT('\0') 
    unsigned char heap[2+MEMORY_ADD_SIZE];
#endif
}dynamic_memory, *pdynamic_memory;
#pragma pack(pop)

#ifdef CHECK_MEMORY
#include <crtdbg.h>
// 检测内存是否溢出 
static __forceinline VOID ASSERT_MEMORY_OVERFLOW(LPVOID _addr, const char* filename, int iline, const char* message) 
{
    if (_addr != NULL)
    {
        pdynamic_memory lpBuf = (pdynamic_memory)CONTAINING_RECORD(_addr, dynamic_memory, heap);
        unsigned char* lpcheckbuf = lpBuf->heap + lpBuf->heap_size;
        // 字节比较没有大小端问题 
        if (lpcheckbuf[0] != 0x00 || lpcheckbuf[1] != 0x00 ||
            lpcheckbuf[2] != MEMOEY_FLAGS || lpcheckbuf[3] != MEMOEY_FLAGS)
        {
            // 发生了溢出,第一时间可以捕获 
            // (如果这里是系统Heap错误,涉及到很多API都会出现异常,可以调整优先从C堆中申请) 
#ifdef _DEBUG
            _CrtDbgReport(_CRT_ASSERT,
                filename, 
                iline,
                NULL,
                message);
            if (IsDebuggerPresent())
            {
                // 有调试器则触发断点 
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
#define ASSERT_HEAP(addr)  ASSERT_MEMORY_OVERFLOW(addr, __FILE__, __LINE__, "Memory Overflow ( Build: "__DATE__" / "__TIME__" )\r\n\r\n\t\t o(╯□╰)o")
#define ASSERT_HEAP_WITH_MSG(addr, msg)  ASSERT_MEMORY_OVERFLOW(addr, __FILE__, __LINE__, msg)


// 申请内存,并全置零 
static __forceinline LPVOID AllocMemory(SIZE_T _nsize) 
{
    pdynamic_memory lpBuf = (pdynamic_memory)
        HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, _nsize+sizeof(dynamic_memory)); 
    if (lpBuf == NULL)
    {
        // 内存申请失败，换个函数申请 
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
    // 最后4个字节，前两个字节作为宽字符的结束符(以防一些函数不会加结束符),后两个字符用于校验溢出 
    memset(lpBuf->heap+_nsize, 0, sizeof(wchar_t));
    memset(lpBuf->heap+_nsize+sizeof(wchar_t), MEMOEY_FLAGS, 2);
#endif
    // 对于内存泄漏，在返回前可以先用列表记录起来，需要的时候再扫描哪些没有释放 
    // ... 
    return lpBuf->heap;
}

// 释放内存 
#define FreeMemory(addr)   {FreeMemory_stub(addr); addr = NULL;} 
static __forceinline VOID FreeMemory_stub(LPVOID _addr)
{
    if (_addr != NULL)
    {
        ASSERT_HEAP_WITH_MSG(_addr, "FreeMemory( ) to check Overflow"); // 如果开启检测溢出，释放内存自动检测 
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

// 原基础上扩充或者缩减内存 （也可用于申请内存_addr为NULL) 
static __forceinline LPVOID ReAllocMemory(LPVOID _addr, SIZE_T _nsize)
{
    if (_addr == NULL)
    {
        return AllocMemory(_nsize);
    }

    // 利用系统函数重新申请 
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

    // 系统重新申请失败，试试自己申请拷贝 
    if (lpNewBuf == NULL)
    {
        LPBYTE tmpbuf = (LPBYTE)AllocMemory(_nsize);
        if (tmpbuf)
        {
            lpNewBuf = (pdynamic_memory)CONTAINING_RECORD(tmpbuf, dynamic_memory,heap);
        }

        if (lpNewBuf)
        {
            // 把原来的数据拷贝过来，再释放内存 
            memcpy(lpNewBuf->heap, _addr, lpBuf->heap_size>_nsize?_nsize:lpBuf->heap_size);
            // 释放之前的buffer 
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
    // 最后4个字节，前两个字节作为宽字符的结束符(以防一些函数不会加结束符),后两个字符用于校验溢出 
    memset(lpBuf->heap+_nsize, 0, sizeof(wchar_t));
    memset(lpBuf->heap+_nsize+sizeof(wchar_t), MEMOEY_FLAGS, 2);
#endif
    return lpBuf->heap;
}

// 粗略版的备份文件 
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

#define DeleteHandle(_handle) {if(_handle != INVALID_HANDLE_VALUE) { CloseHandle(_handle); _handle = INVALID_HANDLE_VALUE;}} // 关闭句柄 

// 打开文件 
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