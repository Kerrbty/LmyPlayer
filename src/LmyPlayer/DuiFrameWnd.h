#pragma once
#include "BaseWnd.h"
#include "PlayerEvent.h"
#include "defs.h"
#include "common.h"

#define ID_LBUTTONCLICK   2010
#define ID_PLAYTIMER      2020
#define WM_UNDATE_LOCAL   (WM_USER + 0x2000)
#define WM_UPSATE_LIVE    (WM_USER + 0x2001)

enum enum_PlayMode
{
    EM_PLAY_MODE_SEQUENCE,      // 顺序播放
    EM_PLAY_MODE_RANDOM,        // 随机播放
    EM_PLAY_MODE_SINGLE_CIRCLE  // 单曲循环
};

class CDuiFrameWnd: public CXMLWnd
{
public:
    explicit CDuiFrameWnd(LPCTSTR pszXMLName);
    ~CDuiFrameWnd();

    DUI_DECLARE_MESSAGE_MAP()
    virtual void InitWindow();
    virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    virtual CControlUI* CreateControl(LPCTSTR pstrClassName);
    virtual void Notify(TNotifyUI& msg);
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void OnClick(TNotifyUI& msg);
    virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnKeyClick(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT OnAPlayerMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT OnAPlayerStateChange(UINT uMsg, WPARAM wParam, LPARAM lParam);

    virtual void OnSetFocus(TNotifyUI& msg);
    virtual void KillSetFocus(TNotifyUI& msg);
    virtual void onSelectItem(TNotifyUI& msg);
    virtual void onItemActive(TNotifyUI& msg);
    virtual void onSLiderChange(TNotifyUI& msg);
    virtual void onWindowSize(TNotifyUI& msg);
    LRESULT OnDropFiles(HDROP hDropInfo);
    LRESULT OnUpdateLocalFile();
    LRESULT OnUpdateLiveList();
    LRESULT OnPlayTimer();  // 播放时，定时器同步进度 
    LRESULT OnDlgSelectFile();
    LRESULT OnShowCursor(BOOL bShow);
    
protected:
    CTreeNodeUI* AddLivePlayerTypeNode(CTreeNodeUI *pListTree, LPCTSTR lpStrName);
    CTreeNodeUI* AddLivePlayerNode(CTreeNodeUI *pRootNode, LPCTSTR lpStrName, LPCTSTR lpGUID);
    DWORD AddComboTypeLabel(CComboUI* pCombo, LPCTSTR lpName);
    DWORD AddComboItem(CComboUI* pCombo, LPCTSTR lpName);
    BOOL AttachAplayerWnd();

private:
    CTextUI* m_captionText;  // 标题栏 
    CButtonUI* m_ShowPlayList; // 已经是显示的按钮 
    CButtonUI* m_HidePlayList; // 已经是隐藏的按钮 
    CVerticalLayoutUI* m_PlayListBlock; // 播放列表窗口 
    DWORD m_PlayListWidth;

    CEditUI* m_searchPlayList; // 搜索播放列表 
    CEditUI* m_PlayUrl; // 播放地址框 

    CButtonUI* m_StartButton;  // 播放按钮 
    CButtonUI* m_StopButton;   // 停止播放 
    CButtonUI* m_PauseButton;  // 暂停播放 
    CButtonUI* m_VolumeButton; // 音量按钮,按下变成静音 
    CButtonUI* m_DisVolumeButon;// 静音变成有声音按钮 
    CSliderUI* m_VolumeSlider; // 音量进度条 

    CButtonUI* m_FullScreenBtn;  // 开启全屏 
    CButtonUI* m_NormalScreenBtn;  // 关闭全屏 

    CLabelUI* m_playSpeedRegeLabel; // 播放速度 
    CLabelUI* m_playProgressLabel; // 当前播放时间 
    CVerticalLayoutUI* m_ProgressBlock; // 进度条块 
    CButtonUI* m_FastBackButton;// 快进按钮 
    CSliderUI* m_ProgressSlider;// 播放进度 
    CButtonUI* m_FastForwardButton;// 快退按钮 

    CComboUI* m_comboSelectPlayList; // 下拉选择直播源 

    CTreeNodeUI* m_PlayListWnd;  // 节目单列表 
    CTreeNodeUI* m_LivePlayList;  // 在线直播列表 
    CTreeNodeUI* m_LocalPlayList; // 本地播放列表 

    CHorizontalLayoutUI* m_PlayerClientDlg;  // Aplayer附加位置 

    // 下面播放控制块
    CVerticalLayoutUI* m_PlayControlPanel;
    DWORD m_PlayControlHeight;
    // 标题栏 
    CHorizontalLayoutUI* m_CaptionPanel; 
    DWORD m_CaptionHeight;
    // 直播地址栏 
    CHorizontalLayoutUI* m_LiveUrlPanel;

    VOID InitCControl();

private:
    // 播放器处理接口 
    BOOL StartPlay(LPCTSTR lpMedia);
    BOOL StopPlay();
    BOOL PausePlay();
    LRESULT SwitchFullScreen(UINT uMsg, WPARAM wParam, LPARAM lParam);
    BOOL onFullScreen(BOOL bFull, RECT* rc = NULL);
    static DWORD WINAPI SelectItemLoad(LPVOID lparam);

private:
    CPaintManagerUI m_PaintManager;

    // 进度条鼠标指针显示时间 
    HWND m_hwndSlidertip;
    TOOLINFO m_ToolTip;
    POINT m_ToolMousePt;
    // 影片时长(秒) 
    DWORD m_Movie_Secounds;
    // 播放速度 (*100)
    DWORD m_play_speed;

    // APlayer 类 
    CMyPlayer m_MyAPlayer;

    // 全屏前的坐标 
    RECT m_clientRect;

    // 下拉列表数据 
    PComboPlayList m_combo_playlist;
    // 直播播放列表 
    PLiveUrlList m_live_playlist;

    LocalMovieList m_LocalList;

    _tstring m_current_play;

    HCURSOR m_system_cursor;
    POINT   m_mouse_pos;
    DWORD   m_mouse_time;
};

typedef struct _SelectComboPlayListParam 
{
    PComboPlayList pPL;
    CDuiFrameWnd*  cuiframe;
}SelectComboPlayListParam, *PSelectComboPlayListParam;
