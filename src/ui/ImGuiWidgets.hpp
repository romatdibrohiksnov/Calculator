#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include <string>
#include <cmath>

namespace CustomWidgets {

    // Custom glowing button
    inline bool GlowingButton(const char* label, const ImVec2& size_arg, const ImVec4& baseColor, const ImVec4& glowColor) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ImGui::ItemSize(size, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, id))
            return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

        // Render
        const ImU32 col_bg = ImGui::GetColorU32(held ? ImVec4(baseColor.x * 0.8f, baseColor.y * 0.8f, baseColor.z * 0.8f, 1.0f) : 
                                               hovered ? ImVec4(baseColor.x * 1.2f, baseColor.y * 1.2f, baseColor.z * 1.2f, 1.0f) : baseColor);
        
        // Glow effect
        if (hovered || held) {
            window->DrawList->AddRect(ImVec2(bb.Min.x - 2, bb.Min.y - 2), ImVec2(bb.Max.x + 2, bb.Max.y + 2), 
                                     ImGui::GetColorU32(glowColor), style.FrameRounding, 0, 4.0f);
            
            // Outer glow blur simulation (multiple rects with decreasing alpha)
            for (int i = 0; i < 3; i++) {
                float alpha = 0.3f - (i * 0.1f);
                if (alpha > 0) {
                    window->DrawList->AddRect(ImVec2(bb.Min.x - 4 - i*2, bb.Min.y - 4 - i*2), 
                                             ImVec2(bb.Max.x + 4 + i*2, bb.Max.y + 4 + i*2), 
                                             ImGui::GetColorU32(ImVec4(glowColor.x, glowColor.y, glowColor.z, alpha)), 
                                             style.FrameRounding + i*2, 0, 2.0f);
                }
            }
        }

        ImGui::RenderNavHighlight(bb, id);
        ImGui::RenderFrame(bb.Min, bb.Max, col_bg, true, style.FrameRounding);

        if (g.LogEnabled)
            ImGui::LogSetNextTextDecoration("[", "]");
        
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
        ImGui::PopStyleColor();

        return pressed;
    }

    // Custom display screen
    inline void DisplayScreen(const char* expression, const char* result, const ImVec2& size, const ImVec4& borderColor) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return;

        ImVec2 pos = window->DC.CursorPos;
        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        
        ImGui::ItemSize(size);
        if (!ImGui::ItemAdd(bb, 0)) return;

        // Background gradient
        ImU32 bg_col_top = ImGui::GetColorU32(ImVec4(0.05f, 0.05f, 0.05f, 1.0f));
        ImU32 bg_col_bot = ImGui::GetColorU32(ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
        window->DrawList->AddRectFilledMultiColor(bb.Min, bb.Max, bg_col_top, bg_col_top, bg_col_bot, bg_col_bot);
        
        // Border
        window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(borderColor), 5.0f, 0, 2.0f);

        // Expression text (small, top right)
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // Default font
        ImVec2 exprSize = ImGui::CalcTextSize(expression);
        ImVec2 exprPos = ImVec2(bb.Max.x - exprSize.x - 10, bb.Min.y + 10);
        window->DrawList->AddText(exprPos, ImGui::GetColorU32(ImVec4(0.6f, 0.6f, 0.6f, 1.0f)), expression);
        ImGui::PopFont();

        // Result text (large, bottom right)
        // Note: In a real app we'd load a larger font, here we simulate scale or just use default
        // For now using default font but maybe we can scale it if we had font scaling enabled
        // ImGui::SetWindowFontScale(2.0f); // Only works per window
        
        ImVec2 resSize = ImGui::CalcTextSize(result);
        ImVec2 resPos = ImVec2(bb.Max.x - resSize.x - 10, bb.Max.y - resSize.y - 10);
        window->DrawList->AddText(resPos, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), result);
        // ImGui::SetWindowFontScale(1.0f);
    }
}
