#pragma once

#include "imgui_wrapper_base.hpp"
#include "i18n/translate.hpp"

#include <imgui.h>
#include <cstdarg>
#include <cstddef>
#include <vector>

namespace display_commander {
namespace ui {

/** Draw list proxy: forwards to ReShade's ImDrawList so callers never touch raw ImDrawList* (same TU = same ABI). */
struct ImDrawListProxyReshade : IImDrawList {
    ImDrawList* list_ = nullptr;
    void set(ImDrawList* L) { list_ = L; }
    void AddLine(const ImVec2& p1, const ImVec2& p2, ImU32 col, float thickness = 1.0f) override {
        if (list_) list_->AddLine(p1, p2, col, thickness);
    }
    void AddRect(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, int flags = 0,
                 float thickness = 1.0f) override {
        if (list_) list_->AddRect(p_min, p_max, col, rounding, static_cast<ImDrawFlags>(flags), thickness);
    }
    void AddRectFilled(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f,
                       int flags = 0) override {
        if (list_) list_->AddRectFilled(p_min, p_max, col, rounding, static_cast<ImDrawFlags>(flags));
    }
    void AddCircle(const ImVec2& center, float radius, ImU32 col, int num_segments = 0,
                   float thickness = 1.0f) override {
        if (list_) list_->AddCircle(center, radius, col, num_segments, thickness);
    }
    void AddCircleFilled(const ImVec2& center, float radius, ImU32 col, int num_segments = 0) override {
        if (list_) list_->AddCircleFilled(center, radius, col, num_segments);
    }
    void AddTriangleFilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col) override {
        if (list_) list_->AddTriangleFilled(p1, p2, p3, col);
    }
};

/** ImGui wrapper that forwards to ReShade's ImGui (used in addon overlay). Header-only so ImGui symbols stay in the
 * same TU as overlay code. */
struct ImGuiWrapperReshade : IImGuiWrapper {
    // Korean UI: every user-visible string goes through i18n::TrText (text/tooltips/format strings) or
    // i18n::TrLabel (widget labels). Window/popup/table/child ids are left alone so lookups by id keep working.
    static const char* T(const char* s) { return i18n::TrText(s); }
    static const char* L(const char* s) { return i18n::TrLabel(s); }
    ImDrawListProxyReshade draw_list_proxy_;
    void SameLine(float offset_from_start_x = 0.f, float spacing_w = -1.f) override {
        ImGui::SameLine(offset_from_start_x, spacing_w);
    }
    void Text(const char* fmt, ...) override {
        va_list args;
        va_start(args, fmt);
        ImGui::TextV(T(fmt), args);
        va_end(args);
    }
    void TextColored(const ImVec4& col, const char* fmt, ...) override {
        va_list args;
        va_start(args, fmt);
        ImGui::TextColoredV(col, T(fmt), args);
        va_end(args);
    }
    void TextUnformatted(const char* text) override { ImGui::TextUnformatted(T(text)); }
    bool Button(const char* label) override { return ImGui::Button(L(label)); }
    bool Button(const char* label, const ImVec2& size) override { return ImGui::Button(L(label), size); }
    bool SmallButton(const char* label) override { return ImGui::SmallButton(L(label)); }
    bool Checkbox(const char* label, bool* v) override { return ImGui::Checkbox(L(label), v); }
    bool RadioButton(const char* label, int* v, int v_button_id) override {
        return ImGui::RadioButton(L(label), v, v_button_id);
    }
    bool IsItemHovered() override { return ImGui::IsItemHovered(); }
    bool IsItemActive() override { return ImGui::IsItemActive(); }
    bool IsItemDeactivatedAfterEdit() override { return ImGui::IsItemDeactivatedAfterEdit(); }
    void SetTooltip(const char* fmt, ...) override {
        va_list args;
        va_start(args, fmt);
        ImGui::SetTooltipV(T(fmt), args);
        va_end(args);
    }
    void SetTooltipExV(float wrap_width, const char* fmt, va_list args) override {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
        ImGui::TextV(T(fmt), args);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
    void Spacing() override { ImGui::Spacing(); }
    void Separator() override { ImGui::Separator(); }
    bool BeginChild(const char* str_id, const ImVec2& size, bool border) override {
        return ImGui::BeginChild(str_id, size, border);
    }
    void EndChild() override { ImGui::EndChild(); }
    bool CollapsingHeader(const char* label, int flags = 0) override {
        return ImGui::CollapsingHeader(L(label), static_cast<ImGuiTreeNodeFlags>(flags));
    }
    bool BeginTable(const char* str_id, int columns, int flags, const ImVec2& outer_size = ImVec2(0.f, 0.f)) override {
        return ImGui::BeginTable(str_id, columns, static_cast<ImGuiTableFlags>(flags), outer_size);
    }
    void EndTable() override { ImGui::EndTable(); }
    void TableSetupColumn(const char* label, int flags = 0, float width_weight = 0.f) override {
        ImGui::TableSetupColumn(L(label), static_cast<ImGuiTableColumnFlags>(flags), width_weight);
    }
    void TableSetupScrollFreeze(int cols, int rows) override { ImGui::TableSetupScrollFreeze(cols, rows); }
    void TableHeadersRow() override { ImGui::TableHeadersRow(); }
    void TableNextRow() override { ImGui::TableNextRow(); }
    void TableNextColumn() override { ImGui::TableNextColumn(); }
    void TableSetColumnIndex(int column_n) override { ImGui::TableSetColumnIndex(column_n); }
    bool BeginCombo(const char* label, const char* preview_value, int flags = 0) override {
        return ImGui::BeginCombo(L(label), T(preview_value), static_cast<ImGuiComboFlags>(flags));
    }
    void EndCombo() override { ImGui::EndCombo(); }
    bool Selectable(const char* label, bool selected = false) override { return ImGui::Selectable(L(label), selected); }
    void SetItemDefaultFocus() override { ImGui::SetItemDefaultFocus(); }
    void PushID(int id) override { ImGui::PushID(id); }
    void PushID(const char* str_id) override { ImGui::PushID(str_id); }
    void PopID() override { ImGui::PopID(); }
    void Indent() override { ImGui::Indent(); }
    void Unindent() override { ImGui::Unindent(); }
    bool InputText(const char* label, char* buf, size_t buf_size) override {
        return ImGui::InputText(L(label), buf, buf_size);
    }
    bool InputText(const char* label, char* buf, size_t buf_size, int flags) override {
        return ImGui::InputText(L(label), buf, buf_size, static_cast<ImGuiInputTextFlags>(flags));
    }
    bool InputFloat(const char* label, float* v, float step, float step_fast, const char* format, int flags) override {
        return ImGui::InputFloat(L(label), v, step, step_fast, format, static_cast<ImGuiInputTextFlags>(flags));
    }
    bool InputInt(const char* label, int* v, int step, int step_fast, int flags) override {
        return ImGui::InputInt(L(label), v, step, step_fast, static_cast<ImGuiInputTextFlags>(flags));
    }
    bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format) override {
        return ImGui::SliderInt(L(label), v, v_min, v_max, T(format));
    }
    void TextWrapped(const char* fmt, ...) override {
        va_list args;
        va_start(args, fmt);
        ImGui::TextWrappedV(T(fmt), args);
        va_end(args);
    }
    void TextDisabled(const char* fmt, ...) override {
        va_list args;
        va_start(args, fmt);
        ImGui::TextDisabledV(T(fmt), args);
        va_end(args);
    }
    void PushStyleColor(int col_enum, const ImVec4& color) override {
        ImGui::PushStyleColor(static_cast<ImGuiCol>(col_enum), color);
    }
    void PopStyleColor(int count = 1) override { ImGui::PopStyleColor(count); }
    bool TreeNodeEx(const char* label, int flags) override {
        return ImGui::TreeNodeEx(L(label), static_cast<ImGuiTreeNodeFlags>(flags));
    }
    bool TreeNode(const char* label) override { return ImGui::TreeNode(L(label)); }
    void TreePop() override { ImGui::TreePop(); }
    ImVec2 GetContentRegionAvail() override { return ImGui::GetContentRegionAvail(); }
    float GetStyleItemSpacingX() override { return ImGui::GetStyle().ItemSpacing.x; }
    float GetStyleFramePaddingX() override { return ImGui::GetStyle().FramePadding.x; }
    ImVec2 CalcTextSize(const char* text) override { return ImGui::CalcTextSize(T(text)); }
    void SetNextItemWidth(float width) override { ImGui::SetNextItemWidth(width); }
    void BeginDisabled() override { ImGui::BeginDisabled(); }
    void EndDisabled() override { ImGui::EndDisabled(); }

    void PlotLines(const char* label, const float* values, int values_count, int values_offset,
                   const char* overlay_text, float scale_min, float scale_max, const ImVec2& graph_size) override {
        ImGui::PlotLines(L(label), values, values_count, values_offset, T(overlay_text), scale_min, scale_max,
                        graph_size);
    }
    bool Combo(const char* label, int* current_item, const char* const items[], int items_count) override {
        // Translate the displayed item texts only; the selected index is unchanged.
        std::vector<const char*> translated(static_cast<size_t>(items_count > 0 ? items_count : 0));
        for (int i = 0; i < items_count; ++i) {
            translated[static_cast<size_t>(i)] = T(items[i]);
        }
        return ImGui::Combo(L(label), current_item, translated.data(), items_count);
    }
    IImDrawList* GetWindowDrawList() override {
        ImDrawList* L = ImGui::GetWindowDrawList();
        draw_list_proxy_.set(L);
        return L ? &draw_list_proxy_ : nullptr;
    }
    ImVec2 GetCursorScreenPos() override { return ImGui::GetCursorScreenPos(); }
    void SetCursorScreenPos(const ImVec2& pos) override { ImGui::SetCursorScreenPos(pos); }
    float GetCursorPosX() override { return ImGui::GetCursorPosX(); }
    void Dummy(const ImVec2& size) override { ImGui::Dummy(size); }
    ImVec2 GetItemRectMin() override { return ImGui::GetItemRectMin(); }
    ImVec2 GetItemRectSize() override { return ImGui::GetItemRectSize(); }
    void ProgressBar(float fraction, const ImVec2& size_arg, const char* overlay) override {
        ImGui::ProgressBar(fraction, size_arg, T(overlay));
    }
    ImU32 GetColorU32(int col_enum) override { return ImGui::GetColorU32(static_cast<ImGuiCol>(col_enum)); }
    ImU32 ColorConvertFloat4ToU32(const ImVec4& col) override { return ImGui::ColorConvertFloat4ToU32(col); }
    bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format) override {
        return ImGui::SliderFloat(L(label), v, v_min, v_max, T(format));
    }
    void Columns(int count, const char* id, bool border) override { ImGui::Columns(count, id, border); }
    void NextColumn() override { ImGui::NextColumn(); }
    void SetColumnWidth(int column_index, float width) override { ImGui::SetColumnWidth(column_index, width); }
    void BeginTooltip() override { ImGui::BeginTooltip(); }
    void EndTooltip() override { ImGui::EndTooltip(); }
    void BulletText(const char* fmt, ...) override {
        va_list args;
        va_start(args, fmt);
        ImGui::BulletTextV(T(fmt), args);
        va_end(args);
    }
    float GetTextLineHeight() override { return ImGui::GetTextLineHeight(); }
    float GetTextLineHeightWithSpacing() override { return ImGui::GetTextLineHeightWithSpacing(); }
    float GetFontSize() const override { return ImGui::GetFontSize(); }
    bool InputTextWithHint(const char* label, const char* hint, char* buf, size_t buf_size) override {
        return ImGui::InputTextWithHint(L(label), T(hint), buf, buf_size);
    }
    void BeginGroup() override { ImGui::BeginGroup(); }
    void EndGroup() override { ImGui::EndGroup(); }
    const ImGuiStyle& GetStyle() override { return ImGui::GetStyle(); }
    bool Begin(const char* name, bool* p_open, int flags) override {
        return ImGui::Begin(name, p_open, static_cast<ImGuiWindowFlags>(flags));
    }
    void End() override { ImGui::End(); }
    void SetNextWindowPos(const ImVec2& pos, int cond, const ImVec2& pivot) override {
        ImGui::SetNextWindowPos(pos, static_cast<ImGuiCond>(cond), pivot);
    }
    void SetNextWindowSize(const ImVec2& size, int cond) override {
        ImGui::SetNextWindowSize(size, static_cast<ImGuiCond>(cond));
    }
    void SetNextWindowBgAlpha(float alpha) override { ImGui::SetNextWindowBgAlpha(alpha); }
    ImVec2 GetWindowPos() override { return ImGui::GetWindowPos(); }
    IImDrawList* GetForegroundDrawList() override {
        ImDrawList* L = ImGui::GetForegroundDrawList(nullptr);
        draw_list_proxy_.set(L);
        return L ? &draw_list_proxy_ : nullptr;
    }
    ImVec2 GetDisplaySize() override {
        const ImGuiIO& io = ImGui::GetIO();
        return ImVec2(io.DisplaySize.x, io.DisplaySize.y);
    }
    const ImGuiIO& GetIO() override { return ImGui::GetIO(); }
    unsigned int GetFrameCount() override { return static_cast<unsigned int>(ImGui::GetFrameCount()); }
    // No-op in ReShade overlay: ReShade owns the frame lifecycle (NewFrame/Render). Only standalone UI calls these.
    void NewFrame() override {}
    void Render() override {}
    void CreateContext() override {}
    void DestroyContext() override {}
    void StyleColorsDark() override {}
    void SetConfigFlags(uint32_t) override {}
    void SetDisplaySize(const ImVec2&) override {}
    void SetFontGlobalScale(float) override {}
    bool BeginTabBar(const char* str_id, int flags = 0) override {
        return ImGui::BeginTabBar(str_id, static_cast<ImGuiTabBarFlags>(flags));
    }
    bool BeginTabItem(const char* label, bool* p_open, int flags) override {
        return ImGui::BeginTabItem(L(label), p_open, static_cast<ImGuiTabItemFlags>(flags));
    }
    void EndTabItem() override { ImGui::EndTabItem(); }
    void EndTabBar() override { ImGui::EndTabBar(); }
    bool IsKeyDown(int key) override { return ImGui::IsKeyDown(static_cast<ImGuiKey>(key)); }
    bool IsKeyPressed(int key) override { return ImGui::IsKeyPressed(static_cast<ImGuiKey>(key)); }
    void SetKeyboardFocusHere(int offset = 0) override { ImGui::SetKeyboardFocusHere(offset); }
    void OpenPopup(const char* str_id) override { ImGui::OpenPopup(str_id); }
    bool BeginPopupModal(const char* name, bool* p_open, int flags) override {
        return ImGui::BeginPopupModal(name, p_open, static_cast<ImGuiWindowFlags>(flags));
    }
    void EndPopup() override { ImGui::EndPopup(); }
    bool BeginPopupContextItem(const char* str_id, int popup_flags) override {
        return ImGui::BeginPopupContextItem(str_id, static_cast<ImGuiPopupFlags>(popup_flags));
    }
    bool MenuItem(const char* label, const char* shortcut, bool selected, bool enabled) override {
        return ImGui::MenuItem(L(label), shortcut, selected, enabled);
    }
};

}  // namespace ui
}  // namespace display_commander
