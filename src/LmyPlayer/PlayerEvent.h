#pragma once
#include <Windows.h>
#include <tchar.h>
#include <atlbase.h>
#include "PlayerSink.h"

// WM_USER ~ 0x7FFF 专用窗口消息 
// WM_APP ~ 0xBFFF 可供应用程序使用 
// 0xC00 ~ 0xFFF 供应用程序使用字符串消息 

// 播放器状态 
enum PLAYER_STATUS
{
    PER_OPENSUCCESS     = 0,
    PER_SEEKCOMPLET     = 1,
    PER_BUFFER          = 2,
    PER_VIDEOSIZECHANGE = 3,
    PER_DOWNLOADCODEC   = 4,
    PER_PLAYSTATUS      = 5, // 播放器播放情况,播放、暂停、关闭  
};
#define WM_PLAYERSTATUS           (WM_USER + 0x1000)  // 播放器设置状态 

// 播放状态 
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
#define WM_PLAYSTATECHANGE        (WM_USER + 0x1001)  // 播放状态 
#define WM_PLAYLBUTTONDOWN        (WM_USER + 0x1005)  // 左键按下 
#define WM_PLAYLBUTTONUP          (WM_USER + 0x1006)  // 左键放开 
#define WM_PLAYLBUTTONDCLICK      (WM_USER + 0x1008)  // 双击 
#define WM_PLAYKEYDOWN            (WM_USER + 0x1010)  // 按键消息 
#define WM_PLAYMOUSEMOVE          (WM_USER + 0x1011)  // 鼠标移动(全屏时处理) 
#define WM_PLAYMOUSEHOVER         (WM_USER + 0x1012)  // 鼠标悬停 


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
    int                     m_nPosition;  // 进度百分比 
    long                    m_nDuration;  // 媒体时长(毫秒) 
    APlayer3Lib::IPlayer *  m_pAPlayer;   // APlayer 播放器句柄 
    DWORD                   m_dwCookie;
    CPlayerSink             m_APlayerSink;  // APlayer SDK COM接口 
    IConnectionPoint *      m_pConnectionPoint; // 连接点指针 

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
    HWND                    m_hLmyPlayerWnd;  // 父窗口句柄 
    HWND                    m_hAPlayerWnd;  // 附加APlayer的窗口 
};
