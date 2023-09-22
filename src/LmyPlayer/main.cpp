#include "DuiFrameWnd.h"
#include <CommCtrl.h>
#pragma comment(lib, "atl.lib")

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    AtlAxWinInit();
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

    CDuiFrameWnd *pFrame = new CDuiFrameWnd(_T("XMP.xml"));
    pFrame->Create(NULL, _T("LmyPlayer²¥·ÅÆ÷"), WS_POPUP, WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES);
    pFrame->CenterWindow();
    pFrame->ShowModal();

    delete pFrame;
    pFrame = NULL;
    return 0;
}
