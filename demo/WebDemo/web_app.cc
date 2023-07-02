// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "web_app.h"
#include "web_msg.h"

#include <string>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "web_handler.h"

WebApp::WebApp() {}

void WebApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();

  ::PostMessage(parent_wnd_, WEB_MSG_OnContextInitialized, 0, 0);
}

CefRefPtr<CefClient> WebApp::GetDefaultClient() {
  // Called when a new browser window is created via the Chrome runtime UI.
  return WebHandler::GetInstance();
}
