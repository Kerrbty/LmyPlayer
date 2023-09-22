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
        // ˫������ȫ��,�˳�ȫ�� 
        SendMsg(m_hLmyPlayerWnd, WM_PLAYLBUTTONDCLICK, 0, 0);
        break;
    case WM_KEYDOWN:
        // ������Ϣ 
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
        WM_PLAYERSTATUS /* ������״̬�仯 */, 
        PER_OPENSUCCESS /* ��Դ�ɹ� */, 
        m_nDuration /* ��Ƶʱ��(����) */);
    return S_OK;
}

HRESULT CMyPlayer::OnSeekCompleted(LONG nPosition)
{
    SendMsg(m_hLmyPlayerWnd, 
        WM_PLAYERSTATUS /* ������״̬�仯 */, 
        PER_SEEKCOMPLET /* ���ȱ�� */, 
        nPosition /* ��ǰ���Ž��� */);
    return S_OK;
}

HRESULT CMyPlayer::OnBuffer(LONG nPercent)
{
    SendMsg(m_hLmyPlayerWnd, 
        WM_PLAYERSTATUS /* ������״̬�仯 */, 
        PER_BUFFER      /* ���ȱ�� */, 
        nPercent        /* ��ǰ���Ž��� */);
    return S_OK;
}

HRESULT CMyPlayer::OnVideoSizeChanged(void)
{
    SendMsg(m_hLmyPlayerWnd, 
        WM_PLAYERSTATUS /* ������״̬�仯 */, 
        PER_VIDEOSIZECHANGE   /* ���ȱ�� */, 
        0                 /* ��ǰ���Ž��� */);
    return S_OK;
}

HRESULT CMyPlayer::OnDownloadCodec(BSTR strCodecPath)
{
    USES_CONVERSION;
    // �첽���ؽ�����ģʽ��֪ͨAPlayerȡ����ǰ����
    m_pAPlayer->SetConfig(19, _bstr_t("0"));

    SendMsg(m_hLmyPlayerWnd, 
        WM_PLAYERSTATUS   /* ������״̬�仯 */, 
        PER_DOWNLOADCODEC /* ��Ҫ���ؽ����� */,
        (LPARAM)strCodecPath /* ��ǰ����·�� */);
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