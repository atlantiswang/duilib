#include "web_wnd.h"


#include "include/cef_command_line.h"
#include "include/cef_sandbox_win.h"
#include "web_app.h"



int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    int exit_code;

#if defined(ARCH_CPU_32_BITS)
    // Run the main thread on 32-bit Windows using a fiber with the preferred 4MiB
    // stack size. This function must be called at the top of the executable entry
    // point function (`main()` or `wWinMain()`). It is used in combination with
    // the initial stack size of 0.5MiB configured via the `/STACK:0x80000` linker
    // flag on executable targets. This saves significant memory on threads (like
    // those in the Windows thread pool, and others) whose stack size can only be
    // controlled via the linker flag.
    exit_code = CefRunWinMainWithPreferredStackSize(wWinMain, hInstance,
                                                    lpCmdLine, nCmdShow);
    if (exit_code >= 0) {
      // The fiber has completed so return here.
      return exit_code;
    }
#endif

    void* sandbox_info = nullptr;

#if defined(CEF_USE_SANDBOX)
    // Manage the life span of the sandbox information object. This is necessary
    // for sandbox support on Windows. See cef_sandbox_win.h for complete details.
    CefScopedSandboxInfo scoped_sandbox;
    sandbox_info = scoped_sandbox.sandbox_info();
#endif

    // Provide CEF with command-line arguments.
    CefMainArgs main_args(hInstance);

    // CEF applications have multiple sub-processes (render, GPU, etc) that share
    // the same executable. This function checks the command-line and, if this is
    // a sub-process, executes the appropriate logic.
    exit_code = CefExecuteProcess(main_args, nullptr, sandbox_info);
    if (exit_code >= 0) {
      // The sub-process has completed so return here.
      return exit_code;
    }
    // CEF has initialized.

    // WebApp implements application-level callbacks for the browser process.
    // It will create the first browser instance in OnContextInitialized() after
    // Parse command-line arguments for use in this method.
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromString(::GetCommandLineW());

    // Specify CEF global settings here.
    CefSettings settings;

    if (command_line->HasSwitch("enable-chrome-runtime")) {
      // Enable experimental Chrome runtime. See issue #2969 for details.
      settings.chrome_runtime = true;
    }

#if !defined(CEF_USE_SANDBOX)
    settings.no_sandbox = true;
#endif

    // CEF消息循环在独立线程，不与主UI线程冲突
    settings.multi_threaded_message_loop = 1;

    // 初始化duilib资源
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
    CPaintManagerUI::SetResourceZip(_T("WebDemo_skin.zip"));

    // 初始化dui主窗口
    WebWnd* wnd = new WebWnd();
    //wnd->Create(NULL, _T("WebDemo"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW , 0, 0, 1024, 968);
    wnd->Create(NULL, _T("WebDemo"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 1024, 738);
    wnd->CenterWindow();
    ::ShowWindow(*wnd, SW_SHOWMAXIMIZED);


    // 初始化WebApp
    CefRefPtr<WebApp> app(new WebApp);
    // 设置duilib地主窗口句柄到WebApp，用于回调通知
    app->SetParentWnd(wnd->GetHWND());
    // Initialize CEF.
    CefInitialize(main_args, settings, app.get(), sandbox_info);

    CPaintManagerUI::MessageLoop();

    // Run the CEF message loop. This will block until CefQuitMessageLoop() is
    // called.
    // 设置settings.multi_threaded_message_loop = 1;不需要再调用CefRunMessageLoop
    //CefRunMessageLoop();

    // Shut down CEF.
    CefShutdown();

	return 0;
}