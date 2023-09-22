#include "MsgBox.h"
#include <Windows.h>
#include <tchar.h>

#define PROPNAME        L"szWndProp"
#define CLASSNAME       L"WeWorkMessageBoxFrame"
#define TITLEBASE       L"��ʾ��Ϣ"
#define IDM_SETBODY     1001
#define WND_WIDTH       373 
#define WND_HEIGHT      168 
#define CloseBtnSize    25
#define FONT_HEIGHT     14 
#define FONT_WIDTH      7
#define MOUSEOVERCOLOR  RGB(225, 225, 225)

typedef struct _ClientInfo
{
    RECT closeBtn;
    bool closeMouseOver;
    RECT okBtn;
    bool okMouseOver;
    const wchar_t* szTitle;
    const wchar_t* szBody;
    int ilines;
}ClientInfo, *PClientInfo;

int GetLines(const wchar_t* szText, DWORD* nCount = NULL)
{
    int lines = 1;
    DWORD nc = 0;

    if (szText != NULL)
    {
        const wchar_t* p = szText;
        while(*p)
        {
            if (*p=='\r' || *p == '\n')
            {
                while(*p)
                {
                    if (*p != '\r' && *p != '\n')
                    {
                        break;
                    }
                    p++;
                }
                lines++;
                nc = 0;
            }
            else
            {
                if (*p > 0x80)
                {
                    nc++;
                }
                nc++;

                if (nCount && nc > *nCount)
                {
                    *nCount = nc;
                }
            }
            if (*p == L'\0')
            {
                break;
            }
            p++;
        }
    }
    return lines;
}

static bool PointInRect(int x, int y, RECT* rect)
{
    if (rect->left   < x && 
        rect->right  > x &&
        rect->top    < y &&
        rect->bottom > y
        )
    {
        return true;
    }
    return false;
}

static void DrawMouseBg(PAINTSTRUCT* ps, RECT* rect, COLORREF bgColor = MOUSEOVERCOLOR, COLORREF lineColor = RGB(255,255,255))
{
    HBRUSH hb = CreateSolidBrush(bgColor);
    HPEN hp = CreatePen(PS_DOT, 1, lineColor);
    HBRUSH orgBrs = (HBRUSH)SelectObject(ps->hdc, hb);
    HPEN orgHpn = (HPEN)SelectObject(ps->hdc, hp);
    Rectangle(ps->hdc, 
        rect->left, 
        rect->top, 
        rect->right,
        rect->bottom);
    SelectObject(ps->hdc, orgHpn);
    SelectObject(ps->hdc, orgBrs);
    DeleteObject(hp);
    DeleteObject(hb);
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    PClientInfo prop = (PClientInfo)GetPropW(hWnd, PROPNAME);

    switch (message)
    {
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        switch (wmId)
        {
        case IDM_SETBODY:
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            if (prop)
            {
                HFONT hFont = CreateFont(
                    -FONT_HEIGHT/*�߶�*/, -FONT_WIDTH/*���*/, 
                    0/*��ת�Ƕ�*/, 0/*���ù�*/, 400 /*һ�����ֵ��Ϊ400*/,
                    FALSE/*����б��*/, FALSE/*�����»���*/, FALSE/*����ɾ����*/,
                    DEFAULT_CHARSET, //��������ʹ��Ĭ���ַ��������������� _CHARSET ��β�ĳ�������
                    OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, //���в������ù�
                    DEFAULT_QUALITY, //Ĭ���������
                    FF_DONTCARE, //��ָ��������*/
                    TEXT("����") //������
                    );
                HFONT hOldFont = (HFONT)SelectObject(ps.hdc, hFont);

                // ��汳����ͼ 
                if (prop->closeMouseOver)
                {
                    DrawMouseBg(&ps, &prop->closeBtn);
                }
                int off = CloseBtnSize/3;
                MoveToEx(ps.hdc, prop->closeBtn.left+off, prop->closeBtn.top+off, NULL);
                LineTo(ps.hdc, prop->closeBtn.right-off+1, prop->closeBtn.bottom-off+1);
                MoveToEx(ps.hdc, prop->closeBtn.left+off, prop->closeBtn.bottom-off, NULL);
                LineTo(ps.hdc, prop->closeBtn.right-off+1, prop->closeBtn.top+off-1);

                // ȷ����ť 
                if (prop->okMouseOver)
                {
                    DrawMouseBg(&ps, &prop->okBtn, MOUSEOVERCOLOR, RGB(160,160,160));
                }
                else
                {
                    DrawMouseBg(&ps, &prop->okBtn, RGB(255, 255, 255), RGB(160,160,160));
                }

                // ����͸����д�� 
                SetBkMode(ps.hdc, TRANSPARENT);

                RECT rect;
                GetClientRect(hWnd, &rect);
                // ���� 
                rect.top = FONT_HEIGHT;
                DrawTextW(hdc, prop->szTitle, wcslen(prop->szTitle), &rect, DT_CENTER|DT_VCENTER);
                // ���� 
                rect.top = (rect.bottom - FONT_HEIGHT*prop->ilines)/2;
                DrawTextW(hdc, prop->szBody, wcslen(prop->szBody), &rect, DT_CENTER|DT_VCENTER);
                // ȷ����ť 
                rect = prop->okBtn;
                rect.top += 5;
                DrawTextW(hdc, L"ȷ��", 2, &rect, DT_CENTER|DT_VCENTER);

                // ɾ�� 
                SelectObject(ps.hdc, hOldFont);
                DeleteObject(hFont);
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_LBUTTONDOWN:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            if (prop)
            {
                if ( 
                    PointInRect(x, y, &prop->closeBtn) ||
                    PointInRect(x, y, &prop->okBtn)
                    )
                {
                    SendMessage(hWnd, WM_CLOSE, 0, 0); // ����::DestroyWindow,�ڲ�����WM_DESTROY 
                }
                else
                {
                     SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
                }
            }
        }
        break;
    case WM_MOUSEMOVE:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            if (prop)
            {
                // �ж�������ڹرհ�ť���� 
                bool closeMouse = PointInRect(x, y, &prop->closeBtn);
                if (prop->closeMouseOver != closeMouse)
                {
                    prop->closeMouseOver = closeMouse;
                    InvalidateRect(hWnd, &prop->closeBtn, TRUE);
                }

                // �ж��������ȷ����ť���� 
                bool okMouse = PointInRect(x, y, &prop->okBtn);
                if (prop->okMouseOver != okMouse)
                {
                    prop->okMouseOver = okMouse;
                    InvalidateRect(hWnd, &prop->okBtn, TRUE);
                }
            }
        }
        break;
    case WM_CHAR:
        // 0xD �س�/0x1B ESC/0x20 �ո�
        if ( wParam == 0x1B )
        {
            SendMessage(hWnd, WM_CLOSE, 0, 0);
        }
        break;
    case WM_SIZE:
        if (prop)
        {
            RECT rect;
            GetClientRect(hWnd, &rect);

            // �رհ�ť 
            prop->closeBtn.left = rect.right - CloseBtnSize;
            prop->closeBtn.right = rect.right;
            prop->closeBtn.top = 0;
            prop->closeBtn.bottom = CloseBtnSize;

            // ȷ����ť
            prop->okBtn.left = rect.right - 80 - CloseBtnSize/2;
            prop->okBtn.right = rect.right - CloseBtnSize/2;
            prop->okBtn.top = rect.bottom - FONT_HEIGHT*2 - 10;
            prop->okBtn.bottom = rect.bottom - FONT_HEIGHT;
        }
        break;
    case WM_DESTROY:
        if (prop)
        {
            delete prop;
            SetPropW(hWnd, PROPNAME, NULL);
        }
        PostQuitMessage(0);  // ���� WM_QUIT 
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

static ATOM MyRegisterClass()
{
    WNDCLASSEXW wcex = { sizeof(WNDCLASSEXW) };

    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.hInstance		= GetModuleHandle(NULL);
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszClassName	= CLASSNAME;
    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HWND hParentWnd, const wchar_t* szBody, const wchar_t* szTitle)
{
    HWND hChildWnd;

    PClientInfo pInfo = new ClientInfo;
    if (pInfo == NULL)
    {
        return FALSE;
    }
    DWORD maxlen = 0;
    memset(pInfo, 0, sizeof(ClientInfo));
    pInfo->szBody = szBody;
    pInfo->szTitle = szTitle;
    pInfo->ilines = GetLines(szBody, &maxlen);

    // �����ı����ݣ����¼��㴰�ڿ�� 
    int width = WND_WIDTH;
    if (maxlen > WND_WIDTH/FONT_WIDTH/2 - 6)
    {
        width = (maxlen+10)*FONT_WIDTH;
    }
    int height = WND_HEIGHT;
    if (pInfo->ilines > 1)
    {
        height += (pInfo->ilines-1)*FONT_HEIGHT;
    }
    // Ĭ����ʾ�����м� 
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    hChildWnd = CreateWindowExW( 
            WS_EX_TOPMOST,
            CLASSNAME, 
            szTitle, 
            WS_POPUP | WS_THICKFRAME,
            (x-width)/2, 
            (y-height)/2, 
            width, 
            height, 
            NULL, 
            NULL, 
            GetModuleHandle(NULL), 
            NULL
        );

    if (!hChildWnd)
    {
        delete pInfo;
        return FALSE;
    }
    SetPropW(hChildWnd, PROPNAME, (HANDLE)pInfo);

    if (hParentWnd != NULL)
    {
        // ��Ҫ��ʾ�ڸ����ڵ����м� 
        // SetParent(hChildWnd, hParentWnd);
        RECT lpParentRect;
        GetWindowRect(hParentWnd, &lpParentRect);
        POINT ParentPoint;
        ParentPoint.x = lpParentRect.left + (lpParentRect.right-lpParentRect.left)/2;
        ParentPoint.y = lpParentRect.top + (lpParentRect.bottom-lpParentRect.top)/2;
        
        HMONITOR hMonitor = MonitorFromPoint(ParentPoint, MONITOR_DEFAULTTONULL);
        MONITORINFO mi = { sizeof(MONITORINFO) };
        if (GetMonitorInfo(hMonitor, &mi))
        {
            if (
                mi.rcWork.left < ParentPoint.x && mi.rcWork.right > ParentPoint.x &&
                mi.rcWork.top < ParentPoint.y && mi.rcWork.bottom > ParentPoint.y
                )
            {
                MoveWindow(hChildWnd,
                    ParentPoint.x - width / 2,
                    ParentPoint.y - height / 2,
                    width, height,
                    TRUE);
            }
        }
    }
    
    ShowWindow(hChildWnd, SW_NORMAL);
    UpdateWindow(hChildWnd);

    // �ȳ�ʼ���رա�ȷ�ϰ�ť 
    SendMessage(hChildWnd, WM_SIZE, 0, 0);
    // �ػ���� 
    RECT rect;
    GetClientRect(hChildWnd, &rect);
    InvalidateRect(hChildWnd, &rect, TRUE);
    return TRUE;
}

int __stdcall MsgBoxW(HWND hParentWnd, const wchar_t* szBody, const wchar_t* szTitle)
{
    MSG msg;
    static ATOM aTom = 0;
    if (aTom == 0)
    {
        aTom = MyRegisterClass();
    }

    if (szTitle == NULL)
    {
        szTitle = TITLEBASE;
    }
    if (!InitInstance(hParentWnd, szBody, szTitle))
    {
        return -1;
    }

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

static PWSTR MulToWide( LPCSTR str, UINT CodePage )
{
    PWSTR  pElementText;
    int    iTextLen;

    iTextLen = MultiByteToWideChar( CodePage,
        0,
        (PCHAR)str,
        -1,
        NULL,
        0 );

    pElementText = 
        (PWSTR)malloc((iTextLen+1)*sizeof(WCHAR));
    if (pElementText)
    {
        MultiByteToWideChar( CodePage,
            0,
            (PCHAR)str,
            -1,
            pElementText,
            iTextLen );
    }

    return pElementText;
}

int __stdcall MsgBoxA(HWND hParentWnd, const char* szBody, const char* szTitle)
{
    wchar_t* szwBody = NULL;
    wchar_t* szwTitle = NULL;
    if (szBody)
    {
        szwBody = MulToWide(szBody, CP_ACP);
    }
    if (szTitle)
    {
        szwTitle = MulToWide(szTitle, CP_ACP);
    }
    
    int ret = MsgBoxW(hParentWnd, szwBody, szwTitle);

    if (szwTitle)
    {
        free(szwTitle);
    }
    if (szwBody)
    {
        free(szwBody);
    }
    return ret;
}