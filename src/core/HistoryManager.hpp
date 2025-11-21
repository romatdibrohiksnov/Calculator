#pragma once

#include <string>
#include <vector>
#include <deque>
#include <fstream>

struct HistoryEntry {
    std::string expression;
    std::string result;
    std::string timestamp;
    
    HistoryEntry(const std::string& expr, const std::string& res, const std::string& time)
        : expression(expr), result(res), timestamp(time) {}
};

class HistoryManager {
public:
    HistoryManager(size_t maxSize = 100) : maxHistorySize(maxSize) {}
    
    void addEntry(const std::string& expression, const std::string& result) {
        std::string timestamp = getCurrentTimestamp();
        history.emplace_front(expression, result, timestamp);
        
        if (history.size() > maxHistorySize) {
            history.pop_back();
        }
    }
    
    void clear() {
        history.clear();
    }
    
    const std::deque<HistoryEntry>& getHistory() const {
        return history;
    }
    
    size_t size() const {
        return history.size();
    }
    
    bool isEmpty() const {
        return history.empty();
    }
    
    const HistoryEntry& getEntry(size_t index) const {
        if (index >= history.size()) {
            throw std::out_of_range("History index out of range");
        }
        return history[index];
    }
    
    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        file << "Calculator History\n";
        file << "==================\n\n";
        
        for (const auto& entry : history) {
            file << "[" << entry.timestamp << "]\n";
            file << entry.expression << " = " << entry.result << "\n\n";
        }
        
        file.close();
        return true;
    }
    
    std::string getLastExpression() const {
        if (history.empty()) return "";
        return history.front().expression;
    }
    
    std::string getLastResult() const {
        if (history.empty()) return "";
        return history.front().result;
    }
    
private:
    std::deque<HistoryEntry> history;
    size_t maxHistorySize;
    
    std::string getCurrentTimestamp() const {
        auto now = std::time(nullptr);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return std::string(buffer);
    }
};
