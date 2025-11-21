# Professional Calculator

A high-tech calculator application built with C++ and Qt6 featuring:

## Features

### 🧮 Advanced Math Engine
- **Basic Operations**: Addition, subtraction, multiplication, division
- **Scientific Functions**: Trigonometric (sin, cos, tan, asin, acos, atan)
- **Logarithmic**: log, ln, custom base logarithms
- **Power Functions**: Square root, cube root, nth root, power, exponential
- **Advanced**: Factorial, permutation, combination, modulo
- **Expression Parser**: Evaluate complete mathematical expressions with parentheses and operator precedence

### 🎨 Beautiful Theme System
- **Dark Theme**: High-tech design with cyan accents and glowing effects
- **Light Theme**: Clean modern design with blue accents
- **Cyberpunk Theme**: Neon futuristic design with pink/cyan colors
- **Smooth Transitions**: Animated theme switching
- **Theme Persistence**: Remembers your preferred theme

### ✨ Modern UI Components
- **Custom Glowing Buttons**: Beautiful hover and press animations
- **Smart Display**: Auto-sizing text, multi-line expression view
- **Responsive Layout**: Adapts to window resizing
- **Category-based Styling**: Different colors for numbers, operators, functions

### 📝 Calculator Modes
- **Basic Mode**: Essential operations in a compact 4x4 layout
- **Scientific Mode**: Extended layout with scientific functions
- **History**: Track all calculations with timestamps

### ⌨️ Keyboard Support
- Numbers and operators via keyboard
- Enter for equals
- Escape for clear
- Backspace to delete
# Professional Calculator

A high-tech calculator application built with C++ and Qt6 featuring:

## Features

### 🧮 Advanced Math Engine
- **Basic Operations**: Addition, subtraction, multiplication, division
- **Scientific Functions**: Trigonometric (sin, cos, tan, asin, acos, atan)
- **Logarithmic**: log, ln, custom base logarithms
- **Power Functions**: Square root, cube root, nth root, power, exponential
- **Advanced**: Factorial, permutation, combination, modulo
- **Expression Parser**: Evaluate complete mathematical expressions with parentheses and operator precedence

### 🎨 Beautiful Theme System
- **Dark Theme**: High-tech design with cyan accents and glowing effects
- **Light Theme**: Clean modern design with blue accents
- **Cyberpunk Theme**: Neon futuristic design with pink/cyan colors
- **Smooth Transitions**: Animated theme switching
- **Theme Persistence**: Remembers your preferred theme

### ✨ Modern UI Components
- **Custom Glowing Buttons**: Beautiful hover and press animations
- **Smart Display**: Auto-sizing text, multi-line expression view
- **Responsive Layout**: Adapts to window resizing
- **Category-based Styling**: Different colors for numbers, operators, functions

### 📝 Calculator Modes
- **Basic Mode**: Essential operations in a compact 4x4 layout
- **Scientific Mode**: Extended layout with scientific functions
- **History**: Track all calculations with timestamps

### ⌨️ Keyboard Support
- Numbers and operators via keyboard
- Enter for equals
- Escape for clear
- Backspace to delete
- Parentheses support

## Building

### Requirements
- CMake 3.16+
- C++17 compatible compiler
- Internet connection (for fetching dependencies)

### Build Instructions

```bash
# Windows
cd "c:/path/Calculator"
mkdir build
cd build
cmake ..
cmake --build . --config Release

# Run
./Release/ProfessionalCalculator.exe
```

```bash
# Linux/Mac
cd "/path/to/Calculator"
mkdir build
cd build
cmake ..
make

# Run
./ProfessionalCalculator
```

## VS Code Setup

1. **Install Extensions**:
   - C/C++ (Microsoft)
   - CMake Tools (Microsoft)

2. **Open Project**:
   - Open the `Calculator` folder in VS Code

3. **Build**:
   - Press `Ctrl+Shift+B` to build the project
   - CMake will automatically fetch ImGui and GLFW

4. **Run/Debug**:
   - Press `F5` to launch the calculator

## Usage

### Basic Operations
1. Click number buttons or type on keyboard
2. Click operator buttons (+, -, *, /)
3. Press = or Enter to calculate

### Scientific Functions
1. Switch to Scientific mode from Settings menu
2. Use function buttons (sin, cos, log, etc.)
3. Functions automatically add opening parenthesis
4. Close parenthesis when ready

### Theme Switching
1. Settings → Theme
2. Choose Dark, Light, or Cyberpunk
3. Theme saves automatically

### History
- Settings → View History to see recent calculations
- Settings → Export History to save to file

## Architecture

The project follows MVC pattern:

- **Model** (`src/core/`): MathEngine and HistoryManager
- **View** (`src/ui/`): MainWindow and UI components
- **Controller** (`src/utils/`): ThemeManager

## License

MIT License - feel free to use and modify!
