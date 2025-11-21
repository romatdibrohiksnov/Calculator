#pragma once

#include "imgui.h"
#include "../core/MathEngine.hpp"
#include "../core/HistoryManager.hpp"
#include <string>
#include <vector>

class GuiRenderer {
public:
    GuiRenderer();
    ~GuiRenderer();

    void render(int width, int height);

private:
    MathEngine* mathEngine;
    HistoryManager* historyManager;
    
    std::string currentExpression;
    std::string currentResult;
    bool newCalculation;
    bool showHistory;
    int currentMode; // 0: Basic, 1: Scientific

    // Graphing
    bool showGraph;
    bool showMathPalette;
    std::string graphExpression;
    float graphRangeX; // X-axis range (+/-)
    float graphRangeY; // Y-axis range (+/-)
    float graphCenterX; // Center X coordinate
    float graphCenterY; // Center Y coordinate

    void renderMenuBar();
    void renderDisplay(float width, float height);
    void renderBasicKeypad(float width, float height);
    void renderScientificKeypad(float width, float height);
    void renderProfessionalKeypad(float width, float height);
    void renderMathPalette(float width, float height);
    void renderHistory(float width, float height);
    void renderGraph(float width, float height);
    
    void handleInput(const std::string& input);
    void handleKeyboardInput();
    void calculateResult();
    void clear();
    void backspace();
};
