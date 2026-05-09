#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <regex>
#include <algorithm>
#include <sstream>
using namespace std;

class ComplexityAnalyzer {
private:
    vector<string> codeLines;
    map<string, int> recursiveCalls;
    
    regex forLoopPattern{"for\\s*\\([^;]*;[^;]*;[^)]*\\)"};
    regex whileLoopPattern{"while\\s*\\([^)]*\\)"};
    regex doWhilePattern{"do\\s*\\{[^}]*\\}\\s*while\\s*\\([^)]*\\)"};
    regex recursiveCallPattern{"\\b(\\w+)\\s*\\([^)]*\\)\\s*(?://.*|/\\*.*\\*/)??\\s*//\\s*recursive"};
    regex dataStructurePattern{"(vector|list|map|set|unordered_map|unordered_set|queue|stack|deque|array)\\s*<.*>\\s*\\w+"};
    regex nestedLoopPattern{"for.*\\{.*for.*\\{.*\\}.*\\}"};
    regex arrayDeclarationPattern{"\\w+\\s+\\w+\\s*\\[\\s*\\w*\\s*\\]"};
    regex dynamicAllocPattern{"(new|malloc|calloc|realloc)\\s*\\([^)]*\\)"};
    
public:
    ComplexityAnalyzer(const vector<string>& code) : codeLines(code) {}
    
    void analyzeTimeComplexity() {
        int loopDepth = 0;
        int maxNestedLevel = 0;
        int currentNestedLevel = 0;
        string timeComplexity = "O(1)";
        bool hasRecursion = false;
        
        for (const auto& line : codeLines) {
            // Remove comments for better analysis
            string cleanLine = line.substr(0, line.find("//"));
            
            // Check for loops and update nesting level
            if (regex_search(cleanLine, forLoopPattern) || 
                regex_search(cleanLine, whileLoopPattern) ||
                regex_search(cleanLine, doWhilePattern)) {
                
                currentNestedLevel++;
                maxNestedLevel = max(maxNestedLevel, currentNestedLevel);
                loopDepth++;
            }
            
            // Check for closing braces to decrease nesting level
            if (cleanLine.find('}') != string::npos) {
                currentNestedLevel = max(0, currentNestedLevel - 1);
            }
            
            // Check for recursive calls
            smatch match;
            if (regex_search(line, match, recursiveCallPattern)) {
                string funcName = match[1];
                recursiveCalls[funcName]++;
                hasRecursion = true;
            }
        }
        
        // Determine time complexity based on maximum nested level
        switch (maxNestedLevel) {
            case 0: timeComplexity = "O(1)"; break;
            case 1: timeComplexity = "O(n)"; break;
            case 2: timeComplexity = "O(n²)"; break;
            case 3: timeComplexity = "O(n³)"; break;
            default: timeComplexity = "O(n^" + to_string(maxNestedLevel) + ")";
        }
        
        // Adjust for recursion
        if (hasRecursion) {
            if (timeComplexity == "O(1)") {
                timeComplexity = "Likely O(2ⁿ) or O(n!) - needs manual review";
            } else {
                timeComplexity += " + recursive component";
            }
        }
        
        cout << "Estimated Time Complexity: " << timeComplexity << endl;
        cout << "Maximum loop nesting level: " << maxNestedLevel << endl;
        cout << "Total loops detected: " << loopDepth << endl;
        
        if (!recursiveCalls.empty()) {
            cout << "Recursive functions found: ";
            for (const auto& [func, count] : recursiveCalls) {
                cout << func << " (" << count << " calls) ";
            }
            cout << endl;
        }
    }
    
    void analyzeSpaceComplexity() {
        int spaceComplexityLevel = 0;
        vector<string> usedDS;
        vector<string> variables;
        
        for (const auto& line : codeLines) {
            // Remove comments
            string cleanLine = line.substr(0, line.find("//"));
            
            // Check for data structure declarations
            smatch match;
            if (regex_search(cleanLine, match, dataStructurePattern)) {
                string dsType = match[1];
                usedDS.push_back(dsType);
                
                if (dsType == "vector" || dsType == "list" || dsType == "deque" || 
                    dsType == "queue" || dsType == "stack" || dsType == "array") {
                    spaceComplexityLevel = max(spaceComplexityLevel, 1);
                } else if (dsType == "map" || dsType == "set" || 
                          dsType == "unordered_map" || dsType == "unordered_set") {
                    spaceComplexityLevel = max(spaceComplexityLevel, 2);
                }
            }
            
            // Check for array declarations
            if (regex_search(cleanLine, arrayDeclarationPattern)) {
                spaceComplexityLevel = max(spaceComplexityLevel, 1);
                size_t bracketPos = cleanLine.find('[');
                if (bracketPos != string::npos) {
                    usedDS.push_back("array");
                }
            }
            
            // Check for dynamic memory allocation
            if (regex_search(cleanLine, dynamicAllocPattern)) {
                spaceComplexityLevel = max(spaceComplexityLevel, 1);
                usedDS.push_back("dynamic memory");
            }
            
            // Simple variable detection (basic types)
            if (cleanLine.find("int ") != string::npos || cleanLine.find("double ") != string::npos ||
                cleanLine.find("float ") != string::npos || cleanLine.find("char ") != string::npos ||
                cleanLine.find("bool ") != string::npos) {
                
                if (cleanLine.find('=') == string::npos && cleanLine.find('[') == string::npos) {
                    variables.push_back("primitive");
                }
            }
        }
        
        string spaceComplexity;
        switch (spaceComplexityLevel) {
            case 0: spaceComplexity = "O(1) (constant space)"; break;
            case 1: spaceComplexity = "O(n) (linear space)"; break;
            case 2: spaceComplexity = "O(n) or O(n log n)"; break;
            default: spaceComplexity = "O(n) (linear space)";
        }
        
        cout << "Estimated Space Complexity: " << spaceComplexity << endl;
        if (!usedDS.empty()) {
            cout << "Data structures detected: ";
            for (const auto& ds : usedDS) {
                cout << ds << " ";
            }
            cout << endl;
        }
        cout << "Primitive variables detected: " << variables.size() << endl;
    }
    
    void printAnalysis() {
        cout << "\n=== CODE COMPLEXITY ANALYSIS ===" << endl;
        analyzeTimeComplexity();
        cout << "------------------------------" << endl;
        analyzeSpaceComplexity();
        cout << "==============================" << endl;
    }
};

vector<string> getCodeFromUser() {
    vector<string> codeLines;
    string line;
    
    cout << "Enter your C++ code (type 'END' on a new line to finish):" << endl;
    cout << "Start typing your code:" << endl;
    
    while (getline(cin, line)) {
        if (line == "END") {
            break;
        }
        codeLines.push_back(line);
    }
    
    return codeLines;
}

int main() {
    cout << "C++ Code Complexity Analyzer" << endl;
    cout << "============================" << endl;
    
    vector<string> userCode = getCodeFromUser();
    
    if (userCode.empty()) {
        cout << "No code entered. Exiting..." << endl;
        return 0;
    }
    
    cout << "\nAnalyzing your code..." << endl;
    cout << "Code received (" << userCode.size() << " lines):" << endl;
    cout << "----------------------------------------" << endl;
    for (const auto& line : userCode) {
        cout << line << endl;
    }
    cout << "----------------------------------------" << endl;
    
    ComplexityAnalyzer analyzer(userCode);
    analyzer.printAnalysis();
    
    cout << "\nAnalysis complete!" << endl;
    
    return 0;
}