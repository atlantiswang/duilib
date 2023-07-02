// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#undef GetNextSibling
#undef GetFirstChild

#include "include/cef_app.h"

// Implement application-level callbacks for the browser process.
class WebApp : public CefApp, public CefBrowserProcessHandler {
 public:
  WebApp();

  // CefApp methods:
  CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
    return this;
  }

  // CefBrowserProcessHandler methods:
  void OnContextInitialized() override;
  CefRefPtr<CefClient> GetDefaultClient() override;

  // 设置父窗口句柄
  void SetParentWnd(HWND parent) { parent_wnd_ = parent; }

 private:
  HWND parent_wnd_ = NULL;
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(WebApp);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
