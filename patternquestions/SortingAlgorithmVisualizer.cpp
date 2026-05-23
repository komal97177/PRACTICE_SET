#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
using namespace std;

class SortingVisualizer {
private:
    vector<int> arr;
    
    void displayArray(string algorithm = "") {
        if (!algorithm.empty()) {
            cout << algorithm << ": ";
        }
        for (int num : arr) {
            cout << num << " ";
        }
        cout << endl;
    }
    
    void delay() {
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    
public:
    void setArray(vector<int> input) {
        arr = input;
    }
    
    void bubbleSort() {
        cout << "Initial array: ";
        displayArray();
        
        for (int i = 0; i < arr.size() - 1; i++) {
            for (int j = 0; j < arr.size() - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                    cout << "Step: ";
                    displayArray();
                    delay();
                }
            }
        }
        cout << "Sorted array: ";
        displayArray("Bubble Sort");
    }
    
    void selectionSort() {
        cout << "Initial array: ";
        displayArray();
        
        for (int i = 0; i < arr.size() - 1; i++) {
            int minIdx = i;
            for (int j = i + 1; j < arr.size(); j++) {
                if (arr[j] < arr[minIdx]) {
                    minIdx = j;
                }
            }
            swap(arr[i], arr[minIdx]);
            cout << "Step: ";
            displayArray();
            delay();
        }
        cout << "Sorted array: ";
        displayArray("Selection Sort");
    }
    
    void insertionSort() {
        cout << "Initial array: ";
        displayArray();
        
        for (int i = 1; i < arr.size(); i++) {
            int key = arr[i];
            int j = i - 1;
            
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
                cout << "Step: ";
                displayArray();
                delay();
            }
            arr[j + 1] = key;
            cout << "Step: ";
            displayArray();
            delay();
        }
        cout << "Sorted array: ";
        displayArray("Insertion Sort");
    }
};

int main() {
    SortingVisualizer sorter;
    vector<int> arr;
    int n, value, choice;
    
    cout << "Enter number of elements: ";
    cin >> n;
    
    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> value;
        arr.push_back(value);
    }
    
    sorter.setArray(arr);
    
    do {
        cout << "\nSorting Algorithm Visualizer\n";
        cout << "1. Bubble Sort\n";
        cout << "2. Selection Sort\n";
        cout << "3. Insertion Sort\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                sorter.bubbleSort();
                break;
            case 2:
                sorter.selectionSort();
                break;
            case 3:
                sorter.insertionSort();
                break;
            case 4:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 4);
    
    return 0;
}