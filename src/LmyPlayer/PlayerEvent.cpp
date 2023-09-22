#include "PlayerEvent.h"

#define SendMsg ::SendMessage

void CMyPlayer::Initialize()
{
    m_nPosition = 0;
    m_nDuration = 1000;
    m_pAPlayer = NULL;
    m_pConnectionPoint = NULL;
    m_dwCookie = 0;
    m_hAPlayerWnd = NULL;
    m_hLmyPlayerWnd = NULL;
}

void CMyPlayer::Release()
{
    if (m_pAPlayer != NULL)
    {
        m_pAPlayer->Close();

        if (m_dwCookie != 0)
        {
            m_pConnectionPoint->Unadvise(m_dwCookie);
            m_dwCookie = 0;
        }

        m_pAPlayer->Release();
        m_pAPlayer = NULL;

        if (IsWindow(m_hAPlayerWnd))
        {
            DestroyWindow(m_hAPlayerWnd);
            m_hAPlayerWnd = NULL;
        }
    }
}

HRESULT CMyPlayer::OnMessage(LONG nMessage, LONG wParam, LONG lParam)
{
    switch(nMessage)
    {
    case WM_LBUTTONDOWN:
        SendMsg(m_hLmyPlayerWnd, WM_PLAYLBUTTONDOWN, 0, 0);
        break;
    case WM_LBUTTONUP:
        SendMsg(m_hLmyPlayerWnd, WM_PLAYLBUTTONUP, 0, 0);
        break;
    case WM_LBUTTONDBLCLK:
        // 双击进入全屏,退出全屏 
        SendMsg(m_hLmyPlayerWnd, WM_PLAYLBUTTONDCLICK, 0, 0);
        break;
    case WM_KEYDOWN:
        // 按键消息 
        SendMsg(m_hLmyPlayerWnd, WM_PLAYKEYDOWN, wParam, lParam);
        break;
    case WM_MOUSEMOVE:
        SendMsg(m_hLmyPlayerWnd, WM_PLAYMOUSEMOVE, wParam, lParam);
        break;
    }
    return S_OK;
}


HRESULT CMyPlayer::OnStateChanged(LONG nOldState, LONG nNewState)
{
    SendMsg(m_hLmyPlayerWnd, WM_PLAYSTATECHANGE, nNewState, 0);
    return S_OK;
}

HRESULT CMyPlayer::OnOpenSucceeded()
{
    m_nDuration = m_pAPlayer->GetDuration();
    SendMsg(m_hLmyPlayerWnd, 
        WM_PLAYERSTATUS /* 播放器状态变化 */, 
        PER_OPENSUCCESS /* 打开源成功 */, 
        m_nDuration /* 视频时长(毫秒) */);
    return S_OK;
}

HRESULT CMyPlayer::OnSeekCompleted(LONG nPosition)
{
    SendMsg(m_hLmyPlayerWnd, 
        WM_PLAYERSTATUS /* 播放器状态变化 */, 
        PER_SEEKCOMPLET /* 进度变更 */, 
        nPosition /* 当前播放进度 */);
    return S_OK;
}

HRESULT CMyPlayer::OnBuffer(LONG nPercent)
{
    SendMsg(m_hLmyPlayerWnd, 
        WM_PLAYERSTATUS /* 播放器状态变化 */, 
        PER_BUFFER      /* 进度变更 */, 
        nPercent        /* 当前播放进度 */);
    return S_OK;
}

HRESULT CMyPlayer::OnVideoSizeChanged(void)
{
    SendMsg(m_hLmyPlayerWnd, 
        WM_PLAYERSTATUS /* 播放器状态变化 */, 
        PER_VIDEOSIZECHANGE   /* 进度变更 */, 
        0                 /* 当前播放进度 */);
    return S_OK;
}

HRESULT CMyPlayer::OnDownloadCodec(BSTR strCodecPath)
{
    USES_CONVERSION;
    // 异步下载解码器模式，通知APlayer取消当前下载
    m_pAPlayer->SetConfig(19, _bstr_t("0"));

    SendMsg(m_hLmyPlayerWnd, 
        WM_PLAYERSTATUS   /* 播放器状态变化 */, 
        PER_DOWNLOADCODEC /* 需要下载解码器 */,
        (LPARAM)strCodecPath /* 当前加载路径 */);
    return S_OK;
}

void CMyPlayer::SetPlayerHwnd(HWND hWnd)
{
    if (hWnd != NULL)
    {
        m_hAPlayerWnd = hWnd;
        m_hLmyPlayerWnd = GetParent(m_hAPlayerWnd);
    }
}

HWND CMyPlayer::GetPlayerHwnd()
{
    return m_hAPlayerWnd;
}