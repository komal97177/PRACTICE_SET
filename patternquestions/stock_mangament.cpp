#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <thread>
#include <mutex>
#include <fstream>
#include <sstream>

using namespace std;

mutex dataMutex;

// Product structure
struct Product {
    int id;
    string name;
    string category;
    double price;
    int quantity;
    string lastUpdated;
};

// File-based database simulation
class Database {
private:
    string dbName;
    vector<Product> products;
    int nextId;
    
    string getCurrentDateTime() {
        time_t now = time(0);
        char* dt = ctime(&now);
        string datetime(dt);
        datetime.erase(datetime.find_last_not_of("\n") + 1);
        return datetime;
    }
    
public:
    Database(const string& name) : dbName(name), nextId(1) {
        loadFromFile();
    }
    
    ~Database() {
        saveToFile();
    }
    
    void loadFromFile() {
        lock_guard<mutex> lock(dataMutex);
        ifstream inFile(dbName);
        if (!inFile) {
            cout << "No existing database found. Creating new one." << endl;
            return;
        }
        
        products.clear();
        string line;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            
            stringstream ss(line);
            string token;
            vector<string> tokens;
            
            while (getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            
            if (tokens.size() >= 6) {
                Product p;
                p.id = stoi(tokens[0]);
                p.name = tokens[1];
                p.category = tokens[2];
                p.price = stod(tokens[3]);
                p.quantity = stoi(tokens[4]);
                p.lastUpdated = tokens[5];
                
                products.push_back(p);
                if (p.id >= nextId) nextId = p.id + 1;
            }
        }
        
        inFile.close();
        cout << "Loaded " << products.size() << " products from database." << endl;
    }
    
    void saveToFile() {
        lock_guard<mutex> lock(dataMutex);
        ofstream outFile(dbName);
        if (!outFile) {
            cerr << "Error opening file for writing!" << endl;
            return;
        }
        
        for (const auto& p : products) {
            outFile << p.id << "|" << p.name << "|" << p.category << "|" 
                   << p.price << "|" << p.quantity << "|" << p.lastUpdated << endl;
        }
        
        outFile.close();
    }
    
    int addProduct(const Product& product) {
        lock_guard<mutex> lock(dataMutex);
        Product p = product;
        p.id = nextId++;
        p.lastUpdated = getCurrentDateTime();
        products.push_back(p);
        saveToFile();
        return 0;
    }
    
    int updateProduct(const Product& product) {
        lock_guard<mutex> lock(dataMutex);
        for (auto& p : products) {
            if (p.id == product.id) {
                p.name = product.name;
                p.category = product.category;
                p.price = product.price;
                p.quantity = product.quantity;
                p.lastUpdated = getCurrentDateTime();
                saveToFile();
                return 0;
            }
        }
        return -1;
    }
    
    int deleteProduct(int id) {
        lock_guard<mutex> lock(dataMutex);
        for (auto it = products.begin(); it != products.end(); ++it) {
            if (it->id == id) {
                products.erase(it);
                saveToFile();
                return 0;
            }
        }
        return -1;
    }
    
    vector<Product> getAllProducts() {
        lock_guard<mutex> lock(dataMutex);
        return products;
    }
    
    Product getProductById(int id) {
        lock_guard<mutex> lock(dataMutex);
        for (const auto& p : products) {
            if (p.id == id) {
                return p;
            }
        }
        return Product(); // Return empty product if not found
    }
    
    vector<Product> searchProducts(const string& keyword) {
        lock_guard<mutex> lock(dataMutex);
        vector<Product> results;
        string lowerKeyword = toLower(keyword);
        
        for (const auto& p : products) {
            if (toLower(p.name).find(lowerKeyword) != string::npos ||
                toLower(p.category).find(lowerKeyword) != string::npos) {
                results.push_back(p);
            }
        }
        return results;
    }
    
    string toLower(const string& str) {
        string result = str;
        for (char& c : result) {
            c = tolower(c);
        }
        return result;
    }
};

// StockManager class to handle business logic
class StockManager {
private:
    Database db;
    
public:
    StockManager(const string& dbName) : db(dbName) {}
    
    void addProduct() {
        Product p;
        cout << "Enter product name: ";
        cin.ignore();
        getline(cin, p.name);
        cout << "Enter product category: ";
        getline(cin, p.category);
        cout << "Enter product price: ";
        cin >> p.price;
        cout << "Enter product quantity: ";
        cin >> p.quantity;
        
        if (db.addProduct(p) == 0) {
            cout << "Product added successfully!" << endl;
        } else {
            cout << "Failed to add product!" << endl;
        }
    }
    
    void updateProduct() {
        int id;
        cout << "Enter product ID to update: ";
        cin >> id;
        
        Product p = db.getProductById(id);
        if (p.id == 0) {
            cout << "Product not found!" << endl;
            return;
        }
        
        cout << "Current product details:" << endl;
        displayProduct(p);
        
        cout << "Enter new product name (current: " << p.name << "): ";
        cin.ignore();
        getline(cin, p.name);
        cout << "Enter new product category (current: " << p.category << "): ";
        getline(cin, p.category);
        cout << "Enter new product price (current: " << p.price << "): ";
        cin >> p.price;
        cout << "Enter new product quantity (current: " << p.quantity << "): ";
        cin >> p.quantity;
        
        if (db.updateProduct(p) == 0) {
            cout << "Product updated successfully!" << endl;
        } else {
            cout << "Failed to update product!" << endl;
        }
    }
    
    void deleteProduct() {
        int id;
        cout << "Enter product ID to delete: ";
        cin >> id;
        
        Product p = db.getProductById(id);
        if (p.id == 0) {
            cout << "Product not found!" << endl;
            return;
        }
        
        cout << "Are you sure you want to delete this product? (y/n)" << endl;
        displayProduct(p);
        
        char confirm;
        cin >> confirm;
        if (confirm == 'y' || confirm == 'Y') {
            if (db.deleteProduct(id) == 0) {
                cout << "Product deleted successfully!" << endl;
            } else {
                cout << "Failed to delete product!" << endl;
            }
        }
    }
    
    void displayAllProducts() {
        vector<Product> products = db.getAllProducts();
        if (products.empty()) {
            cout << "No products found!" << endl;
            return;
        }
        
        cout << "\n==================================================================================" << endl;
        cout << "                                 PRODUCT INVENTORY                                 " << endl;
        cout << "==================================================================================" << endl;
        cout << setw(5) << "ID" << setw(20) << "Name" << setw(15) << "Category" 
             << setw(10) << "Price" << setw(10) << "Quantity" << setw(20) << "Last Updated" << endl;
        cout << "----------------------------------------------------------------------------------" << endl;
        
        for (const auto& p : products) {
            cout << setw(5) << p.id << setw(20) << truncateString(p.name, 18) 
                 << setw(15) << truncateString(p.category, 13) << setw(10) << fixed << setprecision(2) << p.price 
                 << setw(10) << p.quantity << setw(20) << truncateString(p.lastUpdated, 16) << endl;
        }
        cout << "==================================================================================" << endl;
    }
    
    void displayProduct(const Product& p) {
        cout << "\nProduct Details:" << endl;
        cout << "ID: " << p.id << endl;
        cout << "Name: " << p.name << endl;
        cout << "Category: " << p.category << endl;
        cout << "Price: $" << fixed << setprecision(2) << p.price << endl;
        cout << "Quantity: " << p.quantity << endl;
        cout << "Last Updated: " << p.lastUpdated << endl;
    }
    
    void searchProducts() {
        string keyword;
        cout << "Enter search keyword: ";
        cin.ignore();
        getline(cin, keyword);
        
        vector<Product> products = db.searchProducts(keyword);
        if (products.empty()) {
            cout << "No products found!" << endl;
            return;
        }
        
        cout << "\nSearch Results:" << endl;
        cout << "==================================================================================" << endl;
        cout << setw(5) << "ID" << setw(20) << "Name" << setw(15) << "Category" 
             << setw(10) << "Price" << setw(10) << "Quantity" << setw(20) << "Last Updated" << endl;
        cout << "----------------------------------------------------------------------------------" << endl;
        
        for (const auto& p : products) {
            cout << setw(5) << p.id << setw(20) << truncateString(p.name, 18) 
                 << setw(15) << truncateString(p.category, 13) << setw(10) << fixed << setprecision(2) << p.price 
                 << setw(10) << p.quantity << setw(20) << truncateString(p.lastUpdated, 16) << endl;
        }
        cout << "==================================================================================" << endl;
    }
    
    void generateLowStockReport() {
        vector<Product> products = db.getAllProducts();
        vector<Product> lowStockProducts;
        
        for (const auto& p : products) {
            if (p.quantity < 10) { // Threshold for low stock
                lowStockProducts.push_back(p);
            }
        }
        
        if (lowStockProducts.empty()) {
            cout << "No products with low stock!" << endl;
            return;
        }
        
        cout << "\nLOW STOCK ALERT REPORT:" << endl;
        cout << "==================================================================================" << endl;
        cout << setw(5) << "ID" << setw(20) << "Name" << setw(15) << "Category" 
             << setw(10) << "Price" << setw(10) << "Quantity" << setw(20) << "Last Updated" << endl;
        cout << "----------------------------------------------------------------------------------" << endl;
        
        for (const auto& p : lowStockProducts) {
            cout << setw(5) << p.id << setw(20) << truncateString(p.name, 18) 
                 << setw(15) << truncateString(p.category, 13) << setw(10) << fixed << setprecision(2) << p.price 
                 << setw(10) << p.quantity << setw(20) << truncateString(p.lastUpdated, 16) << endl;
        }
        cout << "==================================================================================" << endl;
    }
    
    void generateInventoryValueReport() {
        vector<Product> products = db.getAllProducts();
        double totalValue = 0.0;
        
        cout << "\nINVENTORY VALUE REPORT:" << endl;
        cout << "==================================================================================" << endl;
        cout << setw(5) << "ID" << setw(20) << "Name" << setw(15) << "Category" 
             << setw(10) << "Price" << setw(10) << "Quantity" << setw(15) << "Total Value" << endl;
        cout << "----------------------------------------------------------------------------------" << endl;
        
        for (const auto& p : products) {
            double productValue = p.price * p.quantity;
            totalValue += productValue;
            cout << setw(5) << p.id << setw(20) << truncateString(p.name, 18) 
                 << setw(15) << truncateString(p.category, 13) << setw(10) << fixed << setprecision(2) << p.price 
                 << setw(10) << p.quantity << setw(15) << fixed << setprecision(2) << productValue << endl;
        }
        
        cout << "----------------------------------------------------------------------------------" << endl;
        cout << setw(65) << "TOTAL INVENTORY VALUE: $" << setw(15) << fixed << setprecision(2) << totalValue << endl;
        cout << "==================================================================================" << endl;
    }
    
    string truncateString(const string& str, size_t length) {
        if (str.length() <= length) return str;
        return str.substr(0, length - 3) + "...";
    }
};

// Function to display main menu
void displayMenu() {
    cout << "\n========== STOCK MANAGEMENT SYSTEM ==========" << endl;
    cout << "1. Add Product" << endl;
    cout << "2. Update Product" << endl;
    cout << "3. Delete Product" << endl;
    cout << "4. View All Products" << endl;
    cout << "5. Search Products" << endl;
    cout << "6. Generate Low Stock Report" << endl;
    cout << "7. Generate Inventory Value Report" << endl;
    cout << "8. Exit" << endl;
    cout << "=============================================" << endl;
    cout << "Enter your choice: ";
}

// Background thread for automated low stock alerts
void lowStockMonitor(StockManager& sm, int checkInterval) {
    while (true) {
        this_thread::sleep_for(chrono::seconds(checkInterval));
        cout << "\n[BACKGROUND] Checking for low stock items..." << endl;
        sm.generateLowStockReport();
    }
}

int main() {
    StockManager sm("inventory.txt");
    int choice;
    
    // Start background thread for low stock monitoring (checks every 30 seconds)
    thread monitorThread(lowStockMonitor, ref(sm), 30);
    monitorThread.detach();
    
    cout << "Welcome to Stock Management System!" << endl;
    
    do {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                sm.addProduct();
                break;
            case 2:
                sm.updateProduct();
                break;
            case 3:
                sm.deleteProduct();
                break;
            case 4:
                sm.displayAllProducts();
                break;
            case 5:
                sm.searchProducts();
                break;
            case 6:
                sm.generateLowStockReport();
                break;
            case 7:
                sm.generateInventoryValueReport();
                break;
            case 8:
                cout << "Thank you for using Stock Management System!" << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 8);
    
    return 0;
}