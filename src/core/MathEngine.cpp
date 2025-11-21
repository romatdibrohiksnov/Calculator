#include "MathEngine.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stack>

MathEngine::MathEngine() : memory(0.0), lastError("") {}

// Basic operations
double MathEngine::add(double a, double b) { return a + b; }
double MathEngine::subtract(double a, double b) { return a - b; }
double MathEngine::multiply(double a, double b) { return a * b; }
double MathEngine::divide(double a, double b) {
    if (b == 0.0) {
        setError("Division by zero");
        return 0.0;
    }
    return a / b;
}

// Trigonometric functions
double MathEngine::toRadians(double degrees) { return degrees * PI / 180.0; }
double MathEngine::toDegrees(double radians) { return radians * 180.0 / PI; }

double MathEngine::sine(double x, bool isDegrees) {
    return std::sin(isDegrees ? toRadians(x) : x);
}

double MathEngine::cosine(double x, bool isDegrees) {
    return std::cos(isDegrees ? toRadians(x) : x);
}

double MathEngine::tangent(double x, bool isDegrees) {
    return std::tan(isDegrees ? toRadians(x) : x);
}

double MathEngine::arcsine(double x, bool isDegrees) {
    if (x < -1.0 || x > 1.0) {
        setError("arcsin domain error");
        return 0.0;
    }
    double result = std::asin(x);
    return isDegrees ? toDegrees(result) : result;
}

double MathEngine::arccosine(double x, bool isDegrees) {
    if (x < -1.0 || x > 1.0) {
        setError("arccos domain error");
        return 0.0;
    }
    double result = std::acos(x);
    return isDegrees ? toDegrees(result) : result;
}

double MathEngine::arctangent(double x, bool isDegrees) {
    double result = std::atan(x);
    return isDegrees ? toDegrees(result) : result;
}

// Hyperbolic functions
double MathEngine::hyperbolicSine(double x) { return std::sinh(x); }
double MathEngine::hyperbolicCosine(double x) { return std::cosh(x); }
double MathEngine::hyperbolicTangent(double x) { return std::tanh(x); }
double MathEngine::hyperbolicArcSine(double x) { return std::asinh(x); }
double MathEngine::hyperbolicArcCosine(double x) {
    if (x < 1.0) { setError("acosh domain error"); return 0.0; }
    return std::acosh(x);
}
double MathEngine::hyperbolicArcTangent(double x) {
    if (x <= -1.0 || x >= 1.0) { setError("atanh domain error"); return 0.0; }
    return std::atanh(x);
}

// Logarithmic functions
double MathEngine::logarithm(double x) {
    if (x <= 0.0) {
        setError("log domain error");
        return 0.0;
    }
    return std::log10(x);
}

double MathEngine::naturalLog(double x) {
    if (x <= 0.0) {
        setError("ln domain error");
        return 0.0;
    }
    return std::log(x);
}

double MathEngine::logBase(double x, double base) {
    if (x <= 0.0 || base <= 0.0 || base == 1.0) {
        setError("log base domain error");
        return 0.0;
    }
    return std::log(x) / std::log(base);
}

// Power and root functions
double MathEngine::power(double base, double exponent) {
    return std::pow(base, exponent);
}

double MathEngine::squareRoot(double x) {
    if (x < 0.0) {
        setError("sqrt of negative number");
        return 0.0;
    }
    return std::sqrt(x);
}

double MathEngine::cubeRoot(double x) {
    return std::cbrt(x);
}

double MathEngine::nthRoot(double x, double n) {
    if (n == 0.0) {
        setError("0th root undefined");
        return 0.0;
    }
    return std::pow(x, 1.0 / n);
}

double MathEngine::exponential(double x) {
    return std::exp(x);
}

// Advanced functions
double MathEngine::factorial(int n) {
    if (n < 0) {
        setError("factorial of negative number");
        return 0.0;
    }
    if (n > 170) {
        setError("factorial overflow");
        return 0.0;
    }
    double result = 1.0;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

double MathEngine::permutation(int n, int r) {
    if (n < 0 || r < 0 || r > n) {
        setError("Invalid permutation parameters");
        return 0.0;
    }
    return factorial(n) / factorial(n - r);
}

double MathEngine::combination(int n, int r) {
    if (n < 0 || r < 0 || r > n) {
        setError("Invalid combination parameters");
        return 0.0;
    }
    return factorial(n) / (factorial(r) * factorial(n - r));
}

double MathEngine::modulo(double a, double b) {
    if (b == 0.0) {
        setError("Modulo by zero");
        return 0.0;
    }
    return std::fmod(a, b);
}

double MathEngine::absolute(double x) {
    return std::abs(x);
}

// Calculus and Sequences
double MathEngine::derivative(const std::string& expr, double point) {
    double h = 1e-6;
    double f_x_plus_h = evaluate(expr, point + h);
    double f_x_minus_h = evaluate(expr, point - h);
    
    if (hasError()) return 0.0;
    
    return (f_x_plus_h - f_x_minus_h) / (2 * h);
}

double MathEngine::integral(const std::string& expr, double lower, double upper) {
    int n = 1000; // Number of intervals (must be even for Simpson's)
    double h = (upper - lower) / n;
    
    double sum = evaluate(expr, lower) + evaluate(expr, upper);
    
    for (int i = 1; i < n; i++) {
        double x = lower + i * h;
        double val = evaluate(expr, x);
        if (hasError()) return 0.0;
        
        if (i % 2 == 0) sum += 2 * val;
        else sum += 4 * val;
    }
    
    return sum * h / 3.0;
}

double MathEngine::limit(const std::string& expr, double point, bool fromRight) {
    double h = 1e-7;
    double val = evaluate(expr, point + (fromRight ? h : -h));
    return val;
}

double MathEngine::summation(const std::string& expr, int start, int end) {
    double total = 0.0;
    for (int i = start; i <= end; i++) {
        total += evaluate(expr, (double)i);
        if (hasError()) return 0.0;
    }
    return total;
}

// Memory operations
void MathEngine::memoryClear() { memory = 0.0; }
void MathEngine::memoryRecall(double& value) { value = memory; }
void MathEngine::memoryAdd(double value) { memory += value; }
void MathEngine::memorySubtract(double value) { memory -= value; }
void MathEngine::memoryStore(double value) { memory = value; }

// Expression evaluation
double MathEngine::evaluate(const std::string& expression) {
    return evaluate(expression, 0.0);
}

double MathEngine::evaluate(const std::string& expression, double x) {
    clearError();
    try {
        // Simple variable substitution for 'x'
        // Note: This is a basic implementation. A robust one would pass x to parseExpression.
        // For now, we'll use a member variable or pass it down.
        // Let's modify parseFactor to handle 'x'.
        this->currentX = x; 
        return parseExpression(expression);
    } catch (const std::exception& e) {
        setError(e.what());
        return 0.0;
    }
}

void MathEngine::skipWhitespace(const std::string& expr, size_t& pos) {
    while (pos < expr.length() && std::isspace(expr[pos])) {
        ++pos;
    }
}

double MathEngine::parseNumber(const std::string& expr, size_t& pos) {
    skipWhitespace(expr, pos);
    size_t start = pos;
    
    if (pos < expr.length() && (expr[pos] == '+' || expr[pos] == '-')) {
        ++pos;
    }
    
    while (pos < expr.length() && (std::isdigit(expr[pos]) || expr[pos] == '.')) {
        ++pos;
    }
    
    if (pos == start || (pos == start + 1 && !std::isdigit(expr[start]))) {
        throw std::runtime_error("Invalid number format");
    }
    
    return std::stod(expr.substr(start, pos - start));
}

double MathEngine::parseFactor(const std::string& expr, size_t& pos) {
    skipWhitespace(expr, pos);
    
    // Handle parentheses
    if (pos < expr.length() && expr[pos] == '(') {
        ++pos;
        double result = parseExpression(expr.substr(pos));
        // Find matching closing parenthesis
        int depth = 1;
        size_t start = pos;
        while (pos < expr.length() && depth > 0) {
            if (expr[pos] == '(') ++depth;
            if (expr[pos] == ')') --depth;
            ++pos;
        }
        size_t subexprPos = 0;
        result = parseExpression(expr.substr(start, pos - start - 1));
        return result;
    }
    
    // Handle functions and variables
    if (pos < expr.length() && std::isalpha(expr[pos])) {
        size_t start = pos;
        while (pos < expr.length() && std::isalpha(expr[pos])) {
            ++pos;
        }
        std::string funcName = expr.substr(start, pos - start);
        skipWhitespace(expr, pos);
        
        // Check for variable 'x'
        if (funcName == "x" || funcName == "X") {
            return currentX;
        }
        
        if (pos >= expr.length() || expr[pos] != '(') {
            // Constants
            if (funcName == "pi" || funcName == "PI") return PI;
            if (funcName == "e" || funcName == "E") return E;
            throw std::runtime_error("Expected '(' after function name");
        }
        
        ++pos; // Skip '('
        
        // Handle multi-argument functions (diff, int, sum, lim)
        std::string lowerFunc = funcName;
        std::transform(lowerFunc.begin(), lowerFunc.end(), lowerFunc.begin(), ::tolower);
        
        if (lowerFunc == "diff" || lowerFunc == "int" || lowerFunc == "sum" || lowerFunc == "lim") {
            // Parse first argument as string expression (until comma)
            size_t startArg = pos;
            int parenDepth = 0;
            while (pos < expr.length()) {
                if (expr[pos] == '(') parenDepth++;
                if (expr[pos] == ')') {
                    if (parenDepth == 0) break;
                    parenDepth--;
                }
                if (expr[pos] == ',' && parenDepth == 0) break;
                pos++;
            }
            
            std::string subExpr = expr.substr(startArg, pos - startArg);
            
            if (pos >= expr.length() || expr[pos] != ',') throw std::runtime_error("Expected ','");
            pos++; // Skip ','
            
            double arg2 = parseExpression(expr.substr(pos)); // This might consume too much if there are more args
            // Actually, we need to parse strictly until next comma or end paren
            // Let's simplify: assume parseExpression stops at comma or paren? No it doesn't.
            // We need a parseArgument helper or just manually parse numbers for now.
            // For simplicity, let's assume arguments 2, 3 are simple numbers or expressions that parseFactor can handle?
            // No, parseExpression consumes until end of string or unmatched operator.
            // We need to implement a robust argument parser. 
            
            // RE-IMPLEMENTATION for robustness:
            // We will recursively call parseExpression but we need to make sure it stops at comma.
            // Since parseExpression calls parseTerm -> parseFactor, we can't easily change stop condition without big refactor.
            // Hack: Extract the substring for the argument.
            
            // Find end of second argument
            startArg = pos;
            parenDepth = 0;
            while (pos < expr.length()) {
                if (expr[pos] == '(') parenDepth++;
                if (expr[pos] == ')') {
                    if (parenDepth == 0) break;
                    parenDepth--;
                }
                if (expr[pos] == ',' && parenDepth == 0) break;
                pos++;
            }
            std::string arg2Str = expr.substr(startArg, pos - startArg);
            double val2 = evaluate(arg2Str, currentX); // Evaluate arg2 (limits/bounds)
            
            if (lowerFunc == "diff" || lowerFunc == "lim") {
                if (pos >= expr.length() || expr[pos] != ')') throw std::runtime_error("Expected ')'");
                pos++;
                if (lowerFunc == "diff") return derivative(subExpr, val2);
                else return limit(subExpr, val2);
            }
            
            if (lowerFunc == "int" || lowerFunc == "sum") {
                if (pos >= expr.length() || expr[pos] != ',') throw std::runtime_error("Expected ','");
                pos++; // Skip ','
                
                // Parse third argument
                startArg = pos;
                parenDepth = 0;
                while (pos < expr.length()) {
                    if (expr[pos] == '(') parenDepth++;
                    if (expr[pos] == ')') {
                        if (parenDepth == 0) break;
                        parenDepth--;
                    }
                    // No more commas expected
                    pos++;
                }
                // Backtrack one if we hit ')'
                if (pos > 0 && expr[pos-1] == ')') pos--;
                
                std::string arg3Str = expr.substr(startArg, pos - startArg);
                double val3 = evaluate(arg3Str, currentX);
                
                if (pos >= expr.length() || expr[pos] != ')') throw std::runtime_error("Expected ')'");
                pos++;
                
                if (lowerFunc == "int") return integral(subExpr, val2, val3);
                else return summation(subExpr, (int)val2, (int)val3);
            }
        }

        double arg = parseFactor(expr, pos);
        skipWhitespace(expr, pos);
        
        if (pos >= expr.length() || expr[pos] != ')') {
            throw std::runtime_error("Expected ')' after function argument");
        }
        ++pos;
        
        return parseFunction(funcName, arg);
    }
    
    // Handle numbers
    return parseNumber(expr, pos);
}

double MathEngine::parseTerm(const std::string& expr, size_t& pos) {
    double result = parseFactor(expr, pos);
    
    while (true) {
        skipWhitespace(expr, pos);
        if (pos >= expr.length()) break;
        
        char op = expr[pos];
        if (op == '*' || op == '/' || op == '%' || op == '^') {
            ++pos;
            double right = parseFactor(expr, pos);
            
            if (op == '*') result *= right;
            else if (op == '/') result = divide(result, right);
            else if (op == '%') result = modulo(result, right);
            else if (op == '^') result = power(result, right);
        } else {
            break;
        }
    }
    
    return result;
}

double MathEngine::parseExpression(const std::string& expr) {
    size_t pos = 0;
    double result = parseTerm(expr, pos);
    
    while (true) {
        skipWhitespace(expr, pos);
        if (pos >= expr.length()) break;
        
        char op = expr[pos];
        if (op == '+' || op == '-') {
            ++pos;
            double right = parseTerm(expr, pos);
            
            if (op == '+') result += right;
            else if (op == '-') result -= right;
        } else {
            break;
        }
    }
    
    return result;
}

double MathEngine::parseFunction(const std::string& funcName, double arg) {
    std::string lower = funcName;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    
    if (lower == "sin") return sine(arg);
    if (lower == "cos") return cosine(arg);
    if (lower == "tan") return tangent(arg);
    if (lower == "asin") return arcsine(arg);
    if (lower == "acos") return arccosine(arg);
    if (lower == "atan") return arctangent(arg);
    if (lower == "log") return logarithm(arg);
    if (lower == "ln") return naturalLog(arg);
    if (lower == "sqrt") return squareRoot(arg);
    if (lower == "cbrt") return cubeRoot(arg);
    if (lower == "exp") return exponential(arg);
    if (lower == "abs") return absolute(arg);
    if (lower == "fact") return factorial(static_cast<int>(arg));
    
    // Hyperbolic
    if (lower == "sinh") return hyperbolicSine(arg);
    if (lower == "cosh") return hyperbolicCosine(arg);
    if (lower == "tanh") return hyperbolicTangent(arg);
    if (lower == "asinh") return hyperbolicArcSine(arg);
    if (lower == "acosh") return hyperbolicArcCosine(arg);
    if (lower == "atanh") return hyperbolicArcTangent(arg);
    
    throw std::runtime_error("Unknown function: " + funcName);
}

void MathEngine::setError(const std::string& error) {
    lastError = error;
}

void MathEngine::clearError() {
    lastError.clear();
}
