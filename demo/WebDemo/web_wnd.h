#pragma once

#include "web_handler.h"
#include "DuiLib/UIlib.h"


using namespace DuiLib;

class WebWnd : public CWindowWnd, public INotifyUI {
public:
    WebWnd() {
    }
    virtual ~WebWnd() {}

    LPCTSTR GetWindowClassName() const { 
        return _T("WebWnd"); 
    }

    UINT GetClassStyle() const { 
        return CS_DBLCLKS; 
    }

    void OnFinalMessage(HWND /*hWnd*/) { 
        delete this;
    }
private:
    void Init();
    void OnPrepare(TNotifyUI& msg);
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    void Notify(TNotifyUI& msg);
    void OnUrlGoBtnClicked(TNotifyUI& msg);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnContextInitialized(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    bool OnWebSizeChanged(void *p);
private:
    CPaintManagerUI m_pm;
    CButtonUI* m_pCloseBtn = nullptr;
    CButtonUI* m_pMaxBtn = nullptr;
    CButtonUI* m_pRestoreBtn = nullptr;
    CButtonUI* m_pMinBtn = nullptr;
    CButtonUI* m_pUrlGoBtn = nullptr;
    CEditUI* m_pUrlEdit = nullptr;
    CTabLayoutUI* m_pWebContainer = nullptr;
};