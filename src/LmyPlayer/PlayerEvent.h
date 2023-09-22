#pragma once
#include <Windows.h>
#include <tchar.h>
#include <atlbase.h>
#include "PlayerSink.h"

// WM_USER ~ 0x7FFF ר�ô�����Ϣ 
// WM_APP ~ 0xBFFF �ɹ�Ӧ�ó���ʹ�� 
// 0xC00 ~ 0xFFF ��Ӧ�ó���ʹ���ַ�����Ϣ 

// ������״̬ 
enum PLAYER_STATUS
{
    PER_OPENSUCCESS     = 0,
    PER_SEEKCOMPLET     = 1,
    PER_BUFFER          = 2,
    PER_VIDEOSIZECHANGE = 3,
    PER_DOWNLOADCODEC   = 4,
    PER_PLAYSTATUS      = 5, // �������������,���š���ͣ���ر�  
};
#define WM_PLAYERSTATUS           (WM_USER + 0x1000)  // ����������״̬ 

// ����״̬ 
enum PLAY_STATE
{
    PS_READY          = 0,
    PS_OPENING        = 1,
    PS_PAUSING        = 2,
    PS_PAUSED         = 3,
    PS_PLAYING        = 4,
    PS_PLAY           = 5,
    PS_CLOSING        = 6,
};
#define WM_PLAYSTATECHANGE        (WM_USER + 0x1001)  // ����״̬ 
#define WM_PLAYLBUTTONDOWN        (WM_USER + 0x1005)  // ������� 
#define WM_PLAYLBUTTONUP          (WM_USER + 0x1006)  // ����ſ� 
#define WM_PLAYLBUTTONDCLICK      (WM_USER + 0x1008)  // ˫�� 
#define WM_PLAYKEYDOWN            (WM_USER + 0x1010)  // ������Ϣ 
#define WM_PLAYMOUSEMOVE          (WM_USER + 0x1011)  // ����ƶ�(ȫ��ʱ����) 
#define WM_PLAYMOUSEHOVER         (WM_USER + 0x1012)  // �����ͣ 


class CPlayerEventHandler
{
public:
    virtual HRESULT OnMessage(LONG nMessage, LONG wParam, LONG lParam) = 0;
    virtual HRESULT OnStateChanged(LONG nOldState, LONG nNewState) = 0;
    virtual HRESULT OnOpenSucceeded() = 0;
    virtual HRESULT OnSeekCompleted(LONG nPosition) = 0;
    virtual HRESULT OnBuffer(LONG nPercent) = 0;
    virtual HRESULT OnVideoSizeChanged(void) = 0;
    virtual HRESULT OnDownloadCodec(BSTR strCodecPath) = 0;
};

class CMyPlayer : public CPlayerEventHandler
{
public:
    int                     m_nPosition;  // ���Ȱٷֱ� 
    long                    m_nDuration;  // ý��ʱ��(����) 
    APlayer3Lib::IPlayer *  m_pAPlayer;   // APlayer ��������� 
    DWORD                   m_dwCookie;
    CPlayerSink             m_APlayerSink;  // APlayer SDK COM�ӿ� 
    IConnectionPoint *      m_pConnectionPoint; // ���ӵ�ָ�� 

public:
    void            Initialize();
    void            Release();

    // aplayer event handler
    virtual    HRESULT    OnMessage(LONG nMessage, LONG wParam, LONG lParam);
    virtual    HRESULT    OnStateChanged(LONG nOldState, LONG nNewState);
    virtual    HRESULT    OnOpenSucceeded();
    virtual    HRESULT    OnSeekCompleted(LONG nPosition);
    virtual    HRESULT    OnBuffer(LONG nPercent);
    virtual    HRESULT    OnVideoSizeChanged(void);
    virtual    HRESULT    OnDownloadCodec(BSTR strCodecPath);

public:
    void SetPlayerHwnd(HWND);
    HWND GetPlayerHwnd();

private:
    HWND                    m_hLmyPlayerWnd;  // �����ھ�� 
    HWND                    m_hAPlayerWnd;  // ����APlayer�Ĵ��� 
};
