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
    EM_PLAY_MODE_SEQUENCE,      // ˳�򲥷�
    EM_PLAY_MODE_RANDOM,        // �������
    EM_PLAY_MODE_SINGLE_CIRCLE  // ����ѭ��
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
    LRESULT OnPlayTimer();  // ����ʱ����ʱ��ͬ������ 
    LRESULT OnDlgSelectFile();
    LRESULT OnShowCursor(BOOL bShow);
    
protected:
    CTreeNodeUI* AddLivePlayerTypeNode(CTreeNodeUI *pListTree, LPCTSTR lpStrName);
    CTreeNodeUI* AddLivePlayerNode(CTreeNodeUI *pRootNode, LPCTSTR lpStrName, LPCTSTR lpGUID);
    DWORD AddComboTypeLabel(CComboUI* pCombo, LPCTSTR lpName);
    DWORD AddComboItem(CComboUI* pCombo, LPCTSTR lpName);
    BOOL AttachAplayerWnd();

private:
    CTextUI* m_captionText;  // ������ 
    CButtonUI* m_ShowPlayList; // �Ѿ�����ʾ�İ�ť 
    CButtonUI* m_HidePlayList; // �Ѿ������صİ�ť 
    CVerticalLayoutUI* m_PlayListBlock; // �����б��� 
    DWORD m_PlayListWidth;

    CEditUI* m_searchPlayList; // ���������б� 
    CEditUI* m_PlayUrl; // ���ŵ�ַ�� 

    CButtonUI* m_StartButton;  // ���Ű�ť 
    CButtonUI* m_StopButton;   // ֹͣ���� 
    CButtonUI* m_PauseButton;  // ��ͣ���� 
    CButtonUI* m_VolumeButton; // ������ť,���±�ɾ��� 
    CButtonUI* m_DisVolumeButon;// ���������������ť 
    CSliderUI* m_VolumeSlider; // ���������� 

    CButtonUI* m_FullScreenBtn;  // ����ȫ�� 
    CButtonUI* m_NormalScreenBtn;  // �ر�ȫ�� 

    CLabelUI* m_playSpeedRegeLabel; // �����ٶ� 
    CLabelUI* m_playProgressLabel; // ��ǰ����ʱ�� 
    CVerticalLayoutUI* m_ProgressBlock; // �������� 
    CButtonUI* m_FastBackButton;// �����ť 
    CSliderUI* m_ProgressSlider;// ���Ž��� 
    CButtonUI* m_FastForwardButton;// ���˰�ť 

    CComboUI* m_comboSelectPlayList; // ����ѡ��ֱ��Դ 

    CTreeNodeUI* m_PlayListWnd;  // ��Ŀ���б� 
    CTreeNodeUI* m_LivePlayList;  // ����ֱ���б� 
    CTreeNodeUI* m_LocalPlayList; // ���ز����б� 

    CHorizontalLayoutUI* m_PlayerClientDlg;  // Aplayer����λ�� 

    // ���沥�ſ��ƿ�
    CVerticalLayoutUI* m_PlayControlPanel;
    DWORD m_PlayControlHeight;
    // ������ 
    CHorizontalLayoutUI* m_CaptionPanel; 
    DWORD m_CaptionHeight;
    // ֱ����ַ�� 
    CHorizontalLayoutUI* m_LiveUrlPanel;

    VOID InitCControl();

private:
    // ����������ӿ� 
    BOOL StartPlay(LPCTSTR lpMedia);
    BOOL StopPlay();
    BOOL PausePlay();
    LRESULT SwitchFullScreen(UINT uMsg, WPARAM wParam, LPARAM lParam);
    BOOL onFullScreen(BOOL bFull, RECT* rc = NULL);
    static DWORD WINAPI SelectItemLoad(LPVOID lparam);

private:
    CPaintManagerUI m_PaintManager;

    // ���������ָ����ʾʱ�� 
    HWND m_hwndSlidertip;
    TOOLINFO m_ToolTip;
    POINT m_ToolMousePt;
    // ӰƬʱ��(��) 
    DWORD m_Movie_Secounds;
    // �����ٶ� (*100)
    DWORD m_play_speed;

    // APlayer �� 
    CMyPlayer m_MyAPlayer;

    // ȫ��ǰ������ 
    RECT m_clientRect;

    // �����б����� 
    PComboPlayList m_combo_playlist;
    // ֱ�������б� 
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
