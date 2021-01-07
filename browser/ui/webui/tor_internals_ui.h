/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_UI_WEBUI_TOR_INTERNALS_UI_H_
#define BRAVE_BROWSER_UI_WEBUI_TOR_INTERNALS_UI_H_

#include <memory>
#include <string>
#include <vector>

#include "brave/browser/ui/webui/basic_ui.h"
#include "content/public/browser/web_ui_message_handler.h"

class TorInternalsDOMHandler : public content::WebUIMessageHandler {
 public:
  TorInternalsDOMHandler();
  ~TorInternalsDOMHandler() override;

  // WebUIMessageHandler implementation.
  void RegisterMessages() override;

 private:
  void HandleGetTorGeneralInfo(const base::ListValue* args);

  base::WeakPtrFactory<TorInternalsDOMHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(TorInternalsDOMHandler);
};

class TorInternalsUI : public BasicUI {
 public:
  TorInternalsUI(content::WebUI* web_ui, const std::string& host);
  ~TorInternalsUI() override;

 private:
  DISALLOW_COPY_AND_ASSIGN(TorInternalsUI);
};

#endif  // BRAVE_BROWSER_UI_WEBUI_TOR_INTERNALS_UI_H_
