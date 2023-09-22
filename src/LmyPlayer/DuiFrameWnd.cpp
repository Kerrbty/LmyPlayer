#include "DuiFrameWnd.h"
#include "res/resource.h"
#include "MsgBox.h"
#include <time.h>

#pragma warning(disable : 4996)
#pragma warning(disable : 4018)
CDuiFrameWnd::CDuiFrameWnd( LPCTSTR pszXMLName )
                 : CXMLWnd(pszXMLName),
                 m_captionText(NULL),
                 m_ShowPlayList(NULL),
                 m_HidePlayList(NULL),
                 m_PlayListBlock(NULL),
                 m_searchPlayList(NULL),
                 m_PlayUrl(NULL),
                 m_StartButton(NULL),
                 m_StopButton(NULL),
                 m_PauseButton(NULL),
                 m_VolumeButton(NULL),
                 m_DisVolumeButon(NULL),
                 m_VolumeSlider(NULL),
                 m_FullScreenBtn(NULL),
                 m_NormalScreenBtn(NULL),
                 m_playSpeedRegeLabel(NULL),
                 m_playProgressLabel(NULL),
                 m_ProgressBlock(NULL),
                 m_FastBackButton(NULL),
                 m_ProgressSlider(NULL),
                 m_FastForwardButton(NULL),
                 m_comboSelectPlayList(NULL),
                 m_PlayListWnd(NULL),
                 m_LivePlayList(NULL),
                 m_LocalPlayList(NULL),
                 m_PlayerClientDlg(NULL),
                 m_PlayControlPanel(NULL),
                 m_CaptionPanel(NULL),
                 m_LiveUrlPanel(NULL),
                 m_hwndSlidertip(NULL),
                 m_Movie_Secounds(0),
                 m_play_speed(100),
                 m_PlayControlHeight(0),
                 m_CaptionHeight(0),
                 m_PlayListWidth(0),
                 m_combo_playlist(NULL),
                 m_live_playlist(NULL),
                 m_system_cursor(NULL),
                 m_mouse_time(0)
{
    ZeroMemory(&m_mouse_pos, sizeof(m_mouse_pos));
    ZeroMemory(&m_clientRect, sizeof(m_clientRect));
    ::ZeroMemory(&m_ToolTip, sizeof(TOOLINFO));
    memset(&m_LocalList, 0, sizeof(m_LocalList));
    _InitializeListHead(&m_LocalList.next);
    m_ToolTip.lpszText = new TCHAR[80];
}

CDuiFrameWnd::~CDuiFrameWnd()
{
    if( m_hwndSlidertip != NULL ) {
        ::DestroyWindow(m_hwndSlidertip);
        m_hwndSlidertip = NULL;
    }
    if (m_ToolTip.lpszText)
    {
        delete []m_ToolTip.lpszText;
        m_ToolTip.lpszText = NULL;
    }

    while (!_IsListEmpty(&m_LocalList.next))
    {
        PLIST_ENTRY list = _RemoveTailList(&m_LocalList.next);
        LocalMovieList* pl = CONTAINING_RECORD(list, LocalMovieList, next);
        delete pl;
    }

    FreeComboData(m_combo_playlist);
    FreePlayList(m_live_playlist);
}

VOID CDuiFrameWnd::InitCControl()
{
    if (m_captionText == NULL)
    {
        m_captionText = static_cast<CTextUI*>(m_pm.FindControl(_T("PlayerName")));
    }
    if (m_ShowPlayList == NULL)
    {
        m_ShowPlayList = static_cast<CButtonUI*>(m_pm.FindControl(TEXT("btnPlaylistShow")));
    }
    if (m_HidePlayList == NULL)
    {
        m_HidePlayList = static_cast<CButtonUI*>(m_pm.FindControl(TEXT("btnPlaylistHide")));
    }
    if (m_PlayListBlock == NULL)
    {
        m_PlayListBlock = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(TEXT("ctnPlaylist")));
    }
    if (m_searchPlayList == NULL)
    {
        m_searchPlayList = static_cast<CEditUI*>(m_pm.FindControl(TEXT("editSearch")));
    }
    if (m_PlayUrl == NULL)
    {
        m_PlayUrl = static_cast<CEditUI*>(m_pm.FindControl(TEXT("editURL")));
    }
    if (m_StartButton == NULL)
    {
        m_StartButton = static_cast<CButtonUI*>(m_pm.FindControl(TEXT("btnPlay")));
    }
    if (m_StopButton == NULL)
    {
        m_StopButton = static_cast<CButtonUI*>(m_pm.FindControl(TEXT("btnStop")));
    }
    if (m_PauseButton == NULL)
    {
        m_PauseButton = static_cast<CButtonUI*>(m_pm.FindControl(TEXT("btnPause")));
    }
    if (m_VolumeButton == NULL)
    {
        m_VolumeButton = static_cast<CButtonUI*>(m_pm.FindControl(TEXT("btnVolume")));
    }
    if (m_DisVolumeButon == NULL)
    {
        m_DisVolumeButon = static_cast<CButtonUI*>(m_pm.FindControl(TEXT("btnVolumeZero")));
    }
    if (m_VolumeSlider == NULL)
    {
        m_VolumeSlider = static_cast<CSliderUI*>(m_pm.FindControl(TEXT("sliderVol")));
    }
    if (m_FullScreenBtn == NULL)
    {
        m_FullScreenBtn = static_cast<CButtonUI*>(m_pm.FindControl(TEXT("btnScreenFull")));
    }
    if (m_NormalScreenBtn == NULL)
    {
        m_NormalScreenBtn = static_cast<CButtonUI*>(m_pm.FindControl(TEXT("btnScreenNormal")));
    }
    if (m_playSpeedRegeLabel == NULL)
    {
        m_playSpeedRegeLabel = static_cast<CLabelUI*>(m_pm.FindControl(TEXT("playSpeedRegulation")));
    }
    if (m_playProgressLabel == NULL)
    {
        m_playProgressLabel = static_cast<CLabelUI*>(m_pm.FindControl(TEXT("playProgressLabel")));
    }
    if (m_ProgressBlock == NULL)
    {
        m_ProgressBlock = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(TEXT("ctnSlider")));
    }
    if (m_FastBackButton == NULL)
    {
        m_FastBackButton = static_cast<CButtonUI*>(m_pm.FindControl(TEXT("btnFastBackward")));
    }
    if (m_ProgressSlider == NULL)
    {
        m_ProgressSlider = static_cast<CSliderUI*>(m_pm.FindControl(TEXT("sliderPlay")));
    }
    if (m_FastForwardButton == NULL)
    {
        m_FastForwardButton = static_cast<CButtonUI*>(m_pm.FindControl(TEXT("btnFastForward")));
    }
    if (m_comboSelectPlayList == NULL)
    {
        m_comboSelectPlayList = static_cast<CComboUI*>(m_pm.FindControl(TEXT("comboPlaylist")));
    }
    if (m_PlayListWnd == NULL)
    {
        m_PlayListWnd = static_cast<CTreeNodeUI*>(m_pm.FindControl(TEXT("livePlayList")));
    }
    if (m_LivePlayList == NULL)
    {
        m_LivePlayList = static_cast<CTreeNodeUI*>(m_pm.FindControl(TEXT("livePlaySelecter")));
    }
    if (m_LocalPlayList == NULL)
    {
        m_LocalPlayList = static_cast<CTreeNodeUI*>(m_pm.FindControl(TEXT("localPlayList")));
    }
    if (m_PlayerClientDlg == NULL)
    {
        m_PlayerClientDlg = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(TEXT("ctnClient")));
    }
    if (m_PlayControlPanel == NULL)
    {
        m_PlayControlPanel = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(TEXT("ctnPlayWnd")));
    }
    if (m_CaptionPanel == NULL)
    {
        m_CaptionPanel = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(TEXT("ctnCaption")));
    }
    if (m_LiveUrlPanel == NULL)
    {
        m_LiveUrlPanel = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(TEXT("ctnURL")));
    }
}

DUI_BEGIN_MESSAGE_MAP(CDuiFrameWnd, CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SETFOCUS, OnSetFocus)
DUI_ON_MSGTYPE(DUI_MSGTYPE_KILLFOCUS, KillSetFocus)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, onSelectItem)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMACTIVATE, onItemActive)
DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED, onSLiderChange)
DUI_ON_MSGTYPE(DUI_MSGTYPE_WINDOWSIZE, onWindowSize)
DUI_END_MESSAGE_MAP()

#pragma region "播放列表添加"
//////////////////////////////////////////////////////////////////////////
// 播放列表添加 

// 分类 
CTreeNodeUI* CDuiFrameWnd::AddLivePlayerTypeNode(CTreeNodeUI *pListTree, LPCTSTR lpStrName)
{
    CTreeNodeUI *pNodeRoot = new CTreeNodeUI;
    if (pNodeRoot)
    {
        pNodeRoot->SetItemTextColor(0xFFC8C6CB);
        pNodeRoot->SetItemHotTextColor(0xFFC8C6CB);
        pNodeRoot->SetSelItemTextColor(0xFFC8C6CB);
        pNodeRoot->SetItemText(lpStrName); 
        // pNodeRoot->SetToolTip(lpStrName);
        // pNodeRoot->SetName(lpStrName); 
        pNodeRoot->SetAttribute(_T("height"), _T("22"));
        pNodeRoot->SetAttribute(_T("inset"), _T("-20,0,0,0"));
        pNodeRoot->SetAttribute(_T("itemattr"), _T("valign=&quot;vcenter&quot; font=&quot;0&quot; textpadding=&quot;4,0,0,0&quot;"));
        pNodeRoot->SetAttribute(_T("folderattr"), _T("padding=&quot;0,5,0,0&quot; width=&quot;12&quot; height=&quot;12&quot; selectedimage=&quot;file='treeview_expand.png' source='0,0,12,12' &quot; normalimage=&quot;file='treeview_expand.png' source='13,0,25,12' &quot;"));
        pListTree->AddChildNode(pNodeRoot);
    }
    return pNodeRoot;
}

// 某大类下的播放节点 
CTreeNodeUI* CDuiFrameWnd::AddLivePlayerNode(CTreeNodeUI *pRootNode, LPCTSTR lpStrName, LPCTSTR lpGUID)
{
    CTreeNodeUI *pNodeTmp = new CTreeNodeUI;
    if (pNodeTmp)
    {
        pNodeTmp->SetItemTextColor(0xFFC8C6CB);
        pNodeTmp->SetItemHotTextColor(0xFFC8C6CB);
        pNodeTmp->SetSelItemTextColor(0xFFC8C6CB);
        pNodeTmp->SetItemText(lpStrName);  
        // pNodeTmp->SetToolTip(lpStrName);
        pNodeTmp->SetName(lpGUID); // 这个名字设置用于双击播放处理 
        pNodeTmp->SetAttribute(_T("height"), _T("22"));
        pNodeTmp->SetAttribute(_T("inset"), _T("7,0,0,0"));
        pNodeTmp->SetAttribute(_T("itemattr"), _T("valign=\"vcenter\" font=\"4\""));
        pNodeTmp->SetAttribute(_T("folderattr"), _T("width=\"0\" float=\"true\""));
        pRootNode->AddChildNode(pNodeTmp);
    }
    return pNodeTmp;
}
//////////////////////////////////////////////////////////////////////////
#pragma endregion "播放列表添加"


#pragma region "添加直播列表"
//////////////////////////////////////////////////////////////////////////
// 添加直播列表 
DWORD CDuiFrameWnd::AddComboTypeLabel(CComboUI* pCombo, LPCTSTR lpName)
{
    CLabelUI *pLabel = new CLabelUI;
    if (pLabel)
    {
        pLabel->SetText(lpName);
        pLabel->SetBkColor(0xFF2E2E2E);
        pLabel->SetTextColor(0xFFC8C6CB);
        pLabel->SetAttribute(TEXT("aligen"), TEXT("center"));
        pCombo->Add(pLabel);
    }

    return 0;
}

DWORD CDuiFrameWnd::AddComboItem(CComboUI* pCombo, LPCTSTR lpName)
{
    CListLabelElementUI *pListElement = new CListLabelElementUI;
    if (pListElement)
    {
        pListElement->SetText(lpName);
        pCombo->Add(pListElement);
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////
#pragma endregion "添加直播列表"

// 初始化 WM_CREATE 消息事件 
void CDuiFrameWnd::InitWindow()
{
    // 初始化播放器控件 
    m_MyAPlayer.Initialize();

    // 加载光标 
    m_system_cursor = LoadCursor(CPaintManagerUI::GetInstance(), MAKEINTRESOURCE(IDC_CURSOR1));

    // 搜索所有控件 
    InitCControl();

    // 任务栏图标 
    SetIcon(_APP_ICON_RESOURCE_VALUE);

    //////////////////////////////////////////////////////////////////////////
    // 直播下拉选项 
    if (m_comboSelectPlayList)
    {
        m_combo_playlist = GetComboData();
        m_comboSelectPlayList->RemoveAll();

        if (m_combo_playlist)
        {
            DWORD nIndex = 0;
            for (PLIST_ENTRY it = m_combo_playlist->next.Flink; 
                it != &m_combo_playlist->next; 
                it = it->Flink)
            {
                PComboPlayList pPL = CONTAINING_RECORD(it, ComboPlayList, next);
                if (pPL->szPath[0] == 0)
                {
                    AddComboTypeLabel(m_comboSelectPlayList, pPL->Name);
                }
                else
                {
                    AddComboItem(m_comboSelectPlayList, pPL->Name);
                }
                pPL->nIndex = nIndex++;
            }
        }

        // 如果没有设置在线直播，隐藏选项卡 
        if (m_comboSelectPlayList->GetCount() == 0)
        {
            if (m_PlayListWnd)
            {
                m_PlayListWnd->SetVisible(false);
            }

            if (m_LivePlayList)
            {
                m_LivePlayList->SetVisible(false);
            }
        }
    }


    //////////////////////////////////////////////////////////////////////////
    // 直播列表 
    if (m_PlayListWnd)
    {
        // 删除已有播放列表 
        while(TRUE)
        {
            // 0 是下拉选择框，略过 
            CTreeNodeUI* pNode = m_PlayListWnd->GetChildNode(1);
            if (pNode == NULL)
            {
                break;
            }
            m_PlayListWnd->Remove(pNode);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // 本地播放列表 
    if (m_LocalPlayList)
    {
        // 这里使用 RemoveAll() 会有问题  
        while(TRUE)
        {
            CTreeNodeUI* pNode = m_LocalPlayList->GetChildNode(0);
            if (pNode == NULL)
            {
                break;
            }
            m_LocalPlayList->Remove(pNode);
        }
    }

#ifdef _DEBUG
    if (m_PlayUrl)
    {
        m_PlayUrl->SetText(TEXT("http://cd3.kankandv.xyz/20230914/hyMHLemk/600kb/hls/index.m3u8"));
    }
#endif

}

// 窗口销毁 
LRESULT CDuiFrameWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_MyAPlayer.Release();
    return __super::OnDestroy(uMsg, wParam, lParam, bHandled);
}

CControlUI* CDuiFrameWnd::CreateControl( LPCTSTR pstrClassName )
{
    CDuiString     strXML;
    CDialogBuilder builder;

    if (_tcsicmp(pstrClassName, _T("Caption")) == 0)
    {
        strXML = _T("Caption.xml");
    }
    else if (_tcsicmp(pstrClassName, _T("PlayPanel")) == 0)
    {
        strXML = _T("PlayPanel.xml");
    }
    else if (_tcsicmp(pstrClassName, _T("Playlist")) == 0)
    {
        strXML = _T("Playlist.xml");
    }

    if (! strXML.IsEmpty())
    {
        CControlUI* pUI = builder.Create(strXML.GetData(), NULL, NULL, &m_PaintManager, NULL); // 这里必须传入m_PaintManager，不然子XML不能使用默认滚动条等信息。
        return pUI;
    }

    return NULL;
}

// 界面不同控件的单击事件 
void CDuiFrameWnd::OnClick( TNotifyUI& msg )
{
    CDuiString sCtrlName = msg.pSender->GetName();
    // 隐藏播放列表 
    if (sCtrlName == TEXT("btnPlaylistShow"))
    {
        
        if (m_PlayListBlock)
        {
            m_PlayListBlock->SetVisible(true);
            if (m_ShowPlayList) m_ShowPlayList->SetVisible(false);
            if (m_HidePlayList) m_HidePlayList->SetVisible(true);
        }
        
    }
    // 显示播放列表 
    else if (sCtrlName == TEXT("btnPlaylistHide"))
    {
        if (m_PlayListBlock)
        {
            m_PlayListBlock->SetVisible(false);
            if( m_ShowPlayList ) m_ShowPlayList->SetVisible(true);
            if (m_HidePlayList) m_HidePlayList->SetVisible(false);
        }
    }
    // 输入url开始播放 
    else if (sCtrlName == TEXT("btnRefresh"))
    {
        // 停止当前播放,设置新的播放 
        if (m_PlayUrl)
        {
            CDuiString sPlayUrl = m_PlayUrl->GetText();
            do 
            {
                if (sPlayUrl.GetLength() < 8)
                {
                    break;
                }

                if (sPlayUrl == TEXT("请输入播放地址"))
                {
                    break;
                }

                // m_current_play.clear();
                StartPlay(sPlayUrl.GetData());
            } while (FALSE);
        }
    }
    // 减速
    else if (sCtrlName == TEXT("btnFastBackward"))
    {
        TCHAR buf[12] = {0};
        // 视频后退10s 
        // 104
        m_play_speed -= 10; 
        wsprintf(buf, TEXT("%u"), m_play_speed);
        m_MyAPlayer.m_pAPlayer->SetConfig(104, _bstr_t(buf));

        swprintf(buf, TEXT("倍速:%0.1f"), m_play_speed/100.0f);
        m_playSpeedRegeLabel->SetText(buf);
    }
    // 加速 
    else if (sCtrlName == TEXT("btnFastForward"))
    {
        TCHAR buf[12] = {0};
        // 视频快进10s 
        m_play_speed += 10;
        wsprintf(buf, TEXT("%u"), m_play_speed);
        m_MyAPlayer.m_pAPlayer->SetConfig(104, _bstr_t(buf));

        
        swprintf(buf, TEXT("倍速:%0.1f"), m_play_speed/100.0f);
        m_playSpeedRegeLabel->SetText(buf);
    }
    // 停止播放 
    else if (sCtrlName == TEXT("btnStop"))
    {
        StopPlay();
    }
    // 播放 
    else if (sCtrlName == TEXT("btnPlay"))
    {
        HRESULT hRes = m_MyAPlayer.m_pAPlayer->GetState();
        if (hRes == PS_PAUSED)
        {
            StartPlay(NULL);
        }
        else
        {
            OnDlgSelectFile();
        }
    }
    // 暂停 
    else if (sCtrlName == TEXT("btnPause"))
    {
        PausePlay();
    }
    // 上一个  
    else if (sCtrlName == TEXT("btnPrevious"))
    {
        for (
            PLIST_ENTRY li = m_LocalList.next.Flink; 
            li != &m_LocalList.next;
            li = li->Flink
            )
        {
            LocalMovieList* lml = (LocalMovieList*)CONTAINING_RECORD(li, LocalMovieList, next);
            if (_tcscmp(m_current_play.c_str(), lml->HashStr) == 0 && li->Blink != &m_LocalList.next)
            {
                LocalMovieList* pPrev = (LocalMovieList*)CONTAINING_RECORD(li->Blink, LocalMovieList, next);
                StartPlay(pPrev->szPath);
                break;
            }
        }
    }
    // 下一个  
    else if (sCtrlName == TEXT("btnNext"))
    {
        for (
            PLIST_ENTRY li = m_LocalList.next.Flink; 
            li != &m_LocalList.next;
            li = li->Flink
            )
        {
            LocalMovieList* lml = (LocalMovieList*)CONTAINING_RECORD(li, LocalMovieList, next);
            if (_tcscmp(m_current_play.c_str(), lml->HashStr) == 0 && li->Flink != &m_LocalList.next)
            {
                LocalMovieList* pNext = (LocalMovieList*)CONTAINING_RECORD(li->Flink, LocalMovieList, next);
                StartPlay(pNext->szPath);
                break;
            }
        }
    }
    // 静音 
    else if (sCtrlName == TEXT("btnVolume"))
    {
        if (m_VolumeButton) m_VolumeButton->SetVisible(false);
        if (m_DisVolumeButon) m_DisVolumeButon->SetVisible(true); 
        m_MyAPlayer.m_pAPlayer->SetVolume(0);
    }
    // 恢复声音(取消静音) 
    else if (sCtrlName == TEXT("btnVolumeZero"))
    {
        if (m_VolumeButton) m_VolumeButton->SetVisible(true);
        if (m_DisVolumeButon) m_DisVolumeButon->SetVisible(false); 

        if (m_VolumeSlider)
        {
            int vol = m_VolumeSlider->GetValue();
            m_MyAPlayer.m_pAPlayer->SetVolume(vol);
        }
    }
    // 打开文件 
    else if (sCtrlName == TEXT("btnOpenMini"))
    {
        OnDlgSelectFile();
    }
    // 全屏 
    else if (sCtrlName == TEXT("btnScreenFull"))
    {
        SwitchFullScreen(WM_LBUTTONUP, msg.wParam, msg.lParam);
    }
    // 退出全屏 
    else if (sCtrlName == TEXT("btnScreenNormal"))
    {
        onFullScreen(FALSE);
    }
    __super::OnClick(msg);
}

// 两个搜索框获取焦点,删除默认文字 
void CDuiFrameWnd::OnSetFocus(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();
    if (sCtrlName == TEXT("editSearch"))
    {
        if (msg.pSender->GetText() == TEXT("请输入搜索关键词"))
        {
            msg.pSender->SetText(TEXT(""));
        }
    }
    else if (sCtrlName == TEXT("editURL"))
    {
        if (msg.pSender->GetText() == TEXT("请输入播放地址"))
        {
            msg.pSender->SetText(TEXT(""));
        }
    }
}

// 两个搜索框失去焦点,如果为空,则显示默认文字 
void CDuiFrameWnd::KillSetFocus(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();
    if (sCtrlName == TEXT("editSearch"))
    {
        if (msg.pSender->GetText().GetLength() == 0)
        {
            msg.pSender->SetText(TEXT("请输入搜索关键词"));
        }
    }
    else if (sCtrlName == TEXT("editURL"))
    {
        if (msg.pSender->GetText().GetLength() == 0)
        {
            msg.pSender->SetText(TEXT("请输入播放地址"));
        }
    }
}

// 下拉框中选择数据加载完成以后的更新在线播放列表处理 
DWORD WINAPI CDuiFrameWnd::SelectItemLoad(LPVOID lparam)
{
    PSelectComboPlayListParam pScombo = (PSelectComboPlayListParam)lparam;
    if (pScombo == NULL)
    {
        return 0;
    }

    FreePlayList(pScombo->cuiframe->m_live_playlist);
    if (pScombo->pPL->nCount > 0)
    {
        // 数据库 
        pScombo->cuiframe->m_live_playlist = GetVideoDBList(pScombo->pPL->nStart, pScombo->pPL->nCount);
    }
    else
    {
        // 文件解析 
        pScombo->cuiframe->m_live_playlist = GetLivePlayList(pScombo->pPL->szPath);
    }

    ::PostMessage(pScombo->cuiframe->GetHWND(), WM_UPSATE_LIVE, 0, 0);
    delete pScombo;
    return 0;
}

// 下拉框选择消息处理 
void CDuiFrameWnd::onSelectItem(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();
    if (sCtrlName == TEXT("comboPlaylist"))
    {
        CComboUI* pCombo = static_cast<CComboUI*>(msg.pSender);
        // 列表选择 
        CDuiString pSelect = pCombo->GetText();
        int iSelect = pCombo->GetCurSel();

        // 查找对应项 
        if (m_combo_playlist)
        {
            DWORD nIndex = 0;
            for (PLIST_ENTRY it = m_combo_playlist->next.Flink; 
                it != &m_combo_playlist->next; 
                it = it->Flink)
            {
                PComboPlayList pPL = CONTAINING_RECORD(it, ComboPlayList, next);
                if (pPL->nIndex == iSelect)
                {
                    PSelectComboPlayListParam ppram = new SelectComboPlayListParam;
                    ppram->pPL = pPL;
                    ppram->cuiframe = this;
                    HANDLE hThread = ::CreateThread(NULL, 0, SelectItemLoad, ppram, 0, NULL);
                    ::CloseHandle(hThread);
                    break;
                }
            }
        }
    }
}

// 播放列表中的双击视频选择 
void CDuiFrameWnd::onItemActive(TNotifyUI& msg)
{
    CDuiString sItemName = msg.pSender->GetName();
    // 从本地列表搜索 
    for (PLIST_ENTRY it = m_LocalList.next.Flink; 
        it != &m_LocalList.next;
        it = it->Flink)
    {
        LocalMovieList* lm = CONTAINING_RECORD(it, LocalMovieList, next);
        if (sItemName == lm->HashStr)
        {
            StartPlay(lm->szPath);
            return;
        }
    }

    if (m_live_playlist)
    {
        // 从网络列表搜索 
        for (PLIST_ENTRY it = m_live_playlist->next.Flink; 
            it != &m_live_playlist->next;
            it = it->Flink)
        {
            PLiveUrlList lm = CONTAINING_RECORD(it, LiveUrlList, next);
            if (sItemName == lm->HashStr)
            {
                m_PlayUrl->SetText(lm->szPath);
                StartPlay(lm->szPath);

                LPTSTR lpText = new TCHAR[MAX_PATH];
                if (lpText)
                {
                    wsprintf(lpText, TEXT("LmyPlayer播放器 - %s"), lm->Name);
                    m_captionText->SetText(lpText);
                    delete []lpText;
                }
                return;
            }

            if ( !_IsListEmpty(&lm->child) )
            {
                for (PLIST_ENTRY jt = lm->child.Flink; 
                    jt != &lm->child;
                    jt = jt->Flink)
                {
                    PLiveUrlList lchm = CONTAINING_RECORD(jt, LiveUrlList, next);
                    if (sItemName == lchm->HashStr)
                    {
                        m_PlayUrl->SetText(lchm->szPath);
                        StartPlay(lchm->szPath);

                        LPTSTR lpText = new TCHAR[MAX_PATH];
                        if (lpText)
                        {
                            wsprintf(lpText, TEXT("LmyPlayer播放器 - %s"), lchm->Name);
                            m_captionText->SetText(lpText);
                            delete []lpText;
                        }
                        return;
                    }                    
                }
            }
        }
    }
}

// 进度条修改消息处理 
void CDuiFrameWnd::onSLiderChange(TNotifyUI& msg)
{
    CDuiString sSilder = msg.pSender->GetName();
    if (sSilder == TEXT("sliderPlay"))
    {
        if (m_ProgressSlider)
        {
            long nPos = m_ProgressSlider->GetValue() * m_Movie_Secounds;
            m_MyAPlayer.m_pAPlayer->SetPosition(nPos);
        }
    }
    else if (sSilder == TEXT("sliderVol"))
    {
        // 声音变化，如果静音的要打开 
        if ( m_VolumeButton && !m_VolumeButton->IsVisible() )
        {
            m_VolumeButton->SetVisible(true);
            if (m_DisVolumeButon)
            {
                m_DisVolumeButon->SetVisible(false);
            }
        }

        if (m_VolumeSlider)
        {
            int volPos = m_VolumeSlider->GetValue();
            m_MyAPlayer.m_pAPlayer->SetVolume(volPos);
        }
    }
}

// 初始时添加APlayer窗口 
BOOL CDuiFrameWnd::AttachAplayerWnd()
{
    BOOL bRes = FALSE;
    do 
    {
        // 加载dll 
        CDuiString sAPlayerPath = CPaintManagerUI::GetInstancePath() + TEXT("APlayer.dll");
        HMODULE hAPlayerModule = 
            ::LoadLibraryEx(sAPlayerPath.GetData(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
        if (hAPlayerModule == NULL)
        {
            break;
        }

        // 获取对象接口 
        typedef HRESULT (STDAPICALLTYPE *_pfnDllGetClassObject)(REFCLSID, REFIID, LPVOID*);
        _pfnDllGetClassObject pfnDllGetClassObject = (_pfnDllGetClassObject)
            GetProcAddress(hAPlayerModule, "DllGetClassObject");
        if(pfnDllGetClassObject == NULL) break;

        // 得到APlayer对象 
        CComPtr<IClassFactory> spCF;
        HRESULT hr = (*pfnDllGetClassObject)(__uuidof(APlayer3Lib::Player), IID_IClassFactory, (LPVOID*)&spCF);
        if(FAILED(hr)) break;


        hr = spCF->CreateInstance(NULL, __uuidof(APlayer3Lib::IPlayer), (void **)&m_MyAPlayer.m_pAPlayer);
        if(FAILED(hr))
            break;

        HWND hChildWnd = ::CreateWindowEx(WS_EX_TOOLWINDOW, _T(ATLAXWIN_CLASS), _T(""),
            WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
            GetHWND(), NULL, NULL, NULL);
        m_MyAPlayer.SetPlayerHwnd(hChildWnd);

        hr = AtlAxAttachControl(m_MyAPlayer.m_pAPlayer, hChildWnd, NULL);
        CComQIPtr<IConnectionPointContainer> spContainer;
        if (SUCCEEDED(hr))
        {
            hr = m_MyAPlayer.m_pAPlayer->QueryInterface(__uuidof(IConnectionPointContainer), 
                (void **)&spContainer);
        }

        if (SUCCEEDED(hr))
        {
            hr = spContainer->FindConnectionPoint(__uuidof(APlayer3Lib::_IPlayerEvents), 
                &m_MyAPlayer.m_pConnectionPoint);
        }

        if (SUCCEEDED(hr))
        {
            m_MyAPlayer.m_APlayerSink.SetEventHandler(&m_MyAPlayer);
            hr = m_MyAPlayer.m_pConnectionPoint->Advise(&m_MyAPlayer.m_APlayerSink, &m_MyAPlayer.m_dwCookie);
        }

        CDuiString sLogoPng = CPaintManagerUI::GetInstancePath() + TEXT("skin\\bg_logo.bmp");
        HBITMAP hBitMap = (HBITMAP)LoadImage(NULL, sLogoPng.GetData(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE);
        m_MyAPlayer.m_pAPlayer->SetCustomLogo((LONG)hBitMap);

        bRes = TRUE;
    } while (FALSE);

    return bRes;
}

// 窗口大小变化 
void CDuiFrameWnd::onWindowSize(TNotifyUI& msg)
{
    if (m_PlayerClientDlg)
    {
        RECT rect = m_PlayerClientDlg->GetClientPos();
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        if (m_MyAPlayer.m_pAPlayer == NULL)
        {
            // 控制面板高度 
            if (m_PlayControlPanel)
            {
                RECT rect = m_PlayControlPanel->GetClientPos();
                m_PlayControlHeight = rect.bottom - rect.top;
            }
            // 标题栏高度 
            if (m_CaptionPanel)
            {
                RECT rect = m_CaptionPanel->GetClientPos();
                m_CaptionHeight = rect.bottom - rect.top;
            }
            // 播放列表宽度 
            if (m_PlayListBlock)
            {
                RECT rect = m_PlayListBlock->GetClientPos();
                m_PlayListWidth = rect.right - rect.left;
            }

            if ( AttachAplayerWnd() )
            {
                // 初始设置 
                if (m_VolumeSlider)
                {
                    int volPos = m_VolumeSlider->GetValue();
                    m_MyAPlayer.m_pAPlayer->SetVolume(volPos);
                }
            }
            else
            {
                MsgBox(GetHWND(), TEXT("加载APlayer失败！"), TEXT("LmyPlayer播放器"));
            }
        }
        
        if (m_MyAPlayer.GetPlayerHwnd())
        {
            ::SetWindowPos(m_MyAPlayer.GetPlayerHwnd(), 
                HWND_TOP, 
                rect.left + 1, 
                rect.top + 1, 
                rect.right - rect.left - 4, // 右边要留一定像素让鼠标缩放窗口 
                rect.bottom - rect.top - 2, 
                SWP_SHOWWINDOW);
        }
    }
}

void CDuiFrameWnd::Notify( TNotifyUI& msg )
{
    __super::Notify(msg);
}

// 拖拽文件处理 
LRESULT CDuiFrameWnd::OnDropFiles(HDROP hDropInfo)
{
    LPTSTR lpFilePath = new TCHAR[MAX_PATH];
    if (lpFilePath)
    {
        UINT nCount = DragQueryFile(hDropInfo, -1, NULL, 0);
        for (UINT i=0; i<nCount; i++)
        {
            DragQueryFile(hDropInfo, i, lpFilePath, MAX_PATH);

            // 视频处于停止状态才播放第一个文件
            if (i == 0 && m_MyAPlayer.m_pAPlayer->GetState() == PS_READY)
            {
                StartPlay(lpFilePath);
            }

            // 加入本地播放列表 
            LocalMovieList* pM = new LocalMovieList;
            if (pM)
            {
                memset(pM, 0, sizeof(LocalMovieList));
                _tcscpy(pM->szPath, lpFilePath);
                LPCTSTR lpName = PathFindFileName(lpFilePath);
                _tcscpy(pM->Name, lpName);
                PathRemoveExtension(pM->Name);

                CalcMd5((PBYTE)pM->szPath, _tcslen(pM->szPath)*sizeof(TCHAR), pM->HashStr);
                _InsertTailList(&m_LocalList.next, &pM->next);
            }
        }
        DragFinish(hDropInfo);

        PostMessage(WM_UNDATE_LOCAL);
        delete []lpFilePath;
    }
    return NOERROR;
}

// 更新本地播放列表 
LRESULT CDuiFrameWnd::OnUpdateLocalFile()
{
    if (m_LocalPlayList == NULL)
    {
        return 0;
    }

    // 这里使用 RemoveAll() 会有问题  
    while(TRUE)
    {
        CTreeNodeUI* pNode = m_LocalPlayList->GetChildNode(0);
        if (pNode == NULL)
        {
            break;
        }
        m_LocalPlayList->Remove(pNode);
    }

    for (PLIST_ENTRY p = m_LocalList.next.Flink; 
        p != &m_LocalList.next;
        p = p->Flink)
    {
        LocalMovieList* plml = CONTAINING_RECORD(p, LocalMovieList, next);
        AddLivePlayerNode(m_LocalPlayList, plml->Name, plml->HashStr);
    }
    return 0;
}

// 更新在线播放列表 
LRESULT CDuiFrameWnd::OnUpdateLiveList()
{ 
    if (m_PlayListWnd == NULL)
    {
        return 0;
    }

    // 删除已有播放列表 
    while(TRUE)
    {
        // 0 是下拉选择框，略过 
        CTreeNodeUI* pNode = m_PlayListWnd->GetChildNode(1);
        if (pNode == NULL)
        {
            break;
        }
        m_PlayListWnd->Remove(pNode);
    }

    // 放入新数据 
    if(m_live_playlist && !_IsListEmpty(&m_live_playlist->next))
    {
        for (PLIST_ENTRY it = m_live_playlist->next.Flink; 
            it != &m_live_playlist->next;
            it = it->Flink)
        {
            PLiveUrlList pLiveUrl = CONTAINING_RECORD(it, LiveUrlList, next);
            if (pLiveUrl->szPath[0] == TEXT('\0'))
            {
                // 添加类型 
                CTreeNodeUI* pNewDemoNode1 = AddLivePlayerTypeNode(m_PlayListWnd, pLiveUrl->Name);
                // 遍历子项 
                for (PLIST_ENTRY child = pLiveUrl->child.Flink;
                    child != &pLiveUrl->child;
                    child = child->Flink)
                {
                    PLiveUrlList pChildLiveList = CONTAINING_RECORD(child, LiveUrlList, next);
                    AddLivePlayerNode(pNewDemoNode1, pChildLiveList->Name, pChildLiveList->HashStr);
                }

//                 CTreeViewUI* treeview = static_cast<CTreeViewUI*>(m_pm.FindControl(TEXT("treeview_item")));
//                 if (treeview)
//                 {
//                     treeview->SetItemExpand(false, pNewDemoNode1);
//                 }
            }
            else
            {
                if (pLiveUrl->Name[0] != TEXT('\0'))
                {
                    AddLivePlayerNode(m_PlayListWnd, pLiveUrl->Name, pLiveUrl->HashStr);
                }
            }
        }
    }
    return 0;
}

// 播放时定时同步进度 
LRESULT CDuiFrameWnd::OnPlayTimer()
{
    // 禁止系统休眠 
    if (SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED) == NULL)
    {
        SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
    }

    m_Movie_Secounds = m_MyAPlayer.m_pAPlayer->GetDuration();
    m_Movie_Secounds = m_Movie_Secounds/1000;
    if (m_Movie_Secounds == 0)
    {
        ::KillTimer(GetHWND(), ID_PLAYTIMER);
        m_ProgressBlock->SetEnabled(false);
        return 0;
    }

    POINT pt;
    GetCursorPos(&pt);
    if (pt.x == m_mouse_pos.x && pt.y == m_mouse_pos.y)
    {
        if (time(NULL) > m_mouse_time+3)
        {
            OnShowCursor(FALSE);
        }
    }
    else
    {
        m_mouse_time = time(NULL);
        m_mouse_pos.x = pt.x;
        m_mouse_pos.y = pt.y;
    }
    

    if (!m_ProgressBlock->IsEnabled())
    {
        m_ProgressBlock->SetEnabled(true);
    }

    int totalHours = m_Movie_Secounds/60/60;
    int totalMinutes = m_Movie_Secounds/60%60;
    int totalSecounds = m_Movie_Secounds%60;

    LONG nDuration = m_MyAPlayer.m_pAPlayer->GetPosition(); 
    nDuration = nDuration/1000;
    int posHours = nDuration/60/60;
    int posMinutes = nDuration/60%60;
    int posSecounds = nDuration%60;

    TCHAR TimeBuf[64] = {0};
    if (totalHours > 0)
    {
        wsprintf(TimeBuf, TEXT("%02u:%02u:%02u/%02u:%02u:%02u"), 
            posHours, posMinutes, posSecounds, 
            totalHours, totalMinutes, totalSecounds);
    }
    else
    {
        wsprintf(TimeBuf, TEXT("%02u:%02u/%02u:%02u"), 
            posMinutes, posSecounds, 
            totalMinutes, totalSecounds);
    }
    m_playProgressLabel->SetText(TimeBuf);

    if (m_Movie_Secounds)
    {
        int nPos = nDuration*1000/m_Movie_Secounds;
        m_ProgressSlider->SetValue(nPos);
    }

    return 0;
}

// 选择视频文件 
LRESULT CDuiFrameWnd::OnDlgSelectFile()
{
    LPTSTR lpFileName = new TCHAR[1024];
    LPTSTR lpMediaFormat = new TCHAR[1024];
    if (lpFileName && lpMediaFormat)
    {
        memset(lpFileName, 0, sizeof(TCHAR)*1024);
        memset(lpMediaFormat, 0, sizeof(TCHAR)*1024);

        DWORD nLen = 0;
        lstrcpy(lpMediaFormat, _T("支持的媒体文件"));
        nLen = lstrlen(lpMediaFormat) + 1;
        lstrcpy(lpMediaFormat + nLen, _T("*.wmv;*.wmp;*.wm;*.asf;*.wma;*.avi;*.wav;*.mpg;*.mpeg;*.dat;"));
        lstrcat(lpMediaFormat + nLen, _T("*.ts;*.mpa;*.mp2;*.vob;*.ifo;*.mp3;*.mid;*.ogm;*.ogg;*.cda;"));
        lstrcat(lpMediaFormat + nLen, _T("*.d2v;*.mp4;*.3gp;*.mkv;*.rm;*.ram;*.rmvb;*.rpm;*.ra;*.mov;"));
        lstrcat(lpMediaFormat + nLen, _T("*.qt;*.amr;*.mpc;*.flv;*.swf;"));
        lstrcat(lpMediaFormat + nLen, _T("*.evo;*.tta;*.m4b;"));
        lstrcat(lpMediaFormat + nLen, _T("*.xv;*.xvx;*.xlmv"));
        nLen = nLen + lstrlen(lpMediaFormat + nLen) + 1;
        lstrcpy(lpMediaFormat + nLen, _T("所有文件(*.*)"));
        nLen = nLen + lstrlen(lpMediaFormat + nLen) + 1;
        lstrcpy(lpMediaFormat + nLen, _T("*.*"));

        OPENFILENAME ofn = { sizeof(OPENFILENAME) };
        ofn.hwndOwner = GetHWND();
        ofn.lpstrTitle = _T("选择媒体文件");
        ofn.lpstrFilter = lpMediaFormat;
        ofn.nFilterIndex = 1;
        ofn.lpstrFile = lpFileName;
        ofn.nMaxFile = 1024;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ALLOWMULTISELECT;

        if ( ::GetOpenFileName(&ofn) )
        {
            LPTSTR lpSignalPath = new TCHAR[MAX_PATH];
            if (lpSignalPath)
            {
                LPTSTR lpPosFileName = (LPTSTR)lpFileName;
                _tcscpy(lpSignalPath, lpPosFileName);
                PathAddBackslash(lpSignalPath);
                lpPosFileName = lpPosFileName + _tcslen(lpPosFileName) + 1;
                DWORD nPos = _tcslen(lpSignalPath);
                _tcscpy(lpSignalPath + nPos, lpPosFileName);
                if (m_MyAPlayer.m_pAPlayer->GetState() == PS_READY)
                {
                    StartPlay(lpSignalPath);
                }
                while (TRUE)
                {
                    // 加入本地播放列表 
                    LocalMovieList* pM = new LocalMovieList;
                    if (pM)
                    {
                        memset(pM, 0, sizeof(LocalMovieList));
                        _tcscpy(pM->szPath, lpSignalPath);
                        LPCTSTR lpName = PathFindFileName(lpSignalPath);
                        _tcscpy(pM->Name, lpName);
                        PathRemoveExtension(pM->Name);

                        CalcMd5((PBYTE)pM->szPath, _tcslen(pM->szPath)*sizeof(TCHAR), pM->HashStr);
                        _InsertTailList(&m_LocalList.next, &pM->next);
                    }

                    lpPosFileName = lpPosFileName + _tcslen(lpPosFileName) + 1;
                    if (*lpPosFileName)
                    {
                        _tcscpy(lpSignalPath + nPos, lpPosFileName);
                    }
                    else
                    {
                        break;
                    }
                }
                delete []lpSignalPath;
            }
            PostMessage(WM_UNDATE_LOCAL);
        }
    }
    delete []lpMediaFormat;
    delete []lpFileName;
    return 0;
}

// 播放控件上的鼠标移动事件 
LRESULT CDuiFrameWnd::OnAPlayerMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // 全屏时候，鼠标移动显示标题栏 或者 控制播放栏 
    RECT rect;
    ::GetWindowRect(GetHWND(), &rect);

    // 鼠标移动要显示鼠标 
    OnShowCursor(TRUE);
    m_mouse_time = time(NULL);
    GetCursorPos(&m_mouse_pos);

    POINT ParentPoint;
    ParentPoint.x = rect.left + (rect.right-rect.left)/2;
    ParentPoint.y = rect.top + (rect.bottom-rect.top)/2;
    HMONITOR hMonitor = MonitorFromPoint(ParentPoint, MONITOR_DEFAULTTONULL);
    MONITORINFO mi = { sizeof(MONITORINFO) };
    if (GetMonitorInfo(hMonitor, &mi))
    {
        // 检查是全屏状态 
        if (
            mi.rcMonitor.right - mi.rcMonitor.left <= rect.right - rect.left &&
            mi.rcMonitor.bottom - mi.rcMonitor.top <= rect.bottom - rect.top
            )
        {
            int xPos = GET_X_LPARAM(lParam); 
            int yPos = GET_Y_LPARAM(lParam); 
            if (yPos <= m_CaptionHeight)
            {
                // 显示标题栏 
                if (!m_CaptionPanel->IsVisible())
                {
                    m_CaptionPanel->SetVisible(true);
                }
            }
            else if (yPos >= rect.bottom-m_PlayControlHeight)
            {
                // 显示控制栏 
                if (!m_PlayControlPanel->IsVisible())
                {
                    m_PlayControlPanel->SetVisible(true);
                }
            }
            else if (xPos <= m_PlayListWidth )
            {
                if (!m_PlayListBlock->IsVisible())
                {
                    m_PlayListBlock->SetVisible(true);
                }
            }
            else
            {
                if (m_PlayControlPanel->IsVisible())
                {
                    m_PlayControlPanel->SetVisible(false);
                }
                if (m_CaptionPanel->IsVisible())
                {
                    m_CaptionPanel->SetVisible(false);
                }
                if (m_PlayListBlock->IsVisible())
                {
                    m_PlayListBlock->SetVisible(false);
                }
            }
        }
    }

    return 0;
}

// APlayer 状态变更消息处理 
LRESULT CDuiFrameWnd::OnAPlayerStateChange(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case PS_READY:
        {
            // 播放完或者关闭了 
            SetThreadExecutionState(ES_CONTINUOUS);
            m_captionText->SetText(TEXT("LmyPlayer播放器"));
            m_Movie_Secounds = 0;

            _bstr_t szRes = m_MyAPlayer.m_pAPlayer->GetConfig(7);
            DWORD dwRes = strtoul(szRes, NULL, 10);
            if (dwRes == 0)
            {
                // 播放完成 
                for (
                    PLIST_ENTRY li = m_LocalList.next.Flink; 
                    li != &m_LocalList.next;
                    li = li->Flink
                    )
                {
                    LocalMovieList* lml = (LocalMovieList*)CONTAINING_RECORD(li, LocalMovieList, next);
                    if (_tcscmp(m_current_play.c_str(), lml->HashStr) == 0 && li->Flink != &m_LocalList.next)
                    {
                        LocalMovieList* pnextlml = (LocalMovieList*)CONTAINING_RECORD(li->Flink, LocalMovieList, next);
                        StartPlay(pnextlml->szPath);
                        return 0;
                    }
                }
            }
            else if (dwRes == 1)
            {
                // 主动关闭 
            }
            else
            {
                // 播放失败,错误码dwRes  
            }
        }
        break;
    case PS_PAUSED:
        {
            // 播放按钮隐藏，变成暂停按钮 
            if (m_StartButton)
            {
                m_StartButton->SetVisible(true);
            }
            if (m_PauseButton)
            {
                m_PauseButton->SetVisible(false);
            }
            ::KillTimer(GetHWND(), ID_PLAYTIMER);
        }
        break;
    case PS_PLAYING:
        {
            SetTimer(GetHWND(), ID_PLAYTIMER, 300, NULL);

            // 进度
            TCHAR buf[12] = {0};
            wsprintf(buf, TEXT("%u"), m_play_speed);
            m_MyAPlayer.m_pAPlayer->SetConfig(104, _bstr_t(buf));

            // 停止按钮可用
            if (m_StopButton && !m_StopButton->IsEnabled())
            {
                m_StopButton->SetEnabled(true);
            }

            // 显示进度信息 
            if (m_ProgressBlock && !m_ProgressBlock->IsVisible())
            {
                m_ProgressBlock->SetVisible(true);
            }

            // 播放按钮隐藏，变成暂停按钮 
            if (m_StartButton)
            {
                m_StartButton->SetVisible(false);
            }
            if (m_PauseButton)
            {
                m_PauseButton->SetVisible(true);
            }
        }
        break;
    case PS_CLOSING:
        {
            ::KillTimer(GetHWND(), ID_PLAYTIMER);

            // 播放按钮隐藏，变成暂停按钮 
            if (m_StartButton)
            {
                m_StartButton->SetVisible(true);
            }
            if (m_PauseButton)
            {
                m_PauseButton->SetVisible(false);
            }

            // 隐藏进度信息 
            if (m_ProgressBlock && m_ProgressBlock->IsVisible())
            {
                m_ProgressBlock->SetVisible(false);
            }

            // 禁用停止按钮
            if (m_StopButton && m_StopButton->IsEnabled())
            {
                m_StopButton->SetEnabled(false);
            }
        }
        break;
    }

    return 0;
}

// 全屏切换 
LRESULT CDuiFrameWnd::SwitchFullScreen(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT rect;
    ::GetWindowRect(GetHWND(), &rect);

    POINT ParentPoint;
    ParentPoint.x = rect.left + (rect.right-rect.left)/2;
    ParentPoint.y = rect.top + (rect.bottom-rect.top)/2;
    HMONITOR hMonitor = MonitorFromPoint(ParentPoint, MONITOR_DEFAULTTONULL);
    MONITORINFO mi = { sizeof(MONITORINFO) };
    if (GetMonitorInfo(hMonitor, &mi))
    {
        if (
            mi.rcWork.right - mi.rcWork.left > rect.right - rect.left &&
            mi.rcWork.bottom - mi.rcWork.top > rect.bottom - rect.top
            )
        {
            onFullScreen(TRUE, &mi.rcMonitor);
        }
        else
        {
            onFullScreen(FALSE);
        }
    }

    return 0;
}

// 最底层的消息处理 
LRESULT CDuiFrameWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    LRESULT lRes = __super::HandleMessage(uMsg, wParam, lParam);

    switch (uMsg)
    {
    case WM_KEYDOWN:
        OnKeyClick(uMsg, wParam, lParam);
        break;
    case WM_TIMER:
        if (wParam == ID_LBUTTONCLICK)
        {
            KillTimer(GetHWND(), ID_LBUTTONCLICK);

            HRESULT state = m_MyAPlayer.m_pAPlayer->GetState();
            if (state == PS_PAUSED)
            {
                StartPlay(NULL);
            }
            else if (state == PS_PLAY)
            {
                PausePlay();
            }
        }
        else if (wParam == ID_PLAYTIMER)
        {
            OnPlayTimer();
        }
        break;

    case WM_MOUSEMOVE:
        OnMouseMove(uMsg, wParam, lParam);
        break;

    case WM_LBUTTONUP:
        if (GetCapture() == GetHWND())
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            ReleaseCapture();
            // 在播放控件窗口，属于单击播放 
            RECT rect = m_PlayerClientDlg->GetClientPos();
            GetClientRect(GetHWND(), &rect);
            if (x >= rect.left && x <= rect.right &&
                y >= rect.top && y <= rect.bottom)
            {
                // 先延时处理,如果后面再有点击是双击操作,则不处理单击事件 
                // 一般双击之间是500ms,但是真按500ms,会感觉明细的延迟感 
                SetTimer(GetHWND(), ID_LBUTTONCLICK, 300, NULL);
            }
        }
        break;

    case WM_DROPFILES: 
        lRes = OnDropFiles((HDROP) wParam);
        break;

    case WM_PLAYERSTATUS:
        if (wParam == PER_DOWNLOADCODEC)
        {
            BSTR lpPath = (BSTR)lParam;

            TCHAR szText[MAX_PATH];
            TCHAR szPath[MAX_PATH];
            _tcscpy(szPath, W2T(lpPath));
            PathRemoveFileSpec(szPath);
            wsprintf(szText, 
                _T("缺少解码器 %s, \n下载后解压放在 %s 文件夹下。\n下载地址：http://aplayer.open.xunlei.com/codecs.zip\n\n"), 
                W2T(lpPath), 
                szPath);
            MsgBox(GetHWND(), szText, TEXT("LmyPlayer播放器"));
        }
        break;

    case WM_PLAYSTATECHANGE:
        OnAPlayerStateChange(uMsg, wParam, lParam);
        break;
    case WM_PLAYLBUTTONDOWN:
        // 移出按钮也要监控鼠标消息
        SetCapture( GetHWND() );
        break;
    case WM_PLAYLBUTTONUP:
        break;
    case WM_PLAYLBUTTONDCLICK:
        {
            // 现关闭之前左键消息，直接处理双击 
            KillTimer(GetHWND(), ID_LBUTTONCLICK);
            SwitchFullScreen(uMsg, wParam, lParam);            
        }
        break;
    case WM_PLAYKEYDOWN:
        OnKeyClick(uMsg, wParam, lParam);
        break;
    case WM_PLAYMOUSEMOVE:
        OnAPlayerMouseMove(uMsg, wParam, lParam);
        break;
    case WM_UNDATE_LOCAL:
        OnUpdateLocalFile();
        break;
    case WM_UPSATE_LIVE:
        // 在线播放列表可能太长，(卡顿问题)暂停定时
        ::KillTimer(GetHWND(), ID_PLAYTIMER);
        OnUpdateLiveList();
        if (m_MyAPlayer.m_pAPlayer->GetState() == PS_PLAY)
        {
            SetTimer(GetHWND(), ID_PLAYTIMER, 300, NULL);
        }
        break;
    default:
        break;
    }

    return lRes;
}

// 系统标题栏检测 
LRESULT CDuiFrameWnd::OnNcHitTest( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    return __super::OnNcHitTest(uMsg, wParam, lParam, bHandled);
}

// 键盘按键处理 
LRESULT CDuiFrameWnd::OnKeyClick(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Ctrl + Enter 全屏 
    if (wParam == VK_RETURN)
    {
        if (::GetKeyState(VK_CONTROL) < 0)
        {
            if (m_FullScreenBtn && m_FullScreenBtn->IsVisible() == true)
            {
                SwitchFullScreen(WM_KEYDOWN, wParam, 0);
            }
        }
    }
    // Esc 退出全屏 
    else if (wParam == VK_ESCAPE)
    {
        if (m_FullScreenBtn && m_FullScreenBtn->IsVisible() == false)
        {
            onFullScreen(FALSE);
        }
        return TRUE;
    }
    // 空格键 暂停恢复播放 
    else if (wParam == VK_SPACE)
    {
        HRESULT state = m_MyAPlayer.m_pAPlayer->GetState();
        if (state == PS_PAUSED)
        {
            StartPlay(NULL);
        }
        else if (state == PS_PLAY)
        {
            PausePlay();
        }
        else if (state == PS_READY)
        {
            OnDlgSelectFile();
        }
    }
    // ← 退5s 
    else if (wParam == VK_LEFT)
    {
        LONG nDuration = m_MyAPlayer.m_pAPlayer->GetPosition(); 
        if (nDuration > 0)
        {
            LONG offset = 5*1000;
            if (m_MyAPlayer.m_pAPlayer->GetState() == PS_PLAY)
            {
                offset += 200;
            }
            
            if (nDuration < offset)
            {
                nDuration = 0;
            }
            else
            {
                nDuration -= offset;
            }

            m_MyAPlayer.m_pAPlayer->SetPosition(nDuration); 
        }
        nDuration = nDuration/1000;
    }
    // ↑ 音量加 
    else if (wParam == VK_UP)
    {
        int volPos = m_VolumeSlider->GetValue();
        if (volPos < 195)
        {
            volPos += 5;
        }
        else
        {
            volPos = 100;
        }
        m_VolumeSlider->SetValue(volPos);
        m_MyAPlayer.m_pAPlayer->SetVolume(volPos);
    }
    // → 进5s 
    else if (wParam == VK_RIGHT)
    {
        LONG nDuration = m_MyAPlayer.m_pAPlayer->GetPosition(); 
        if (nDuration > 0)
        {
            LONG offset = 5*1000;
            if (m_MyAPlayer.m_pAPlayer->GetState() == PS_PLAY)
            {
                offset -= 200;
            }

            nDuration += offset;
            LONG nTotalLength = m_MyAPlayer.m_pAPlayer->GetDuration();
            if (nDuration > nTotalLength)
            {
                nDuration = nTotalLength;
            }

            m_MyAPlayer.m_pAPlayer->SetPosition(nDuration); 
        }
        nDuration = nDuration/1000;
    }
    // ↓ 音量减 
    else if (wParam == VK_DOWN)
    {
        int volPos = m_VolumeSlider->GetValue();
        if (volPos > 5)
        {
            volPos -= 5;
        }
        else
        {
            volPos = 0;
        }
        m_VolumeSlider->SetValue(volPos);
        m_MyAPlayer.m_pAPlayer->SetVolume(volPos);
    }

    return 0;
}

// 窗口大小变化 
LRESULT CDuiFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    CDuiRect rcWnd;
    // 计算当前窗口大小 
    ::GetWindowRect(*this, &rcWnd);
    int width = rcWnd.right - rcWnd.left;
    int height = rcWnd.bottom - rcWnd.top;

    // 窗口标题名字随着窗口大小变化长短变化 
    if ( m_pm.IsValid() )
    {
        if (m_captionText)
        {
            m_captionText->SetFixedWidth( width - 40 - 162 );
        }
    }

    return __super::OnSize(uMsg, wParam, lParam, bHandled);
}

// 鼠标悬停消息处理 
LRESULT CDuiFrameWnd::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL bHideToolTip = TRUE;
    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    if (m_ToolMousePt.x == pt.x && m_ToolMousePt.y == pt.y)
    {
        return 0;
    }
    m_ToolMousePt.x = pt.x;
    m_ToolMousePt.y = pt.y;

    CControlUI* pHover = m_pm.FindControl(pt);
    do 
    {
        if (!pHover)
        {
            break;
        }
        CDuiString sClass = pHover->GetClass();
        CDuiString sName = pHover->GetName();
        // 这里只在sliderPlay进度条显示进度信息 
        if (sClass == TEXT("SliderUI"))
        {
            CSliderUI* pSliderPlay = static_cast<CSliderUI*>(pHover);
            RECT rect = pSliderPlay->GetClientPos();
            rect.left += 3;
            rect.right -= 3;

            // 根据进度条长度,鼠标所在位置占比 
            int width = rect.right - rect.left - 4;
            if (pt.x < rect.left)
            {
                pt.x = rect.left;
            }
            if (pt.y > rect.right)
            {
                pt.y = rect.right;
            }
            int pos = (pt.x - rect.left) * 100 / width;

            // 区分那个进度条 
            DWORD bMaxPos = -1;
            // 音量控件 
            if (sName == TEXT("sliderVol"))
            {
                bMaxPos = 100;

                int vol = pos * bMaxPos / 100;
                if (vol>100)
                {
                    vol = 100;
                }
                wsprintf(m_ToolTip.lpszText, TEXT("%u%%"), vol);

                if ( m_VolumeSlider == m_pm.GetFocus() && GetKeyState(VK_LBUTTON)<0 )
                {
                    int volPos = m_VolumeSlider->GetValue();
                    m_MyAPlayer.m_pAPlayer->SetVolume(volPos);
                }
            }
            // 进度条控件 
            else if (sName == TEXT("sliderPlay"))
            {
                bMaxPos = m_Movie_Secounds;

                int itime = pos * bMaxPos / 100;
                int hour = itime/60/60;
                int secound = itime/60%60;
                int minute = itime%60;
                if (hour>0)
                {
                    wsprintf(m_ToolTip.lpszText, TEXT("%02u:%02u:%02u"), hour, secound, minute);
                }
                else
                {
                    wsprintf(m_ToolTip.lpszText, TEXT("%02u:%02u"), secound, minute);
                }
            }

            if (bMaxPos == -1)
            {
                break;
            }
            
            if (m_hwndSlidertip != NULL)
            {
                // 更新文本 
                ::SendMessage(m_hwndSlidertip, TTM_UPDATETIPTEXT, 0, (LPARAM) &m_ToolTip);
            }
            else
            {
                m_ToolTip.cbSize = sizeof(TOOLINFO);
                m_ToolTip.uFlags = TTF_IDISHWND;
                m_ToolTip.hwnd = m_pm.GetPaintWindow();
                m_ToolTip.uId = (UINT_PTR) m_pm.GetPaintWindow();
                m_ToolTip.hinst = m_pm.GetInstance();
                m_ToolTip.rect = rect;

                // 创建显示文本 
                m_hwndSlidertip = ::CreateWindowEx( 
                    0, 
                    TOOLTIPS_CLASS, 
                    NULL, 
                    WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, 
                    CW_USEDEFAULT, 
                    CW_USEDEFAULT, 
                    CW_USEDEFAULT, 
                    CW_USEDEFAULT, 
                    m_pm.GetPaintWindow(), 
                    NULL, 
                    m_pm.GetInstance(), 
                    NULL);

                ::SendMessage(m_hwndSlidertip, TTM_ADDTOOL, 0, (LPARAM) &m_ToolTip);
                ::SendMessage(m_hwndSlidertip,TTM_SETMAXTIPWIDTH,0, pHover->GetToolTipWidth());
            }
            
            // 如果已经创建过,当前是隐藏的,直接显示就好 
            if(!::IsWindowVisible(m_hwndSlidertip))
            {
                ::SendMessage(m_hwndSlidertip, TTM_SETTOOLINFO, 0, (LPARAM)&m_ToolTip);
                ::SendMessage(m_hwndSlidertip, TTM_TRACKACTIVATE, TRUE, (LPARAM)&m_ToolTip);
            }

            bHideToolTip = FALSE;
        }
        
    } while (FALSE);

    // 鼠标移出进度条区域隐藏 
    if (bHideToolTip)
    {
        if (m_hwndSlidertip && ::IsWindowVisible(m_hwndSlidertip))
        {
            ::SendMessage(m_hwndSlidertip, TTM_TRACKACTIVATE, FALSE, (LPARAM)&m_ToolTip);
        }
    }

    return 0;
}

// 开始播放 
BOOL CDuiFrameWnd::StartPlay(LPCTSTR lpMedia)
{
    BOOL bPlay = FALSE;
    if (lpMedia)
    {
        LPTSTR lpMd5 = new TCHAR[34];
        if (lpMd5)
        {
            CalcMd5((PBYTE)lpMedia, _tcslen(lpMedia)*sizeof(TCHAR), lpMd5);
            m_current_play = lpMd5;
        }

        if (S_OK == m_MyAPlayer.m_pAPlayer->Open(lpMedia))
        {
            KillTimer(GetHWND(), ID_PLAYTIMER);
            m_Movie_Secounds = 0;
            m_playProgressLabel->SetText(TEXT(""));
            m_ProgressSlider->SetValue(0);
            bPlay = TRUE;
            m_MyAPlayer.m_pAPlayer->SetConfig(503, TEXT(""));

            if (memicmp(lpMedia, TEXT("http"), sizeof(TCHAR)*4) != 0)
            {
                // 非网络直播，设置标题栏名字 
                TCHAR* lpText = new TCHAR[MAX_PATH];
                do 
                {
                    if (lpText == NULL)
                    {
                        break;
                    }

                    LPTSTR lpFileName = PathFindFileName(lpMedia);
                    wsprintf(lpText, TEXT("LmyPlayer播放器 - %s"), lpFileName);
                    m_captionText->SetText(lpText);

                    // 搜索同名的字幕  
                    _tcscpy(lpText, lpMedia);
                    LPTSTR lpExt = PathFindExtension(lpText);
                    lpExt++;
                    // 获取支持字幕格式 
                    _bstr_t formats = m_MyAPlayer.m_pAPlayer->GetConfig(502);
                    TCHAR* lpformat = (TCHAR*)formats;
                    int nInx = 0;
                    while(*lpformat)
                    {
                        if (*lpformat == TEXT('\0'))
                        {
                            break;
                        }
                        if (*lpformat == TEXT(';'))
                        {
                            lpExt[nInx] = TEXT('\0');
                            if ( PathFileExists(lpText) )
                            {
                                m_MyAPlayer.m_pAPlayer->SetConfig(503, lpText);
                                break;
                            }
                            nInx = 0;
                        }
                        else
                        {
                            lpExt[nInx++] = *lpformat;
                        }

                        lpformat++;
                    }
                } while (false);

                delete []lpText;
            }
        }
    }
    else
    {
        if (S_OK == m_MyAPlayer.m_pAPlayer->Play())
        {
            bPlay = TRUE;
        }
    }
    return bPlay;
}

// 停止播放 
BOOL CDuiFrameWnd::StopPlay()
{
    BOOL bPlay = FALSE;
    if (S_OK == m_MyAPlayer.m_pAPlayer->Close())
    {
        bPlay = TRUE;
    }
    return bPlay;
}

// 暂停播放 
BOOL CDuiFrameWnd::PausePlay()
{
    BOOL bPlay = FALSE;
    if ( S_OK == m_MyAPlayer.m_pAPlayer->Pause() )
    {
        // 播放按钮隐藏，变成暂停按钮 
        if (m_StartButton)
        {
            m_StartButton->SetVisible(true);
        }
        if (m_PauseButton)
        {
            m_PauseButton->SetVisible(false);
        }

        bPlay = TRUE;
    }
    return bPlay;
}

// 全屏播放 
BOOL CDuiFrameWnd::onFullScreen(BOOL bFull, RECT* rc)
{
    BOOL isFull = FALSE;
    if (bFull)
    {
        isFull = TRUE;
        GetWindowRect(GetHWND(), &m_clientRect);
        ::SetWindowPos(
            GetHWND(), 
            HWND_TOPMOST, 
            rc->left, 
            rc->top, 
            rc->right - rc->left, 
            rc->bottom - rc->top, 
            0);

        if (m_PlayListBlock)
        {
            m_PlayListBlock->SetVisible(false);
        }
        if (m_PlayControlPanel)
        {
            m_PlayControlPanel->SetVisible(false);
        }
        if (m_LiveUrlPanel)
        {
            m_LiveUrlPanel->SetVisible(false);
        }
        if (m_CaptionPanel)
        {
            m_CaptionPanel->SetVisible(false);
        }

        if (m_FullScreenBtn && m_NormalScreenBtn)
        {
            m_FullScreenBtn->SetVisible(false);
            m_NormalScreenBtn->SetVisible(true);
        }
    }
    else
    {
        isFull = FALSE;
        ::SetWindowPos(
            GetHWND(), 
            HWND_NOTOPMOST, 
            m_clientRect.left, 
            m_clientRect.top, 
            m_clientRect.right - m_clientRect.left, 
            m_clientRect.bottom - m_clientRect.top, 
            0);
        
        if (m_PlayControlPanel)
        {
            m_PlayControlPanel->SetVisible(true);
        }
        if (m_LiveUrlPanel)
        {
            m_LiveUrlPanel->SetVisible(true);
        }
        if (m_CaptionPanel)
        {
            m_CaptionPanel->SetVisible(true);
        }

        if (m_FullScreenBtn && m_NormalScreenBtn)
        {
            m_FullScreenBtn->SetVisible(true);
            m_NormalScreenBtn->SetVisible(false);
        }

        if (m_PlayListBlock && m_ShowPlayList)
        {
            if (!m_ShowPlayList->IsVisible())
            {
                m_PlayListBlock->SetVisible(true);
            }
            else
            {
                m_PlayListBlock->SetVisible(false);
            }
        }
    }

    return isFull;
}

LRESULT CDuiFrameWnd::OnShowCursor(BOOL bShow)
{
    if (!bShow)
    {
        if (m_system_cursor)
        {
            TCHAR buf[32];
            wsprintf(buf, TEXT("%u"), m_system_cursor);
            m_MyAPlayer.m_pAPlayer->SetConfig(38, buf);
        }
    }
    else
    {
        m_MyAPlayer.m_pAPlayer->SetConfig(38, TEXT("0"));
    }

    return 0;
}