#pragma once
#include "List.h"
#include <tchar.h>

// 下拉框数据 
typedef struct _ComboPlayList 
{
    LIST_ENTRY next;
    TCHAR Name[MAX_PATH]; // 选项名 
    INT nIndex;  // 在下拉列表的索引值 
    TCHAR szPath[MAX_PATH*2]; // 这里是对应文件路径(本地或者在线) 
    // 如果数据库则是起始ID和数量(不一定准确)  
    UINT nStart;
    UINT nCount; 
}ComboPlayList, *PComboPlayList;

// 视频地址 
typedef struct _LiveUrlList
{
    LIST_ENTRY next;
    TCHAR Name[MAX_PATH];
    TCHAR HashStr[34];  // 唯一值 
    TCHAR szPath[MAX_PATH*2];
    LIST_ENTRY child; // 父子节点 
}LiveUrlList, *PLiveUrlList;

// 本地播放地址 
typedef struct _LocalMovieList
{
    LIST_ENTRY next;
    TCHAR Name[MAX_PATH];
    TCHAR HashStr[34];
    TCHAR szPath[MAX_PATH];
}LocalMovieList, *PLocalMovieList;


// 下拉框列表 
PComboPlayList GetComboData();
VOID FreeComboData(PComboPlayList combo);

// 播放列表 FreePlayList
PLiveUrlList GetLivePlayList(LPCTSTR lpFilePath);
PLiveUrlList GetVideoDBList(DWORD dwStart, DWORD dwCount);
PLiveUrlList GetLocalPlayList();
VOID FreePlayList(PLiveUrlList pPlist);

BOOL CalcMd5(PBYTE pbuf, DWORD dwlen, LPTSTR res);
