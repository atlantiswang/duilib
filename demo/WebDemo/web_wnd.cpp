#pragma once

#include "web_wnd.h"
#include "web_msg.h"
#include "web_app.h"
#include "web_handler.h"

void WebWnd::Init() 
{
    m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
    m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
    m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
    m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
    m_pUrlGoBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_go")));
    m_pUrlEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_url")));
    m_pWebContainer = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("web_container")));
    m_pWebContainer->OnSize += MakeDelegate(this, &WebWnd::OnWebSizeChanged);
}

LRESULT WebWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
    styleValue &= ~WS_CAPTION;
    ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    m_pm.Init(m_hWnd);
    //m_pm.SetTransparent(100);
    CDialogBuilder builder;
    CControlUI* pRoot = builder.Create(_T("web.xml"), (UINT)0, NULL, &m_pm);
    ASSERT(pRoot && "Failed to parse XML");
    m_pm.AttachDialog(pRoot);
    m_pm.AddNotifier(this);
    Init();
    return 0;
}

LRESULT WebWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    WebHandler::GetInstance()->CloseAllBrowsers(true);
    bHandled = FALSE;
    return 0;
}

LRESULT WebWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    ::PostQuitMessage(0L);
    bHandled = FALSE;
    return 0;
}

void WebWnd::OnPrepare(TNotifyUI& msg) {
    auto url = m_pUrlEdit->GetText();
    if (url.IsEmpty()) {
        return;
    }
}

void WebWnd::Notify(TNotifyUI& msg) {
    if( msg.sType == _T("windowinit") ) 
        OnPrepare(msg);
    else if( msg.sType == _T("click") ) {
        if( msg.pSender == m_pCloseBtn ) {
            PostQuitMessage(0);
            return; 
        } else if( msg.pSender == m_pMinBtn ) { 
            SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
        } else if( msg.pSender == m_pMaxBtn ) { 
            SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);  
        } else if( msg.pSender == m_pRestoreBtn ) { 
            SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);  
        } else if (msg.pSender == m_pUrlGoBtn) {
            OnUrlGoBtnClicked(msg);
        }
    } else if(msg.sType==_T("setfocus")) {
    } else if( msg.sType == _T("return") ) {
        if( msg.pSender->GetName() == _T("edit_url") ) {
            OnUrlGoBtnClicked(msg);
        }
    }
}

void WebWnd::OnUrlGoBtnClicked(TNotifyUI& msg) {
    auto url = m_pUrlEdit->GetText();
    WebHandler::GetInstance()->UrlGo(url.GetData());
}

LRESULT WebWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    if( ::IsIconic(*this) ) bHandled = FALSE;
    return (wParam == 0) ? TRUE : FALSE;
}

LRESULT WebWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    return 0;
}

LRESULT WebWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    return 0;
}

LRESULT WebWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
    ::ScreenToClient(*this, &pt);

    RECT rcClient;
    ::GetClientRect(*this, &rcClient);

    if( !::IsZoomed(*this) ) {
        RECT rcSizeBox = m_pm.GetSizeBox();
        if( pt.y < rcClient.top + rcSizeBox.top ) {
            if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
            if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
            return HTTOP;
        }
        else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
            if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
            if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
            return HTBOTTOM;
        }
        if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
        if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
    }

    RECT rcCaption = m_pm.GetCaptionRect();
    if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
        && pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
            if( pControl && _tcscmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0 && 
                _tcscmp(pControl->GetClass(), DUI_CTR_OPTION) != 0 &&
                _tcscmp(pControl->GetClass(), DUI_CTR_TEXT) != 0 )
                return HTCAPTION;
    }

    return HTCLIENT;
}

LRESULT WebWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    SIZE szRoundCorner = m_pm.GetRoundCorner();
    if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
        CDuiRect rcWnd;
        ::GetWindowRect(*this, &rcWnd);
        rcWnd.Offset(-rcWnd.left, -rcWnd.top);
        rcWnd.right++; rcWnd.bottom++;
        HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
        ::SetWindowRgn(*this, hRgn, TRUE);
        ::DeleteObject(hRgn);
    }

    bHandled = FALSE;
    return 0;
}

LRESULT WebWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    int primaryMonitorWidth = ::GetSystemMetrics(SM_CXSCREEN);
    int primaryMonitorHeight = ::GetSystemMetrics(SM_CYSCREEN);
    MONITORINFO oMonitor = {};
    oMonitor.cbSize = sizeof(oMonitor);
    ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
    CDuiRect rcWork = oMonitor.rcWork;
    rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);
    if (rcWork.right > primaryMonitorWidth) rcWork.right = primaryMonitorWidth;
    if (rcWork.bottom > primaryMonitorHeight) rcWork.right = primaryMonitorHeight;
    LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
    lpMMI->ptMaxPosition.x = rcWork.left;
    lpMMI->ptMaxPosition.y = rcWork.top;
    lpMMI->ptMaxSize.x = rcWork.right;
    lpMMI->ptMaxSize.y = rcWork.bottom;
    bHandled = FALSE;
    return 0;
}

LRESULT WebWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    // 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
    if( wParam == SC_CLOSE ) {
        ::PostQuitMessage(0L);
        bHandled = TRUE;
        return 0;
    }
    BOOL bZoomed = ::IsZoomed(*this);
    LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    if( ::IsZoomed(*this) != bZoomed ) {
        if( !bZoomed ) {
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
            if( pControl ) pControl->SetVisible(false);
            pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
            if( pControl ) pControl->SetVisible(true);
        }
        else {
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
            if( pControl ) pControl->SetVisible(true);
            pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
            if( pControl ) pControl->SetVisible(false);
        }
    }
    return lRes;
}

LRESULT WebWnd::OnContextInitialized(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    CefRefPtr<WebHandler> handler(new WebHandler());
    // 初始化WebApp

    // Specify CEF browser settings here.
    CefBrowserSettings browser_settings;

    // Information used when creating the native window.
    CefWindowInfo window_info;

    auto rc = m_pWebContainer->GetPos();
    CefRect web_rect(rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
    window_info.SetAsChild(GetHWND(), web_rect);

    // Create the first browser window.
    CefBrowserHost::CreateBrowser(window_info, handler, "http://www.baidu.com", browser_settings,
                                  nullptr, nullptr);

    bHandled = TRUE;
    return 0;
}

bool WebWnd::OnWebSizeChanged(void *p) {
    if (WebHandler::GetInstance()) {
        auto web_rc = m_pWebContainer->GetPos();
        WebHandler::GetInstance()->UpdateRect(web_rc);
    }

    return false;
}

LRESULT WebWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT lRes = 0;
    BOOL bHandled = TRUE;
    switch( uMsg ) {
        case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
        case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
        case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
        case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
        case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
        case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
        case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
        case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
        case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
        case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
        case WEB_MSG_OnContextInitialized: lRes = OnContextInitialized(uMsg, wParam, lParam, bHandled); break;
        default:
            bHandled = FALSE;
    }
    if( bHandled ) return lRes;
    if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}