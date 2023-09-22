#include "common.h"
#include "defs.h"
#include <Shlwapi.h>
#include <Wincrypt.h>
#include <string>
#include <sstream>
#include "jsoncpp/json/json.h"
#include "sqlite3.18/sqlite3.h"
#include "Http/Http.h"
#pragma comment(lib, "Advapi32")

#define DB_LENGHT  50

static BOOL CaclBufferMd5(PBYTE pbuf, DWORD dwlen, PBYTE Md5Val)
{
    BOOL retval = FALSE;
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    DWORD error = 0;
    do
    {
        if( !CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) ) 
        {
            break;
        }
        CryptCreateHash(hProv, CALG_MD5, NULL, 0, &hHash);
        DWORD ilen = 4*sizeof(DWORD);
        if( CryptHashData(hHash, pbuf, dwlen, 0) && 
            CryptGetHashParam(hHash, HP_HASHVAL, (PBYTE)Md5Val, &ilen, 0) )
        {
            retval = TRUE;
        }
    }while(0);

    if(hHash) 
        CryptDestroyHash(hHash);
    if(hProv) 
        CryptReleaseContext(hProv, 0);
    return retval;
}


BOOL CalcMd5(PBYTE pbuf, DWORD dwlen, LPTSTR res)
{
    BYTE pMd5Buf[16];
    if ( CaclBufferMd5(pbuf, dwlen, pMd5Buf) )
    {
        for (int i=0; i<16; i++)
        {
            wsprintf(res+i*2, TEXT("%02X"), pMd5Buf[i]);
        }
        return TRUE;
    }
    return FALSE;
}

static PWSTR WINAPI MultiToWide(LPCSTR str, UINT CodePage = CP_ACP)
{
    PWSTR  pElementText;
    int    iTextLen;

    iTextLen = MultiByteToWideChar(CodePage,
        0,
        (PCHAR)str,
        -1,
        NULL,
        0);

    pElementText =
        (PWSTR)new WCHAR[iTextLen + 1];
    if (pElementText)
    {
        MultiByteToWideChar(CodePage,
            0,
            (PCHAR)str,
            -1,
            pElementText,
            iTextLen);
        return pElementText;
    }
    return NULL;
}

static PSTR WINAPI WideToMulti(LPCWSTR str, UINT CodePage = CP_ACP)
{
    PSTR  pElementText;
    int    iTextLen;

    iTextLen = WideCharToMultiByte(CodePage,
        0,
        str,
        -1,
        NULL,
        0,
        NULL,
        NULL);

    pElementText =
        (PSTR)new char[iTextLen + 1];
    if (pElementText)
    {
        WideCharToMultiByte(CodePage,
            0,
            str,
            -1,
            pElementText,
            iTextLen,
            NULL,
            NULL);
        return pElementText;
    }
    return NULL;
}

std::string utf8_2_gbk(std::string& str)
{
    std::string gbkstr;
    PWSTR new_utf16 = MultiToWide(str.c_str(), CP_UTF8);
    if (new_utf16)
    {
        PSTR new_gbk = WideToMulti(new_utf16, CP_ACP);
        if (new_gbk)
        {
            gbkstr = new_gbk;
            delete[]new_gbk;
        }
        delete[]new_utf16;
    }
    return gbkstr;
}

std::wstring utf8_2_utf16(std::string& str)
{
    std::wstring utf16str;
    PWSTR new_utf16 = MultiToWide(str.c_str(), CP_UTF8);
    if (new_utf16)
    {
        utf16str = new_utf16;
        delete[]new_utf16;
    }
    return utf16str;
}

std::string gbk_2_utf8(std::string& str)
{
    std::string utf8str;
    PWSTR new_utf16 = MultiToWide(str.c_str(), CP_ACP);
    if (new_utf16)
    {
        PSTR new_utf8 = WideToMulti(new_utf16, CP_UTF8);
        if (new_utf8)
        {
            utf8str = new_utf8;
            delete[]new_utf8;
        }
        delete[]new_utf16;
    }
    return utf8str;
}

std::string utf16_2_utf8(std::wstring& wstr)
{
    std::string gbkstr;
    PSTR new_gbk = WideToMulti(wstr.c_str(), CP_UTF8);
    if (new_gbk)
    {
        gbkstr = new_gbk;
        delete[]new_gbk;
    }
    return gbkstr;
}

std::string utf16_2_gbk(std::wstring& wstr)
{
    std::string gbkstr;
    PSTR new_gbk = WideToMulti(wstr.c_str(), CP_ACP);
    if (new_gbk)
    {
        gbkstr = new_gbk;
        delete[]new_gbk;
    }
    return gbkstr;
}


VOID FreeComboData(PComboPlayList combo)
{
    if (combo == NULL)
    {
        return;
    }

    while( !_IsListEmpty(&combo->next) )
    {
        PLIST_ENTRY list = _RemoveTailList(&combo->next);
        PComboPlayList subitem = CONTAINING_RECORD(list, ComboPlayList, next);
        delete subitem;
    }
    delete combo;
}

// 本地 playlist 文件 
static PComboPlayList GetLocalComboData()
{
    PComboPlayList combo = new ComboPlayList;
    do
    {
        if (combo == NULL)
        {
            break;
        }

        // 第一个数据空着 
        memset(combo, 0, sizeof(ComboPlayList));
        _InitializeListHead(&combo->next);
        combo->nIndex = -1;

        // 搜索本地数据 
        TCHAR* filename = new TCHAR[MAX_PATH*2];
        if (filename == NULL)
        {
            break;
        }

        GetModuleFileName(GetModuleHandle(NULL), filename, MAX_PATH);
        PathRemoveFileSpec(filename);
        PathAddBackslash(filename);
        _tcscat(filename, TEXT("playlist"));
        PathAddBackslash(filename);

        DWORD len = _tcslen(filename);
        _tcscpy(filename+len, TEXT("*"));
        // 枚举 
        WIN32_FIND_DATA FindFileData = {0};
        HANDLE hFindFile = FindFirstFile(filename, &FindFileData);
        if (hFindFile != INVALID_HANDLE_VALUE)
        {
            do 
            {
                if (
                    _tcsicmp(FindFileData.cFileName, TEXT(".")) == 0 ||
                    _tcsicmp(FindFileData.cFileName, TEXT("..")) == 0
                    )
                {
                    continue;
                }
                else if ( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
                {
                    PComboPlayList item = new ComboPlayList();
                    if (item)
                    {
                        memset(item, 0, sizeof(ComboPlayList));
                        _tcscpy(item->Name, FindFileData.cFileName);
                        PathRemoveExtension(item->Name);
                        _tcscpy(filename+len, FindFileData.cFileName);
                        _tcscpy(item->szPath, filename);
                        _InsertTailList(&combo->next, &item->next);
                    }
                }
            } while ( FindNextFile(hFindFile, &FindFileData) );
            FindClose(hFindFile);
        }

        delete []filename;
    }while(FALSE);

    return combo;
}

// config.db 记录的网络列表 
static PComboPlayList GetConfigComboData()
{
    PComboPlayList combo = new ComboPlayList;
    do 
    {
        if (combo == NULL)
        {
            break;
        }

        // 第一个数据空着 
        memset(combo, 0, sizeof(ComboPlayList));
        _InitializeListHead(&combo->next);
        combo->nIndex = -1;

        // 配置文件解析utf-8  
        TCHAR* filename = new TCHAR[MAX_PATH*2];
        if (filename == NULL)
        {
            break;
        }

        GetModuleFileName(GetModuleHandle(NULL), filename, MAX_PATH);
        PathRemoveFileSpec(filename);
        PathAddBackslash(filename);
        _tcscat(filename, TEXT("config.db"));

        HANDLE hFile = CreateFile( 
            filename, 
            GENERIC_READ, 
            FILE_SHARE_READ, 
            NULL, 
            OPEN_EXISTING, 
            FILE_ATTRIBUTE_NORMAL, 
            NULL);
        if (hFile != INVALID_HANDLE_VALUE)
        {
            // 读取文件 
            DWORD dwBytes = 0;
            DWORD dwSize = GetFileSize(hFile, NULL);
            unsigned char* JsonBuf = new unsigned char[dwSize+2];
            memset(JsonBuf, 0, dwSize+2);
            SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
            ReadFile(hFile, JsonBuf, dwSize, &dwBytes, NULL);
            CloseHandle(hFile);

            // 解析 
            Json::Reader reader;
            Json::Value jvRoot;
            if (reader.parse((char*)JsonBuf, jvRoot))
            {
                Json::Value jvNetList = jvRoot["netlist"];
                if (!jvNetList.empty() && jvNetList.isArray())
                {
                    for (
                        Json::Value::const_iterator it = jvNetList.begin(); 
                        it != jvNetList.end(); 
                    it++
                        )
                    {
                        Json::Value name = (*it)["title"];
                        Json::Value url = (*it)["neturl"];
                        if (!name.empty() && !url.empty() && 
                            name.isString() && url.isString())
                        {
                            PComboPlayList item = new ComboPlayList();
                            if (item)
                            {
                                memset(item, 0, sizeof(ComboPlayList));
#ifdef _UNICODE
                                std::wstring strName = utf8_2_utf16(name.asString());
                                std::wstring strUrl = utf8_2_utf16(url.asString());
#else
                                std::string strName = utf8_2_gbk(name.asString());
                                std::string strUrl = utf8_2_gbk(url.asString());
#endif
                                _tcscpy(item->Name, strName.c_str());
                                _tcscpy(item->szPath, strUrl.c_str());
                                _InsertTailList(&combo->next, &item->next);
                            }
                        }
                    }
                }
            }

            delete []JsonBuf;
        }

        delete []filename;
    } while (FALSE);
    
    return combo;
}

// 数据库列表 
static PComboPlayList GetDataBaseComboData()
{
    PComboPlayList combo = new ComboPlayList;
    do 
    {
        if (combo == NULL)
        {
            break;
        }

        // 第一个数据空着 
        memset(combo, 0, sizeof(ComboPlayList));
        _InitializeListHead(&combo->next);
        combo->nIndex = -1;

        // 配置文件解析utf-8  
        TCHAR* filename = new TCHAR[MAX_PATH*2];
        if (filename == NULL)
        {
            break;
        }

        GetModuleFileName(GetModuleHandle(NULL), filename, MAX_PATH);
        PathRemoveFileSpec(filename);
        PathAddBackslash(filename);
        _tcscat(filename, TEXT("video.db"));

#ifdef _UNICODE
        std::wstring wstrDBFile(filename);
        std::string pdbFile = utf16_2_utf8(wstrDBFile);
#else
        std::string strDBFile(filename);
        std::string pdbFile = gbk_2_utf8(wstrDBFile);
#endif
        // sqlite3解析 
        sqlite3* pDB;
        if ( sqlite3_open(pdbFile.c_str(), &pDB) == SQLITE_OK )
        {
            const char* psql = "select id from kankandv order by id desc limit 1,1;";

            char *errMsg;
            char **dbResult;
            int nRow = 0, nColumn = 0;
            if ( sqlite3_get_table(pDB, psql, &dbResult, &nRow, &nColumn, &errMsg) == SQLITE_OK)
            {
                DWORD nCount = strtoul(dbResult[1], NULL, 10);
                while (nCount > 0)
                {
                    PComboPlayList item = new ComboPlayList();
                    if (item)
                    {
                        memset(item, 0, sizeof(ComboPlayList));
                        if (nCount%50 != 0)
                        {
                            item->nCount = nCount%DB_LENGHT;
                        }
                        else
                        {
                            item->nCount = DB_LENGHT;
                        }
                        item->nStart = nCount - item->nCount;
                        swprintf(item->Name, TEXT("%u-%u"), item->nStart, nCount);
                        _tcscpy(item->szPath, filename);
                        _InsertTailList(&combo->next, &item->next);

                        nCount -= item->nCount;
                    }
                }
            }
            else
            {
                sqlite3_free(errMsg);
            }
            sqlite3_close(pDB);
        }
        delete []filename;
    } while (FALSE);

    return combo;
}

// 列表转移数据 
static VOID AddToCombo(PComboPlayList dst, PComboPlayList src)
{
    if (dst == NULL || src == NULL)
    {
        return ;
    }

    while(!_IsListEmpty(&src->next))
    {
        // 从左到右遍历插入尾部，顺序不变 
        PLIST_ENTRY list = _RemoveHeadList(&src->next);
        _InsertTailList(&dst->next, list);
    }
}

// 获取下拉选框数据 
PComboPlayList GetComboData()
{
    PComboPlayList combo = new ComboPlayList;
    if (combo)
    {
        // 第一个数据空着 
        memset(combo, 0, sizeof(ComboPlayList));
        _InitializeListHead(&combo->next);
        combo->nIndex = -1;

        // 本地数据 
        PComboPlayList local = GetLocalComboData();
        if (local)
        {
            if ( !_IsListEmpty(&local->next) )
            {
                PComboPlayList tmp = new ComboPlayList;
                if (tmp)
                {
                    memset(tmp, 0, sizeof(ComboPlayList));
                    _tcscpy(tmp->Name, TEXT("----- 本地直播 -----"));
                    _InsertTailList(&combo->next, &tmp->next);
                }
                AddToCombo(combo, local);
            }
            FreeComboData(local);
            local = NULL;
        }
        

        // 配置的网络数据 
        PComboPlayList network = GetConfigComboData();
        if (network)
        {
            if ( !_IsListEmpty(&network->next) )
            {
                PComboPlayList tmp = new ComboPlayList;
                if (tmp)
                {
                    memset(tmp, 0, sizeof(ComboPlayList));
                    _tcscpy(tmp->Name, TEXT("----- 在线直播 -----"));
                    _InsertTailList(&combo->next, &tmp->next);
                }
                AddToCombo(combo, network);
            }
            FreeComboData(network);
            network = NULL;
        }

        // 数据库 
        PComboPlayList database = GetDataBaseComboData();
        if (database)
        {
            if ( !_IsListEmpty(&database->next) )
            {
                PComboPlayList tmp = new ComboPlayList;
                if (tmp)
                {
                    memset(tmp, 0, sizeof(ComboPlayList));
                    _tcscpy(tmp->Name, TEXT("----- db数据 -----"));
                    _InsertTailList(&combo->next, &tmp->next);
                }
                AddToCombo(combo, database);
            }
            FreeComboData(database);
            database = NULL;
        }
    }
    return combo;
}

static PLiveUrlList GetGroupItem(PLiveUrlList playlist, _tstring &name)
{
    for (PLIST_ENTRY it = playlist->next.Flink; 
        it != &playlist->next;
        it = it->Flink)
    {
        PLiveUrlList lm = CONTAINING_RECORD(it, LiveUrlList, next);
        if (name == lm->Name)
        {
            return lm;
        }
    }

    PLiveUrlList tmp = new LiveUrlList;
    if (tmp)
    {
        memset(tmp, 0, sizeof(LiveUrlList));
        _InitializeListHead(&tmp->next);
        _InitializeListHead(&tmp->child);
        _tcscpy(tmp->Name, name.c_str());
        _InsertTailList(&playlist->next, &tmp->next);
        return tmp;
    }
    return NULL;
}

// 播放列表 FreePlayList
PLiveUrlList GetLivePlayList(LPCTSTR lpFilePath)
{
    PLiveUrlList playlist = new LiveUrlList;
    do 
    {
        if (!playlist)
        {
            break;
        }

        // 第一个数据空着 
        memset(playlist, 0, sizeof(LiveUrlList));
        _InitializeListHead(&playlist->next);
        _InitializeListHead(&playlist->child);

        LPBYTE lpBuffer= NULL;
        DWORD dwSize = 0;
        if (
            (lpFilePath[0]|0x20) == TEXT('h') &&
            (lpFilePath[1]|0x20) == TEXT('t') &&
            (lpFilePath[2]|0x20) == TEXT('t') &&
            (lpFilePath[3]|0x20) == TEXT('p') 
            )
        {
            CHttp Net(lpFilePath);
            const char* szLen = Net.GetDataLenthA();
            if (szLen)
            {
                dwSize = strtoul(szLen, NULL, 10);
            }
            else
            {
                dwSize = 4*1024*1024;
            }

            lpBuffer = new BYTE[dwSize+1];
            if (lpBuffer == NULL)
            {
                break;
            }
            memset(lpBuffer, 0, dwSize+1);
            dwSize = Net.GetData(lpBuffer, dwSize, FALSE);
        }
        else
        {
            HANDLE hFile = CreateFile(
                lpFilePath, 
                GENERIC_READ, 
                FILE_SHARE_READ, 
                NULL, 
                OPEN_EXISTING, 
                FILE_ATTRIBUTE_NORMAL, 
                NULL);
            if (hFile == INVALID_HANDLE_VALUE)
            {
                break;
            }

            dwSize = GetFileSize(hFile, NULL);
            lpBuffer = new BYTE[dwSize+2];
            if (lpBuffer == NULL)
            {
                CloseHandle(hFile);
                break;
            }
            DWORD dwBytes = 0;
            memset(lpBuffer, 0, dwSize+2);
            SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
            ReadFile(hFile, lpBuffer, dwSize, &dwBytes, NULL);
            CloseHandle(hFile);
        }

        if (lpBuffer == NULL)
        {
            break;
        }

        std::string lpUtf8((char*)lpBuffer, dwSize);
#ifdef _UNICODE
        std::wstring lpUtf16 = utf8_2_utf16(lpUtf8);
        std::wstringstream strStream(lpUtf16);

        std::wstring name;
        std::wstring url;
#else
        std::string lpGbk = utf8_2_gbk(lpUtf8);
        std::stringstream strStream(lpGbk);

        std::string name;
        std::string url;
#endif
        while(!strStream.eof())
        {
            std::getline(strStream, name);
            std::getline(strStream, url);
            while (url[0] == TEXT('#'))
            {
                name = url;
                if ( strStream.eof() )
                {
                    break;
                }
                std::getline(strStream, url);
            }

            if (
                (url[0]|0x20) == TEXT('h') &&
                (url[1]|0x20) == TEXT('t') &&
                (url[2]|0x20) == TEXT('t') &&
                (url[3]|0x20) == TEXT('p')
                )
            {
                PLiveUrlList groupList = NULL;
                size_t ngroup = name.find(TEXT("group-title="));
                if (ngroup != -1)
                {
                    ngroup += 13;
                    size_t nends = name.find(TEXT('"'), ngroup);
                    _tstring strGroup(name.c_str(), ngroup, nends-ngroup);
                    groupList = GetGroupItem(playlist, strGroup);
                }

                PLiveUrlList il = new LiveUrlList;
                if (il)
                {
                    _InitializeListHead(&il->child);
                    size_t n = name.find_last_of(TEXT(','));
                    if (n == -1)
                    {
                        _tcscpy(il->Name, name.c_str());
                        n = 0;
                    }
                    else
                    {
                        _tcscpy(il->Name, name.c_str()+n+1);
                    }

                    _tcscpy(il->szPath, url.c_str());
                    CalcMd5((PBYTE)url.c_str(), url.length()*sizeof(TCHAR), il->HashStr);
                    if (groupList)
                    {
                        _InsertTailList(&groupList->child, &il->next);
                    }
                    else
                    {
                        _InsertTailList(&playlist->next, &il->next);
                    }
                    
                }
            }
        }

        delete []lpBuffer;
    } while (false);

    return playlist;
}

// 数据库 
PLiveUrlList GetVideoDBList(DWORD dwStart, DWORD dwCount)
{
    PLiveUrlList playlist = new LiveUrlList;
    do 
    {
        if (playlist == NULL)
        {
            break;
        }

        // 第一个数据空着 
        memset(playlist, 0, sizeof(LiveUrlList));
        _InitializeListHead(&playlist->next);
        _InitializeListHead(&playlist->child);

        // 配置文件解析utf-8  
        TCHAR* filename = new TCHAR[MAX_PATH*2];
        if (filename == NULL)
        {
            break;
        }

        GetModuleFileName(GetModuleHandle(NULL), filename, MAX_PATH);
        PathRemoveFileSpec(filename);
        PathAddBackslash(filename);
        _tcscat(filename, TEXT("video.db"));

#ifdef _UNICODE
        std::wstring wstrDBFile(filename);
        std::string pdbFile = utf16_2_utf8(wstrDBFile);
#else
        std::string strDBFile(filename);
        std::string pdbFile = gbk_2_utf8(wstrDBFile);
#endif
        // sqlite3解析 
        sqlite3* pDB;
        if ( sqlite3_open(pdbFile.c_str(), &pDB) == SQLITE_OK )
        {
            char* psql = new char[1024];
            sprintf(psql, "select title,m3u8_url from kankandv where id>%u order by id limit %u;", dwStart, dwCount+1);

            char *errMsg;
            char **dbResult;
            int nRow = 0, nColumn = 0;
            if ( sqlite3_get_table(pDB, psql, &dbResult, &nRow, &nColumn, &errMsg) == SQLITE_OK)
            {
                for (int i=1; i<=nRow; i++)
                {
                    std::string strName = dbResult[nColumn*i];
                    std::string strPath = dbResult[nColumn*i + 1];
                    std::wstring wStrName = utf8_2_utf16(strName);
                    std::wstring wStrPath = utf8_2_utf16(strPath);
                    size_t nPos = wStrPath.find(TEXT("kankandv.xyz"));
                    if (nPos != _tstring::npos)
                    {
                        wStrPath.replace(nPos, 12, TEXT("kankandv.com"));
                    }

                    PLiveUrlList il = new LiveUrlList;
                    if (il)
                    {
                        _InitializeListHead(&il->child);
                        _tcscpy(il->Name, wStrName.c_str());
                        _tcscpy(il->szPath, wStrPath.c_str());
                        CalcMd5((PBYTE)wStrPath.c_str(), wStrPath.length()*sizeof(TCHAR), il->HashStr);
                        // 正序查询，倒叙插入,最新的在最上面  
                        _InsertHeadList(&playlist->next, &il->next);
                    }
                }
            }
            else
            {
                sqlite3_free(errMsg);
            }
            sqlite3_close(pDB);
        }
        delete []filename;
    } while (FALSE);

    return playlist;
}

// 本地历史播放列表,暂时关闭清空 
PLiveUrlList GetLocalPlayList()
{
    return NULL;
}

VOID FreePlayList(PLiveUrlList pPlist)
{
    if (pPlist == NULL)
    {
        return;
    }

    while( !_IsListEmpty(&pPlist->next) )
    {
        PLIST_ENTRY list = _RemoveTailList(&pPlist->next);
        PLiveUrlList subitem = CONTAINING_RECORD(list, LiveUrlList, next);
        while( !_IsListEmpty(&subitem->child) )
        {
            PLIST_ENTRY child = _RemoveTailList(&subitem->child);
            PLiveUrlList childitem = CONTAINING_RECORD(child, LiveUrlList, next);
            delete childitem;
        }
        delete subitem;
    }
    delete pPlist;
}

