#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cctype>
#include <iomanip>

using namespace std;

// Task structure
struct Task {
    int id;
    string description;
    int priority;  // 1: High, 2: Medium, 3: Low
    string due_date;
    bool completed;
    Task* next;

    Task(int id, const string& desc, int pri, const string& date) 
        : id(id), description(desc), priority(pri), due_date(date), completed(false), next(nullptr) {}
};

// Linked list class for tasks
class TaskList {
private:
    Task* head;
    int count;

public:
    TaskList() : head(nullptr), count(0) {}
    ~TaskList() {
        clear();
    }

    // Add a new task to the list
    void addTask(const string& description, int priority, const string& due_date) {
        Task* new_task = new Task(++count, description, priority, due_date);
        new_task->next = head;
        head = new_task;
    }

    // Display all tasks
    void displayTasks() const {
        if (head == nullptr) {
            cout << "No tasks in the to-do list.\n";
            return;
        }

        cout << "\n--- To-Do List ---\n";
        cout << setw(4) << "ID" << " | " << setw(40) << left << "Description" << " | " 
             << setw(8) << "Priority" << " | " << setw(10) << "Due Date" << " | " 
             << setw(9) << "Status" << endl;
        cout << string(80, '-') << endl;
        
        Task* current = head;
        while (current != nullptr) {
            cout << setw(4) << current->id << " | " << setw(40) << left 
                 << (current->description.length() > 40 ? current->description.substr(0, 37) + "..." : current->description)
                 << " | " << setw(8) << getPriorityString(current->priority) << " | " 
                 << setw(10) << (current->due_date.empty() ? "None" : current->due_date) << " | " 
                 << setw(9) << (current->completed ? "Completed" : "Pending") << endl;
            current = current->next;
        }
    }

    // Mark a task as completed
    void markCompleted(int id) {
        Task* task = findTask(id);
        if (task) {
            task->completed = true;
            cout << "Task marked as completed!\n";
        } else {
            cout << "Task with ID " << id << " not found.\n";
        }
    }

    // Delete a task from the list
    void deleteTask(int id) {
        Task* current = head;
        Task* prev = nullptr;
        
        while (current != nullptr) {
            if (current->id == id) {
                if (prev == nullptr) {
                    // Deleting the head
                    head = current->next;
                } else {
                    prev->next = current->next;
                }
                
                delete current;
                cout << "Task deleted successfully!\n";
                return;
            }
            
            prev = current;
            current = current->next;
        }
        
        cout << "Task with ID " << id << " not found.\n";
    }

    // Sort tasks by priority (using bubble sort)
    void sortByPriority() {
        if (head == nullptr || head->next == nullptr) {
            return; // Empty list or single element
        }
        
        bool swapped;
        do {
            swapped = false;
            Task* current = head;
            Task* prev = nullptr;
            
            while (current->next != nullptr) {
                if (current->priority > current->next->priority) {
                    // Swap nodes
                    if (prev == nullptr) {
                        // Swapping head
                        Task* next = current->next;
                        current->next = next->next;
                        next->next = current;
                        head = next;
                    } else {
                        // Swapping middle nodes
                        Task* next = current->next;
                        current->next = next->next;
                        next->next = current;
                        prev->next = next;
                    }
                    swapped = true;
                }
                prev = current;
                current = current->next;
            }
        } while (swapped);
    }

    // Sort tasks by due date
    void sortByDate() {
        if (head == nullptr || head->next == nullptr) {
            return; // Empty list or single element
        }
        
        bool swapped;
        do {
            swapped = false;
            Task* current = head;
            Task* prev = nullptr;
            
            while (current->next != nullptr) {
                bool should_swap = false;
                
                if (current->due_date.empty() && !current->next->due_date.empty()) {
                    should_swap = true; // Move tasks without dates to the end
                } else if (!current->due_date.empty() && !current->next->due_date.empty()) {
                    if (current->due_date > current->next->due_date) {
                        should_swap = true;
                    }
                }
                
                if (should_swap) {
                    // Swap nodes
                    if (prev == nullptr) {
                        // Swapping head
                        Task* next = current->next;
                        current->next = next->next;
                        next->next = current;
                        head = next;
                    } else {
                        // Swapping middle nodes
                        Task* next = current->next;
                        current->next = next->next;
                        next->next = current;
                        prev->next = next;
                    }
                    swapped = true;
                }
                prev = current;
                current = current->next;
            }
        } while (swapped);
    }

    // Search for tasks by keyword
    vector<Task*> searchTasks(const string& keyword) const {
        vector<Task*> results;
        string lower_keyword = toLowercase(keyword);
        
        Task* current = head;
        while (current != nullptr) {
            string lower_description = toLowercase(current->description);
            if (lower_description.find(lower_keyword) != string::npos) {
                results.push_back(current);
            }
            current = current->next;
        }
        
        return results;
    }

    // Save tasks to a file
    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file for writing!\n";
            return;
        }
        
        Task* current = head;
        while (current != nullptr) {
            file << current->id << "|" << current->description << "|" 
                 << current->priority << "|" << current->due_date << "|" 
                 << current->completed << "\n";
            current = current->next;
        }
        
        file.close();
        cout << "Tasks saved to " << filename << " successfully!\n";
    }

    // Load tasks from a file
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "File not found. Creating new task list.\n";
            return;
        }
        
        // Clear existing tasks
        clear();
        
        string line;
        while (getline(file, line)) {
            size_t pos = 0;
            vector<string> tokens;
            
            while ((pos = line.find('|')) != string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + 1);
            }
            tokens.push_back(line); // Last token
            
            if (tokens.size() >= 5) {
                int id = stoi(tokens[0]);
                string description = tokens[1];
                int priority = stoi(tokens[2]);
                string due_date = tokens[3];
                bool completed = static_cast<bool>(stoi(tokens[4]));
                
                Task* new_task = new Task(id, description, priority, due_date);
                new_task->completed = completed;
                new_task->next = head;
                head = new_task;
                
                if (id > count) count = id;
            }
        }
        
        file.close();
        cout << "Tasks loaded from " << filename << " successfully!\n";
    }

private:
    // Helper function to find a task by ID
    Task* findTask(int id) {
        Task* current = head;
        while (current != nullptr) {
            if (current->id == id) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    // Helper function to convert priority to string
    string getPriorityString(int priority) const {
        switch (priority) {
            case 1: return "High";
            case 2: return "Medium";
            case 3: return "Low";
            default: return "Unknown";
        }
    }

    // Helper function to convert string to lowercase
    string toLowercase(const string& str) const {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    // Clear all tasks from the list
    void clear() {
        Task* current = head;
        while (current != nullptr) {
            Task* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
        count = 0;
    }
};

// Function to validate date format (YYYY-MM-DD)
bool validateDate(const string& date) {
    if (date.empty()) return true; // Empty date is allowed
    
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }
    
    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));
    
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    
    return true;
}

// Main function
int main() {
    TaskList todo_list;
    int choice, priority, id;
    string description, due_date, filename, keyword;
    
    cout << "Welcome to the To-Do List Manager!\n";
    
    // Load tasks from file if exists
    todo_list.loadFromFile("todo_data.txt");
    
    while (true) {
        cout << "\n--- To-Do List Menu ---\n";
        cout << "1. Add Task\n";
        cout << "2. View Tasks\n";
        cout << "3. Mark Task as Completed\n";
        cout << "4. Delete Task\n";
        cout << "5. Sort Tasks by Priority\n";
        cout << "6. Sort Tasks by Due Date\n";
        cout << "7. Search Task\n";
        cout << "8. Save Tasks to File\n";
        cout << "9. Load Tasks from File\n";
        cout << "10. Exit\n";
        cout << "Enter your choice: ";
        
        cin >> choice;
        cin.ignore(); // Clear input buffer
        
        switch (choice) {
            case 1:
                cout << "Enter task description: ";
                getline(cin, description);
                
                cout << "Enter priority (1: High, 2: Medium, 3: Low): ";
                cin >> priority;
                if (priority < 1 || priority > 3) {
                    cout << "Invalid priority. Setting to Medium (2).\n";
                    priority = 2;
                }
                
                cout << "Enter due date (YYYY-MM-DD) or press enter for none: ";
                cin.ignore();
                getline(cin, due_date);
                
                if (!due_date.empty() && !validateDate(due_date)) {
                    cout << "Invalid date format. Task added without due date.\n";
                    due_date = "";
                }
                
                todo_list.addTask(description, priority, due_date);
                cout << "Task added successfully!\n";
                break;
                
            case 2:
                todo_list.displayTasks();
                break;
                
            case 3:
                cout << "Enter task ID to mark as completed: ";
                cin >> id;
                todo_list.markCompleted(id);
                break;
                
            case 4:
                cout << "Enter task ID to delete: ";
                cin >> id;
                todo_list.deleteTask(id);
                break;
                
            case 5:
                todo_list.sortByPriority();
                cout << "Tasks sorted by priority!\n";
                break;
                
            case 6:
                todo_list.sortByDate();
                cout << "Tasks sorted by due date!\n";
                break;
                
            case 7:
                cout << "Enter keyword to search: ";
                getline(cin, keyword);
                
                {
                    vector<Task*> results = todo_list.searchTasks(keyword);
                    if (!results.empty()) {
                        cout << "\nSearch Results:\n";
                        cout << setw(4) << "ID" << " | " << setw(40) << left << "Description" << " | " 
                             << setw(8) << "Priority" << " | " << setw(10) << "Due Date" << " | " 
                             << setw(9) << "Status" << endl;
                        cout << string(80, '-') << endl;
                        
                        for (Task* task : results) {
                            cout << setw(4) << task->id << " | " << setw(40) << left 
                                 << (task->description.length() > 40 ? task->description.substr(0, 37) + "..." : task->description)
                                 << " | " << setw(8) << (task->priority == 1 ? "High" : (task->priority == 2 ? "Medium" : "Low")) << " | " 
                                 << setw(10) << (task->due_date.empty() ? "None" : task->due_date) << " | " 
                                 << setw(9) << (task->completed ? "Completed" : "Pending") << endl;
                        }
                    } else {
                        cout << "No tasks found with that keyword.\n";
                    }
                }
                break;
                
            case 8:
                cout << "Enter filename to save: ";
                getline(cin, filename);
                todo_list.saveToFile(filename);
                break;
                
            case 9:
                cout << "Enter filename to load: ";
                getline(cin, filename);
                todo_list.loadFromFile(filename);
                break;
                
            case 10:
                todo_list.saveToFile("todo_data.txt");
                cout << "Goodbye! Your tasks have been saved to todo_data.txt\n";
                return 0;
                
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    
    return 0;
}