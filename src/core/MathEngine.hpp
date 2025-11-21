#pragma once

#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <stdexcept>

class MathEngine {
public:
    MathEngine();
    
    // Main evaluation function
    double evaluate(const std::string& expression);
    double evaluate(const std::string& expression, double x);
    
    // Basic operations
    double add(double a, double b);
    double subtract(double a, double b);
    double multiply(double a, double b);
    double divide(double a, double b);
    
    double power(double base, double exponent);
    double squareRoot(double x);
    double cubeRoot(double x);
    double nthRoot(double x, double n);
    double exponential(double x);    // e^x
    
    // Scientific functions
    double sine(double x, bool isDegrees = true);
    double cosine(double x, bool isDegrees = true);
    double tangent(double x, bool isDegrees = true);
    double arcsine(double x, bool isDegrees = true);
    double arccosine(double x, bool isDegrees = true);
    double arctangent(double x, bool isDegrees = true);
    
    // Hyperbolic functions
    double hyperbolicSine(double x);
    double hyperbolicCosine(double x);
    double hyperbolicTangent(double x);
    double hyperbolicArcSine(double x);
    double hyperbolicArcCosine(double x);
    double hyperbolicArcTangent(double x);
    
    // Logarithmic functions
    double logarithm(double x);      // log base 10
    double naturalLog(double x);     // ln (natural log)
    double logBase(double x, double base);
    
    // Advanced functions
    double factorial(int n);
    double permutation(int n, int r);  // nPr
    double combination(int n, int r);   // nCr
    double modulo(double a, double b);
    double absolute(double x);

    // Calculus and Sequences
    double derivative(const std::string& expr, double point);
    double integral(const std::string& expr, double lower, double upper);
    double limit(const std::string& expr, double point, bool fromRight = true); // Basic limit approximation
    double summation(const std::string& expr, int start, int end);
    
    // Memory operations
    void memoryClear();
    void memoryRecall(double& value);
    void memoryAdd(double value);
    void memorySubtract(double value);
    void memoryStore(double value);
    
    // Constants
    static constexpr double PI = 3.14159265358979323846;
    static constexpr double E = 2.71828182845904523536;
    
    // Error handling
    std::string getLastError() const { return lastError; }
    bool hasError() const { return !lastError.empty(); }
    
private:
    double memory;
    double currentX; // For graphing
    std::string lastError;
    
    // Expression parsing helpers
    double parseExpression(const std::string& expr);
    double parseTerm(const std::string& expr, size_t& pos);
    double parseFactor(const std::string& expr, size_t& pos);
    double parseNumber(const std::string& expr, size_t& pos);
    double parseFunction(const std::string& funcName, double arg);
    
    void skipWhitespace(const std::string& expr, size_t& pos);
    bool isOperator(char c);
    int getPrecedence(char op);
    
    double toRadians(double degrees);
    double toDegrees(double radians);
    
    void setError(const std::string& error);
    void clearError();
};
