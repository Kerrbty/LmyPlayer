#pragma once
#include "List.h"
#include <tchar.h>

// ���������� 
typedef struct _ComboPlayList 
{
    LIST_ENTRY next;
    TCHAR Name[MAX_PATH]; // ѡ���� 
    INT nIndex;  // �������б������ֵ 
    TCHAR szPath[MAX_PATH*2]; // �����Ƕ�Ӧ�ļ�·��(���ػ�������) 
    // ������ݿ�������ʼID������(��һ��׼ȷ)  
    UINT nStart;
    UINT nCount; 
}ComboPlayList, *PComboPlayList;

// ��Ƶ��ַ 
typedef struct _LiveUrlList
{
    LIST_ENTRY next;
    TCHAR Name[MAX_PATH];
    TCHAR HashStr[34];  // Ψһֵ 
    TCHAR szPath[MAX_PATH*2];
    LIST_ENTRY child; // ���ӽڵ� 
}LiveUrlList, *PLiveUrlList;

// ���ز��ŵ�ַ 
typedef struct _LocalMovieList
{
    LIST_ENTRY next;
    TCHAR Name[MAX_PATH];
    TCHAR HashStr[34];
    TCHAR szPath[MAX_PATH];
}LocalMovieList, *PLocalMovieList;


// �������б� 
PComboPlayList GetComboData();
VOID FreeComboData(PComboPlayList combo);

// �����б� FreePlayList
PLiveUrlList GetLivePlayList(LPCTSTR lpFilePath);
PLiveUrlList GetVideoDBList(DWORD dwStart, DWORD dwCount);
PLiveUrlList GetLocalPlayList();
VOID FreePlayList(PLiveUrlList pPlist);

BOOL CalcMd5(PBYTE pbuf, DWORD dwlen, LPTSTR res);
