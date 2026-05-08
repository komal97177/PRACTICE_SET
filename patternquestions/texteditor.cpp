#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
#include <fstream>
#include <regex>
#include <cctype>

using namespace std;

// Text Editor Class
class TextEditor {
private:
    string text;
    stack<string> undoStack;
    stack<string> redoStack;
    size_t cursorPos;
    
    // Save current state to undo stack
    void saveState() {
        undoStack.push(text);
        // Clear redo stack when new operation is performed
        while (!redoStack.empty()) {
            redoStack.pop();
        }
    }
    
public:
    TextEditor() : cursorPos(0) {
        text = "";
    }
    
    // Insert text at current cursor position
    void insertText(const string& newText) {
        saveState();
        text.insert(cursorPos, newText);
        cursorPos += newText.length();
    }
    
    // Delete n characters from current cursor position
    void deleteChars(int n) {
        if (text.empty() || n == 0) return;
        
        saveState();
        
        if (n > 0) {
            // Delete forward
            if (cursorPos + n > text.length()) {
                n = text.length() - cursorPos;
            }
            text.erase(cursorPos, n);
        } else {
            // Delete backward
            n = -n;
            if (n > cursorPos) {
                n = cursorPos;
            }
            text.erase(cursorPos - n, n);
            cursorPos -= n;
        }
    }
    
    // Move cursor left
    void moveCursorLeft(int n = 1) {
        if (cursorPos >= n) {
            cursorPos -= n;
        } else {
            cursorPos = 0;
        }
    }
    
    // Move cursor right
    void moveCursorRight(int n = 1) {
        if (cursorPos + n <= text.length()) {
            cursorPos += n;
        } else {
            cursorPos = text.length();
        }
    }
    
    // Move cursor to beginning
    void moveCursorToBegin() {
        cursorPos = 0;
    }
    
    // Move cursor to end
    void moveCursorToEnd() {
        cursorPos = text.length();
    }
    
    // Undo last operation
    void undo() {
        if (!undoStack.empty()) {
            redoStack.push(text);
            text = undoStack.top();
            undoStack.pop();
            
            // Adjust cursor position
            if (cursorPos > text.length()) {
                cursorPos = text.length();
            }
        }
    }
    
    // Redo last undone operation
    void redo() {
        if (!redoStack.empty()) {
            undoStack.push(text);
            text = redoStack.top();
            redoStack.pop();
            
            // Adjust cursor position
            if (cursorPos > text.length()) {
                cursorPos = text.length();
            }
        }
    }
    
    // Find text (returns position or string::npos if not found)
    size_t findText(const string& searchText, size_t startPos = 0) {
        return text.find(searchText, startPos);
    }
    
    // Find all occurrences of text
    vector<size_t> findAllOccurrences(const string& searchText) {
        vector<size_t> positions;
        size_t pos = text.find(searchText);
        
        while (pos != string::npos) {
            positions.push_back(pos);
            pos = text.find(searchText, pos + 1);
        }
        
        return positions;
    }
    
    // Replace text
    void replaceText(const string& oldText, const string& newText) {
        saveState();
        
        size_t pos = text.find(oldText);
        while (pos != string::npos) {
            text.replace(pos, oldText.length(), newText);
            pos = text.find(oldText, pos + newText.length());
        }
    }
    
    // Replace text at cursor position
    void replaceAtCursor(const string& newText, int length) {
        saveState();
        
        if (cursorPos + length > text.length()) {
            length = text.length() - cursorPos;
        }
        
        text.replace(cursorPos, length, newText);
        cursorPos += newText.length();
    }
    
    // Get current text
    string getText() const {
        return text;
    }
    
    // Get cursor position
    size_t getCursorPosition() const {
        return cursorPos;
    }
    
    // Set text (for loading from file)
    void setText(const string& newText) {
        saveState();
        text = newText;
        cursorPos = text.length();
    }
    
    // Display text with cursor position
    void display() const {
        cout << "Text: " << text << endl;
        cout << "      ";
        for (size_t i = 0; i < cursorPos; i++) {
            cout << " ";
        }
        cout << "^" << endl;
    }
    
    // Load from file
    bool loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        saveState();
        text = "";
        string line;
        while (getline(file, line)) {
            text += line + "\n";
        }
        
        // Remove the last newline if exists
        if (!text.empty() && text.back() == '\n') {
            text.pop_back();
        }
        
        cursorPos = text.length();
        file.close();
        return true;
    }
    
    // Save to file
    bool saveToFile(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        file << text;
        file.close();
        return true;
    }
    
    // Get word count
    int getWordCount() const {
        if (text.empty()) return 0;
        
        int count = 0;
        bool inWord = false;
        
        for (char c : text) {
            if (isspace(c)) {
                inWord = false;
            } else if (!inWord) {
                count++;
                inWord = true;
            }
        }
        
        return count;
    }
    
    // Get character count
    int getCharCount() const {
        return text.length();
    }
    
    // Get line count
    int getLineCount() const {
        if (text.empty()) return 0;
        
        int count = 1;
        for (char c : text) {
            if (c == '\n') {
                count++;
            }
        }
        
        return count;
    }
};

// Function to display menu
void displayMenu() {
    cout << "\n========== TEXT EDITOR ==========\n";
    cout << "1. Insert text\n";
    cout << "2. Delete characters\n";
    cout << "3. Move cursor\n";
    cout << "4. Undo\n";
    cout << "5. Redo\n";
    cout << "6. Find text\n";
    cout << "7. Replace text\n";
    cout << "8. Display text\n";
    cout << "9. Save to file\n";
    cout << "10. Load from file\n";
    cout << "11. Show statistics\n";
    cout << "12. Exit\n";
    cout << "=================================\n";
    cout << "Enter your choice: ";
}

int main() {
    TextEditor editor;
    int choice;
    string input;
    int n;
    
    cout << "Welcome to Text Editor!" << endl;
    
    do {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear input buffer
        
        switch (choice) {
            case 1: {
                cout << "Enter text to insert: ";
                getline(cin, input);
                editor.insertText(input);
                cout << "Text inserted successfully!" << endl;
                break;
            }
            case 2: {
                cout << "Enter number of characters to delete (positive for forward, negative for backward): ";
                cin >> n;
                editor.deleteChars(n);
                cout << "Characters deleted successfully!" << endl;
                break;
            }
            case 3: {
                cout << "Move cursor - 1: Left, 2: Right, 3: Beginning, 4: End: ";
                cin >> n;
                switch (n) {
                    case 1: 
                        editor.moveCursorLeft();
                        break;
                    case 2:
                        editor.moveCursorRight();
                        break;
                    case 3:
                        editor.moveCursorToBegin();
                        break;
                    case 4:
                        editor.moveCursorToEnd();
                        break;
                    default:
                        cout << "Invalid option!" << endl;
                }
                cout << "Cursor moved. Current position: " << editor.getCursorPosition() << endl;
                break;
            }
            case 4:
                editor.undo();
                cout << "Undo performed!" << endl;
                break;
            case 5:
                editor.redo();
                cout << "Redo performed!" << endl;
                break;
            case 6: {
                cout << "Enter text to find: ";
                getline(cin, input);
                size_t pos = editor.findText(input);
                if (pos != string::npos) {
                    cout << "Text found at position: " << pos << endl;
                    editor.moveCursorToBegin();
                    editor.moveCursorRight(pos);
                } else {
                    cout << "Text not found!" << endl;
                }
                break;
            }
            case 7: {
                string oldText, newText;
                cout << "Enter text to replace: ";
                getline(cin, oldText);
                cout << "Enter new text: ";
                getline(cin, newText);
                editor.replaceText(oldText, newText);
                cout << "Text replaced successfully!" << endl;
                break;
            }
            case 8:
                editor.display();
                break;
            case 9: {
                cout << "Enter filename to save: ";
                getline(cin, input);
                if (editor.saveToFile(input)) {
                    cout << "File saved successfully!" << endl;
                } else {
                    cout << "Error saving file!" << endl;
                }
                break;
            }
            case 10: {
                cout << "Enter filename to load: ";
                getline(cin, input);
                if (editor.loadFromFile(input)) {
                    cout << "File loaded successfully!" << endl;
                } else {
                    cout << "Error loading file!" << endl;
                }
                break;
            }
            case 11: {
                cout << "Statistics:" << endl;
                cout << "Characters: " << editor.getCharCount() << endl;
                cout << "Words: " << editor.getWordCount() << endl;
                cout << "Lines: " << editor.getLineCount() << endl;
                break;
            }
            case 12:
                cout << "Exiting Text Editor. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 12);
    
    return 0;
}