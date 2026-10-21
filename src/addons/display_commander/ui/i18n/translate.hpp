#pragma once

// Korean UI translation layer.
//
// Applied inside the ReShade ImGui wrapper (imgui_wrapper_reshade.hpp), so UI code keeps its English literals and
// upstream changes only fall back to English instead of conflicting. Lookup is by exact English runtime string.
//  - TrText():  plain text / printf format strings / tooltips. Returns the Korean string or the input unchanged.
//  - TrLabel(): widget labels. Keeps any "##id" suffix; for labels without one, appends "###<English>" so the ImGui ID
//               stays unique even when two English labels translate to the same Korean text.
// Both understand a leading icon glyph (ForkAwesome, Unicode private use area) and translate the text after it.

namespace display_commander::ui::i18n {

const char* TrText(const char* text);
const char* TrLabel(const char* label);

bool IsEnabled();
void SetEnabled(bool enabled);

}  // namespace display_commander::ui::i18n
