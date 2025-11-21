#include "ThemeManager.hpp"

ThemeManager& ThemeManager::instance() {
    static ThemeManager instance;
    return instance;
}

void ThemeManager::setTheme(const std::string& themeName) {
    if (themeName == "dark") applyDarkTheme();
    else if (themeName == "light") applyLightTheme();
    else if (themeName == "cyberpunk") applyCyberpunkTheme();
}

void ThemeManager::applyDarkTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::StyleColorsDark();
    
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.8f, 1.0f, 0.5f); // Cyan border
    style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    
    style.FrameRounding = 5.0f;
    style.WindowRounding = 0.0f;
}

void ThemeManager::applyLightTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::StyleColorsLight();
    
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.13f, 0.59f, 0.95f, 0.5f); // Blue border
    style.Colors[ImGuiCol_Button] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
    
    style.FrameRounding = 5.0f;
}

void ThemeManager::applyCyberpunkTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::StyleColorsDark();
    
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.01f, 0.13f, 1.0f); // Deep purple
    style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 0.0f, 1.0f, 0.8f); // Neon pink
    style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 1.0f, 1.0f, 1.0f); // Cyan text
    style.Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.05f, 0.2f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1f, 0.0f, 0.2f, 1.0f);
    
    style.FrameRounding = 2.0f;
}
