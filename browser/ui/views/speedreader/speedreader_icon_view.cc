// Copyright (c) 2019 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "brave/browser/ui/views/speedreader/speedreader_icon_view.h"

#include <string>

#include "base/strings/utf_string_conversions.h"
#include "brave/app/brave_command_ids.h"
#include "brave/app/vector_icons/vector_icons.h"
#include "brave/browser/speedreader/speedreader_tab_helper.h"
#include "brave/grit/brave_generated_resources.h"
#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/themes/theme_properties.h"
#include "chrome/browser/ui/views/location_bar/location_bar_bubble_delegate_view.h"
#include "chrome/grit/generated_resources.h"
#include "components/dom_distiller/content/browser/distillable_page_utils.h"
#include "components/dom_distiller/content/browser/uma_helper.h"
#include "include/core/SkColor.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/theme_provider.h"
#include "ui/views/metadata/metadata_impl_macros.h"

namespace {
SkColor kReaderIconColor = SkColorSetRGB(0x4c, 0x54, 0xd2);
}  // anonymous namespace

SpeedreaderIconView::SpeedreaderIconView(
    CommandUpdater* command_updater,
    IconLabelBubbleView::Delegate* icon_label_bubble_delegate,
    PageActionIconView::Delegate* page_action_icon_delegate,
    PrefService* pref_service)
    : PageActionIconView(command_updater,
                         IDC_SPEEDREADER_ICON_ONCLICK,
                         this, /* Make ourselves the icon bubble delegate */
                         page_action_icon_delegate),
      icon_label_bubble_delegate_(icon_label_bubble_delegate) {}
// pref_service_(pref_service) {}

SpeedreaderIconView::~SpeedreaderIconView() = default;

void SpeedreaderIconView::UpdateImpl() {
  auto* contents = GetWebContents();
  if (!contents || !contents->GetLastCommittedURL().SchemeIsHTTPOrHTTPS()) {
    SetVisible(false);
    return;
  }

  if (contents != web_contents_) {
    if (web_contents_)
      dom_distiller::RemoveObserver(web_contents_, this);
    web_contents_ = contents;
    dom_distiller::AddObserver(web_contents_, this);
  }

  auto* tab_helper = speedreader::SpeedreaderTabHelper::Get(web_contents_);
  const bool is_distilled = tab_helper->IsDistilledPage();
  auto result = dom_distiller::GetLatestResult(web_contents_);
  if (result) {
    // fixme: (is_distilled && !isSpeedreaderEnabled)
    const bool visible =
        (result->is_last && result->is_distillable) || is_distilled;
    LOG(ERROR) << "visible: " << result->is_last << " "
               << result->is_distillable << " "
               << web_contents_->GetLastCommittedURL().spec();
    SetVisible(visible);
  } else {
    SetVisible(false);
  }

  if (GetVisible() && is_distilled) {
    int mode_label = tab_helper->IsSpeedreaderEnabled()
                         ? IDS_ICON_SPEEDREADER_MODE_LABEL
                         : IDS_ICON_READER_MODE_LABEL;
    SetLabel(l10n_util::GetStringUTF16(mode_label));
    SetIconColor(kReaderIconColor);
    label()->SetVisible(true);
  } else {
    const ui::ThemeProvider* tp = GetThemeProvider();
    SkColor icon_color_default =
        tp->GetColor(ThemeProperties::COLOR_TOOLBAR_BUTTON_ICON);
    SetIconColor(icon_color_default);
    label()->SetVisible(false);
  }
}

const gfx::VectorIcon& SpeedreaderIconView::GetVectorIcon() const {
  return kSpeedreaderIcon;
}

std::u16string SpeedreaderIconView::GetTextForTooltipAndAccessibleName() const {
  return l10n_util::GetStringUTF16(GetActive() ? IDS_EXIT_DISTILLED_PAGE
                                               : IDS_DISTILL_PAGE);
}

void SpeedreaderIconView::OnExecuting(
    PageActionIconView::ExecuteSource execute_source) {}

views::BubbleDialogDelegate* SpeedreaderIconView::GetBubble() const {
  auto* web_contents = GetWebContents();
  if (!web_contents)
    return nullptr;

  auto* bubble_tab_helper =
      speedreader::SpeedreaderTabHelper::Get(web_contents);
  if (!bubble_tab_helper)
    return nullptr;

  return reinterpret_cast<LocationBarBubbleDelegateView*>(
      bubble_tab_helper->speedreader_bubble_view());
}

SkColor SpeedreaderIconView::GetIconLabelBubbleSurroundingForegroundColor()
    const {
  // We can always return this since the text will be set to invisible on
  // non-readable pages.
  return kReaderIconColor;
}

SkColor SpeedreaderIconView::GetIconLabelBubbleInkDropColor() const {
  return icon_label_bubble_delegate_->GetIconLabelBubbleInkDropColor();
}

SkColor SpeedreaderIconView::GetIconLabelBubbleBackgroundColor() const {
  return icon_label_bubble_delegate_->GetIconLabelBubbleBackgroundColor();
}

void SpeedreaderIconView::OnResult(
    const dom_distiller::DistillabilityResult& result) {
  Update();
}

BEGIN_METADATA(SpeedreaderIconView, PageActionIconView)
END_METADATA