#pragma once

#include <string>
#include <map>
#include "imgui.h"

class ThemeManager {
public:
    static ThemeManager& instance();
    
    void setTheme(const std::string& themeName);
    
private:
    ThemeManager() = default;
    ~ThemeManager() = default;
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;
    
    void applyDarkTheme();
    void applyLightTheme();
    void applyCyberpunkTheme();
};
