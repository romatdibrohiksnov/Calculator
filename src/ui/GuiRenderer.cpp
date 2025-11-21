#include "GuiRenderer.hpp"
#include "ImGuiWidgets.hpp"
#include "../utils/ThemeManager.hpp"
#include <algorithm>
#include <cmath>

GuiRenderer::GuiRenderer() 
    : mathEngine(new MathEngine()), 
      historyManager(new HistoryManager()),
      newCalculation(false),
      showHistory(false),
      currentMode(0), // Basic
      showGraph(false),
      showMathPalette(true), // Default to open for visibility
      graphExpression("sin(x)"),
      graphRangeX(10.0f),
      graphRangeY(5.0f),
      graphCenterX(0.0f),
      graphCenterY(0.0f)
{
    currentResult = "0";
}

// ... (existing code) ...

void GuiRenderer::renderMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Mode")) {
            if (ImGui::MenuItem("Basic", NULL, currentMode == 0)) currentMode = 0;
            if (ImGui::MenuItem("Scientific", NULL, currentMode == 1)) currentMode = 1;
            if (ImGui::MenuItem("Professional", NULL, currentMode == 2)) currentMode = 2;
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("History", NULL, showHistory)) showHistory = !showHistory;
            if (ImGui::MenuItem("Graph Mode", NULL, showGraph)) showGraph = !showGraph;
            if (ImGui::MenuItem("Math Palette", NULL, showMathPalette)) showMathPalette = !showMathPalette;
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Theme")) {
            if (ImGui::MenuItem("Dark")) ThemeManager::instance().setTheme("dark");
            if (ImGui::MenuItem("Light")) ThemeManager::instance().setTheme("light");
            if (ImGui::MenuItem("Cyberpunk")) ThemeManager::instance().setTheme("cyberpunk");
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void GuiRenderer::renderGraph(float width, float height) {
    ImGui::SetNextWindowPos(ImVec2(width * 0.05f, height * 0.05f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(width * 0.9f, height * 0.9f), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Graphing Mode", &showGraph)) {
        // Input controls
        char buffer[256];
        strncpy(buffer, graphExpression.c_str(), sizeof(buffer));
        if (ImGui::InputText("Function y=", buffer, sizeof(buffer))) {
            graphExpression = buffer;
        }
        
        ImGui::DragFloat("Range X", &graphRangeX, 0.1f, 1.0f, 100.0f);
        ImGui::DragFloat("Range Y", &graphRangeY, 0.1f, 1.0f, 100.0f);
        
        // Drawing area
        ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
        ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
        if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
        if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
        ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

        // Draw border and background
        ImGuiIO& io = ImGui::GetIO();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(20, 20, 20, 255));
        draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

        // Clip to canvas
        draw_list->PushClipRect(canvas_p0, canvas_p1, true);

        // Handle Zoom and Pan
        if (ImGui::IsWindowHovered()) {
            // Zoom with mouse wheel
            float wheel = io.MouseWheel;
            if (wheel != 0.0f) {
                float zoomFactor = (wheel > 0.0f) ? 0.9f : 1.1f;
                graphRangeX *= zoomFactor;
                graphRangeY *= zoomFactor;
            }

            // Pan with mouse drag
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
                ImVec2 delta = io.MouseDelta;
                // Convert screen delta to graph coordinates
                // Delta is in pixels, we need to subtract it from center (drag left -> move view right)
                float scaleX = (2 * graphRangeX) / canvas_sz.x;
                float scaleY = (2 * graphRangeY) / canvas_sz.y;
                
                graphCenterX -= delta.x * scaleX;
                graphCenterY += delta.y * scaleY; // Y is inverted in screen coords
            }
        }

        // Coordinate transformation helpers
        auto toScreenX = [&](double x) {
            return canvas_p0.x + (x - graphCenterX + graphRangeX) / (2 * graphRangeX) * canvas_sz.x;
        };
        auto toScreenY = [&](double y) {
            return canvas_p0.y + (1.0 - (y - graphCenterY + graphRangeY) / (2 * graphRangeY)) * canvas_sz.y;
        };

        // Draw Axes
        float originX = toScreenX(0.0);
        float originY = toScreenY(0.0);
        
        // X-Axis
        if (originY >= canvas_p0.y && originY <= canvas_p1.y)
            draw_list->AddLine(ImVec2(canvas_p0.x, originY), ImVec2(canvas_p1.x, originY), IM_COL32(100, 100, 100, 255));
        // Y-Axis
        if (originX >= canvas_p0.x && originX <= canvas_p1.x)
            draw_list->AddLine(ImVec2(originX, canvas_p0.y), ImVec2(originX, canvas_p1.y), IM_COL32(100, 100, 100, 255));

        // Plot Function
        if (!graphExpression.empty()) {
            const int steps = 1000; // Higher resolution
            ImVec2 lastPoint;
            bool first = true;
            
            for (int i = 0; i <= steps; i++) {
                double t = (double)i / steps;
                // Calculate x based on visible range and center
                double x = (graphCenterX - graphRangeX) + t * (2 * graphRangeX);
                
                double y = mathEngine->evaluate(graphExpression, x);
                
                if (!mathEngine->hasError()) {
                    ImVec2 point(toScreenX(x), toScreenY(y));
                    
                    if (!first) {
                        // Don't draw lines that jump too far (asymptotes)
                        if (std::abs(point.y - lastPoint.y) < canvas_sz.y) {
                            draw_list->AddLine(lastPoint, point, IM_COL32(0, 255, 0, 255), 2.0f);
                        }
                    }
                    lastPoint = point;
                    first = false;
                } else {
                    first = true; // Reset on error (gap in graph)
                }
            }
        }

        draw_list->PopClipRect();
    }
    ImGui::End();
}

GuiRenderer::~GuiRenderer() {
    delete mathEngine;
    delete historyManager;
}

void GuiRenderer::render(int width, int height) {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | 
                                   ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                                   ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | 
                                   ImGuiWindowFlags_NoScrollWithMouse;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID); // Force to Main Viewport

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
    
    ImGui::Begin("Calculator", nullptr, window_flags);
    
    // Handle keyboard input every frame
    handleKeyboardInput();

    renderMenuBar();
    
    // Calculate available height dynamically
    // Get current Y position (after menu bar)
    float startY = ImGui::GetCursorPosY();
    // Total window height - current Y - bottom padding (10)
    float availableHeight = (float)height - startY - 10.0f;
    
    float displayHeight = availableHeight * 0.25f; // 25% for display
    renderDisplay((float)width - 20, displayHeight);
    
    ImGui::Dummy(ImVec2(0, 10)); // Spacing
    
    // Remaining height for keypad: Available - Display - Spacing (10)
    float keypadHeight = availableHeight - displayHeight - 10.0f;
    
    if (currentMode == 0) {
        renderBasicKeypad((float)width - 20, keypadHeight);
    } else if (currentMode == 1) {
        renderScientificKeypad((float)width - 20, keypadHeight);
    } else {
        renderProfessionalKeypad((float)width - 20, keypadHeight);
    }

    if (showHistory) {
        renderHistory((float)width, (float)height);
    }
    
    if (showGraph) {
        renderGraph((float)width, (float)height);
    }
    
    if (showMathPalette) {
        renderMathPalette((float)width, (float)height);
    }

    ImGui::End();
    ImGui::PopStyleVar(2);
}



void GuiRenderer::renderDisplay(float width, float height) {
    ImVec4 borderColor = ImGui::GetStyle().Colors[ImGuiCol_Border];
    CustomWidgets::DisplayScreen(currentExpression.c_str(), currentResult.c_str(), ImVec2(width, height), borderColor);
}

void GuiRenderer::renderBasicKeypad(float width, float height) {
    const char* labels[] = {
        "C", "<", "%", "/",
        "7", "8", "9", "*",
        "4", "5", "6", "-",
        "1", "2", "3", "+",
        "0", ".", "=", ""
    };

    int rows = 5;
    int cols = 4;
    float btnWidth = (width - (cols - 1) * 5) / cols;
    float btnHeight = (height - (rows - 1) * 5) / rows;

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));

    for (int i = 0; i < 19; i++) {
        const char* label = labels[i];
        if (strlen(label) == 0) continue; // Skip empty slot (handled by 0 spanning)

        ImVec2 size = ImVec2(btnWidth, btnHeight);
        if (std::string(label) == "0") size.x = btnWidth * 2 + 5;

        ImVec4 baseColor;
        ImVec4 glowColor;
        
        // Determine colors based on type
        if (isdigit(label[0]) || label[0] == '.') {
            baseColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
            glowColor = ImVec4(0.0f, 0.5f, 1.0f, 0.5f);
        } else if (label[0] == '=') {
            baseColor = ImVec4(0.0f, 0.4f, 0.0f, 1.0f);
            glowColor = ImVec4(0.0f, 1.0f, 0.0f, 0.8f);
        } else if (label[0] == 'C' || label[0] == '<') {
            baseColor = ImVec4(0.4f, 0.1f, 0.1f, 1.0f);
            glowColor = ImVec4(1.0f, 0.2f, 0.2f, 0.8f);
        } else {
            baseColor = ImVec4(0.1f, 0.2f, 0.3f, 1.0f);
            glowColor = ImVec4(0.0f, 0.8f, 1.0f, 0.8f);
        }

        if (CustomWidgets::GlowingButton(label, size, baseColor, glowColor)) {
            handleInput(label);
        }

        if ((i + 1) % 4 != 0 && i != 18) ImGui::SameLine();
        if (std::string(label) == "0") ImGui::SameLine(); // Skip next slot
    }

    ImGui::PopStyleVar();
}

void GuiRenderer::renderScientificKeypad(float width, float height) {
    const char* labels[] = {
        "C", "<", "(", ")", "%", "mod", "/",
        "sin", "cos", "tan", "7", "8", "9", "*",
        "asin", "acos", "atan", "4", "5", "6", "-",
        "sqrt", "sqr", "pow", "1", "2", "3", "+",
        "log", "ln", "exp", "0", ".", "=", ""
    };

    int rows = 5;
    int cols = 7;
    float btnWidth = (width - (cols - 1) * 5) / cols;
    float btnHeight = (height - (rows - 1) * 5) / rows;

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));

    int count = 0;
    for (const char* label : labels) {
        ImVec2 size = ImVec2(btnWidth, btnHeight);
        
        ImVec4 baseColor;
        ImVec4 glowColor;
        
        if (strlen(label) == 0) {
            ImGui::Dummy(size);
        } else {
            if (isdigit(label[0]) || (label[0] == '.' && strlen(label)==1)) {
                baseColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
                glowColor = ImVec4(0.0f, 0.5f, 1.0f, 0.5f);
            } else if (label[0] == '=') {
                baseColor = ImVec4(0.0f, 0.4f, 0.0f, 1.0f);
                glowColor = ImVec4(0.0f, 1.0f, 0.0f, 0.8f);
            } else if (std::string(label) == "C" || std::string(label) == "<") {
                baseColor = ImVec4(0.4f, 0.1f, 0.1f, 1.0f);
                glowColor = ImVec4(1.0f, 0.2f, 0.2f, 0.8f);
            } else if (strlen(label) > 1 || label[0] == '(' || label[0] == ')') {
                baseColor = ImVec4(0.2f, 0.1f, 0.3f, 1.0f);
                glowColor = ImVec4(0.8f, 0.0f, 1.0f, 0.8f);
            } else {
                baseColor = ImVec4(0.1f, 0.2f, 0.3f, 1.0f);
                glowColor = ImVec4(0.0f, 0.8f, 1.0f, 0.8f);
            }

            if (CustomWidgets::GlowingButton(label, size, baseColor, glowColor)) {
                handleInput(label);
            }
        }

        count++;
        if (count % cols != 0) ImGui::SameLine();
    }

    ImGui::PopStyleVar();
}

void GuiRenderer::renderProfessionalKeypad(float width, float height) {
    const char* labels[] = {
        "C", "<", "(", ")", "%", "mod", "ans", "/",
        "sin", "cos", "tan", "diff", "7", "8", "9", "*",
        "asin", "acos", "atan", "int", "4", "5", "6", "-",
        "sqrt", "sqr", "pow", "sum", "1", "2", "3", "+",
        "log", "ln", "exp", "lim", "0", ".", "=", "Graph"
    };

    int rows = 5;
    int cols = 8; // 8 columns for Professional Mode
    float btnWidth = (width - (cols - 1) * 5) / cols;
    float btnHeight = (height - (rows - 1) * 5) / rows;

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));

    int count = 0;
    for (const char* label : labels) {
        ImVec2 size = ImVec2(btnWidth, btnHeight);
        
        ImVec4 baseColor;
        ImVec4 glowColor;
        
        if (strlen(label) == 0) {
            ImGui::Dummy(size);
        } else {
            if (isdigit(label[0]) || (label[0] == '.' && strlen(label)==1)) {
                baseColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
                glowColor = ImVec4(0.0f, 0.5f, 1.0f, 0.5f);
            } else if (label[0] == '=') {
                baseColor = ImVec4(0.0f, 0.4f, 0.0f, 1.0f);
                glowColor = ImVec4(0.0f, 1.0f, 0.0f, 0.8f);
            } else if (std::string(label) == "C" || std::string(label) == "<") {
                baseColor = ImVec4(0.4f, 0.1f, 0.1f, 1.0f);
                glowColor = ImVec4(1.0f, 0.2f, 0.2f, 0.8f);
            } else if (std::string(label) == "diff" || std::string(label) == "int" || 
                       std::string(label) == "sum" || std::string(label) == "lim") {
                // Advanced Calculus Buttons - Special Color (Gold/Orange)
                baseColor = ImVec4(0.4f, 0.3f, 0.1f, 1.0f);
                glowColor = ImVec4(1.0f, 0.8f, 0.0f, 0.8f);
            } else if (strlen(label) > 1 || label[0] == '(' || label[0] == ')') {
                baseColor = ImVec4(0.2f, 0.1f, 0.3f, 1.0f);
                glowColor = ImVec4(0.8f, 0.0f, 1.0f, 0.8f);
            } else {
                baseColor = ImVec4(0.1f, 0.2f, 0.3f, 1.0f);
                glowColor = ImVec4(0.0f, 0.8f, 1.0f, 0.8f);
            }

            if (CustomWidgets::GlowingButton(label, size, baseColor, glowColor)) {
                handleInput(label);
            }
        }

        count++;
        if (count % cols != 0) ImGui::SameLine();
    }

    ImGui::PopStyleVar();
}

void GuiRenderer::renderHistory(float width, float height) {
    ImGui::SetNextWindowPos(ImVec2(width * 0.1f, height * 0.1f));
    ImGui::SetNextWindowSize(ImVec2(width * 0.8f, height * 0.8f));
    
    if (ImGui::Begin("History", &showHistory, ImGuiWindowFlags_NoCollapse)) {
        if (ImGui::Button("Clear History")) {
            historyManager->clear();
        }
        ImGui::Separator();
        
        ImGui::BeginChild("HistoryList");
        for (size_t i = 0; i < historyManager->size(); i++) {
            const auto& entry = historyManager->getEntry(i);
            ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "%s", entry.timestamp.c_str());
            ImGui::Text("%s = %s", entry.expression.c_str(), entry.result.c_str());
            ImGui::Separator();
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void GuiRenderer::handleInput(const std::string& input) {
    if (input == "C") {
        clear();
    } else if (input == "<") {
        backspace();
    } else if (input == "=") {
        calculateResult();
    } else if (input == "sqr") {
        currentExpression += "^2";
    } else if (input == "pow") {
        currentExpression += "^";
    } else if (input == "sqrt") {
        currentExpression += "sqrt(";
    } else if (input == "diff") {
        currentExpression += "diff(";
    } else if (input == "int") {
        currentExpression += "int(";
    } else if (input == "sum") {
        currentExpression += "sum(";
    } else if (input == "lim") {
        currentExpression += "lim(";
    } else if (input == "lim") {
        currentExpression += "lim(";
    } else if (input == "Graph") {
        showGraph = !showGraph;
    } else if (input == "ans") {
        // Placeholder for Ans
    } else if (input == "sin" || input == "cos" || input == "tan" || 
               input == "asin" || input == "acos" || input == "atan" || 
               input == "log" || input == "ln" || input == "exp") {
        currentExpression += input + "(";
    } else {
        if (newCalculation) {
            if (isdigit(input[0])) currentExpression = "";
            newCalculation = false;
        }
        currentExpression += input;
    }
}

void GuiRenderer::calculateResult() {
    if (currentExpression.empty()) return;

    try {
        double result = mathEngine->evaluate(currentExpression);
        
        if (mathEngine->hasError()) {
            currentResult = "Error";
        } else {
            // Format result
            char buffer[64];
            if (std::abs(result - std::round(result)) < 1e-10) {
                snprintf(buffer, sizeof(buffer), "%.0f", result);
            } else {
                snprintf(buffer, sizeof(buffer), "%.10g", result);
            }
            currentResult = buffer;
            
            historyManager->addEntry(currentExpression, currentResult);
        }
    } catch (...) {
        currentResult = "Error";
    }
    newCalculation = true;
}

void GuiRenderer::clear() {
    currentExpression = "";
    currentResult = "0";
    newCalculation = false;
}

void GuiRenderer::backspace() {
    if (!currentExpression.empty()) {
        currentExpression.pop_back();
    }
}


void GuiRenderer::renderMathPalette(float width, float height) {
    ImGui::SetNextWindowPos(ImVec2(width * 0.6f, height * 0.1f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Math Palette", &showMathPalette)) {
        if (ImGui::BeginTabBar("MathTabs")) {
            
            // Algebra Tab
            if (ImGui::BeginTabItem("Alg")) {
                if (ImGui::Button("sqrt")) handleInput("sqrt"); ImGui::SameLine();
                if (ImGui::Button("cbrt")) handleInput("cbrt"); ImGui::SameLine();
                if (ImGui::Button("x^y")) handleInput("pow");
                
                if (ImGui::Button("log")) handleInput("log"); ImGui::SameLine();
                if (ImGui::Button("ln")) handleInput("ln"); ImGui::SameLine();
                if (ImGui::Button("e^x")) handleInput("exp");
                
                if (ImGui::Button("abs")) handleInput("abs"); ImGui::SameLine();
                if (ImGui::Button("n!")) handleInput("fact"); ImGui::SameLine();
                if (ImGui::Button("mod")) handleInput("mod");
                
                ImGui::EndTabItem();
            }
            
            // Trigonometry Tab
            if (ImGui::BeginTabItem("Trig")) {
                if (ImGui::Button("sin")) handleInput("sin"); ImGui::SameLine();
                if (ImGui::Button("cos")) handleInput("cos"); ImGui::SameLine();
                if (ImGui::Button("tan")) handleInput("tan");
                
                if (ImGui::Button("asin")) handleInput("asin"); ImGui::SameLine();
                if (ImGui::Button("acos")) handleInput("acos"); ImGui::SameLine();
                if (ImGui::Button("atan")) handleInput("atan");
                
                ImGui::Separator();
                ImGui::Text("Hyperbolic");
                if (ImGui::Button("sinh")) handleInput("sinh"); ImGui::SameLine();
                if (ImGui::Button("cosh")) handleInput("cosh"); ImGui::SameLine();
                if (ImGui::Button("tanh")) handleInput("tanh");
                
                if (ImGui::Button("asinh")) handleInput("asinh"); ImGui::SameLine();
                if (ImGui::Button("acosh")) handleInput("acosh"); ImGui::SameLine();
                if (ImGui::Button("atanh")) handleInput("atanh");
                
                ImGui::EndTabItem();
            }
            
            // Calculus Tab
            if (ImGui::BeginTabItem("Calc")) {
                if (ImGui::Button("Derivative (d/dx)")) handleInput("diff");
                if (ImGui::Button("Integral (S)")) handleInput("int");
                if (ImGui::Button("Limit (lim)")) handleInput("lim");
                if (ImGui::Button("Sum (Sigma)")) handleInput("sum");
                ImGui::EndTabItem();
            }
            
            // Statistics/Comb Tab
            if (ImGui::BeginTabItem("Stat")) {
                if (ImGui::Button("nPr")) handleInput("nPr"); ImGui::SameLine();
                if (ImGui::Button("nCr")) handleInput("nCr");
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void GuiRenderer::handleKeyboardInput() {
    ImGuiIO& io = ImGui::GetIO();
    
    // Handle character input (numbers, operators, parenthesis)
    for (int i = 0; i < io.InputQueueCharacters.Size; i++) {
        char c = (char)io.InputQueueCharacters[i];
        if (isdigit(c) || c == '.' || c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '(' || c == ')') {
            std::string s(1, c);
            handleInput(s);
        }
    }

    // Handle special keys
    if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)) {
        handleInput("=");
    }
    if (ImGui::IsKeyPressed(ImGuiKey_Backspace)) {
        handleInput("<");
    }
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        handleInput("C");
    }
}
