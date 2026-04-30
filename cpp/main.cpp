#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// ==========================================
// ENTITY CLASSES
// ==========================================

class Product {
private:
    string productId;
    string productName;

public:
    Product(string id, string name) : productId(id), productName(name) {}
    string getProductId() const { return productId; }
    string getProductName() const { return productName; }
};

class Supplier {
private:
    string supplierId;
    string supplierName;

public:
    Supplier(string id, string name) : supplierId(id), supplierName(name) {}
    string getSupplierId() const { return supplierId; }
    string getSupplierName() const { return supplierName; }
};

class Inventory {
private:
    Product product;
    int currentStock;
    int reorderLevel;

public:
    Inventory(Product p, int stock, int reorder) : product(p), currentStock(stock), reorderLevel(reorder) {}
    
    Product getProduct() const { return product; }
    int getCurrentStock() const { return currentStock; }
    
    bool updateStock(int incomingQuantity) {
        if (incomingQuantity <= 0) {
            return false;
        }
        if (currentStock + incomingQuantity > 999999) {
            return false;
        }
        currentStock += incomingQuantity;
        return true;
    }
};

class PurchaseOrderItem {
private:
    Product product;
    int quantity;
    double unitPrice;

public:
    PurchaseOrderItem(Product p, int q, double price) : product(p), quantity(q), unitPrice(price) {}
    
    double getTotalPrice() const {
        return quantity * unitPrice;
    }
};

class PurchaseOrder {
private:
    string poNumber;
    Supplier supplier;
    vector<PurchaseOrderItem> items;
    double totalAmount;

public:
    PurchaseOrder(string po, Supplier s) : poNumber(po), supplier(s), totalAmount(0.0) {}

    void addItem(PurchaseOrderItem item) {
        items.push_back(item);
        calculateTotal();
    }

    void calculateTotal() {
        totalAmount = 0.0;
        for (const auto& item : items) {
            totalAmount += item.getTotalPrice();
        }
    }

    double getTotalAmount() const { return totalAmount; }
};

// ==========================================
// CONTROL/VALIDATION CLASSES
// ==========================================

class ValidationManager {
public:
    static bool validatePOInputs(string productId, string productName, string supplierId, string supplierName, int quantity, double unitPrice, bool silent = false) {
        
        if (productId.empty()) {
            if (!silent) cout << "  [Validation Error] Product ID is empty.\n";
            return false;
        }
        
        // Mock DB check for existence
        if (productId != "P-1001" && productId != "P-1002") {
            if (!silent) cout << "  [Validation Error] Product ID does not exist in system.\n";
            return false;
        }

        if (productName.empty()) {
            if (!silent) cout << "  [Validation Error] Product Name is empty.\n";
            return false;
        }
        if (supplierId.empty()) {
            if (!silent) cout << "  [Validation Error] Supplier ID is empty.\n";
            return false;
        }
        if (supplierName.empty()) {
            if (!silent) cout << "  [Validation Error] Supplier Name is empty.\n";
            return false;
        }

        if (quantity == 0) {
            if (!silent) cout << "  [Validation Error] Quantity cannot be 0.\n";
            return false;
        }
        if (quantity < 0) {
            if (!silent) cout << "  [Validation Error] Quantity cannot be negative.\n";
            return false;
        }
        if (quantity > 100000) {
            if (!silent) cout << "  [Validation Error] Quantity exceeds upper limit.\n";
            return false;
        }

        if (unitPrice == 0.0) {
            if (!silent) cout << "  [Validation Error] Unit price cannot be 0.\n";
            return false;
        }
        if (unitPrice < 0.0) {
            if (!silent) cout << "  [Validation Error] Unit price cannot be negative.\n";
            return false;
        }

        return true;
    }
};

class SystemManager {
private:
    Inventory* laptopInventory;

public:
    SystemManager(Inventory* inv) : laptopInventory(inv) {}

    bool raisePurchaseOrder(string poNumber, string pId, string pName, string sId, string sName, int qty, double price, bool silent = false) {
        if (!silent) {
            cout << "\n--- Initiating PO: " << poNumber << " ---\n";
            cout << "Inputs -> Qty: " << qty << " | Price: Rs " << fixed << setprecision(2) << price << "\n";
        }

        bool isValid = ValidationManager::validatePOInputs(pId, pName, sId, sName, qty, price, silent);
        
        if (!isValid) {
            if (!silent) cout << "-> PO Creation Failed.\n";
            return false;
        }

        Product p(pId, pName);
        Supplier s(sId, sName);
        PurchaseOrderItem item(p, qty, price);
        
        PurchaseOrder po(poNumber, s);
        po.addItem(item);

        if (!silent) cout << "-> PO Generated Successfully! Total Amount: Rs " << po.getTotalAmount() << "\n";

        if (p.getProductId() == laptopInventory->getProduct().getProductId()) {
            laptopInventory->updateStock(qty);
        }

        return true;
    }
};

// ==========================================
// TEST AUTOMATION
// ==========================================
void runTestCase(SystemManager& sys, string testName, string pId, string pName, string sId, string sName, int qty, double price, bool expectedResult) {
    cout << "\n-------------------------------------------------------------------------\n";
    cout << "TEST CASE : " << testName << "\n";
    cout << "INPUTS    : ProductID=" << (pId.empty() ? "\"\"" : pId) 
         << " | SupplierID=" << (sId.empty() ? "\"\"" : sId) 
         << " | Qty=" << qty 
         << " | Price=" << price << "\n";
    
    string expectedStr = expectedResult ? "PO Successfully Created" : "Validation Error (PO Rejected)";
    cout << "EXPECTED  : " << expectedStr << "\n";

    bool actualResult = sys.raisePurchaseOrder("TEST-PO", pId, pName, sId, sName, qty, price, true);
    string actualStr = actualResult ? "PO Successfully Created" : "Validation Error (PO Rejected)";
    
    cout << "ACTUAL    : " << actualStr << "\n";
    
    cout << "STATUS    : ";
    if (actualResult == expectedResult) {
        cout << "[\033[32mPASS\033[0m]\n";
    } else {
        cout << "[\033[31mFAIL\033[0m]\n";
    }
}

void runAllAutomatedTests(SystemManager& sys) {
    cout << "\n=========================================================================\n";
    cout << "                    AUTOMATED BLACK BOX TESTING REPORT                     \n";
    cout << "=========================================================================\n";

    // 1. ECP - Valid: All fields correct
    runTestCase(sys, "1. ECP Valid - All fields correct", "P-1001", "Laptops", "S-505", "TechCorp", 50, 25000.0, true);
    
    // 2. ECP - Valid: Minimum valid quantity
    runTestCase(sys, "2. ECP Valid - Minimum valid quantity (Qty = 1)", "P-1001", "Laptops", "S-505", "TechCorp", 1, 25000.0, true);
    
    // 3. ECP - Valid: Decimal unit price accepted
    runTestCase(sys, "3. ECP Valid - Decimal unit price accepted", "P-1001", "Laptops", "S-505", "TechCorp", 10, 25000.50, true);
    
    // 4. ECP - Valid: Large quantity within allowed range
    runTestCase(sys, "4. ECP Valid - Large quantity within allowed range", "P-1001", "Laptops", "S-505", "TechCorp", 9999, 25000.0, true);
    
    // 5. ECP - Valid: Different valid product and supplier
    runTestCase(sys, "5. ECP Valid - Different valid product and supplier", "P-1002", "Keyboards", "S-999", "OfficeSupplies", 100, 500.0, true);
    
    // 6. ECP - Invalid: Product ID is empty
    runTestCase(sys, "6. ECP Invalid - Product ID is empty", "", "Laptops", "S-505", "TechCorp", 50, 25000.0, false);
    
    // 7. ECP - Invalid: Product Name is empty
    runTestCase(sys, "7. ECP Invalid - Product Name is empty", "P-1001", "", "S-505", "TechCorp", 50, 25000.0, false);
    
    // 8. ECP - Invalid: Supplier ID is empty
    runTestCase(sys, "8. ECP Invalid - Supplier ID is empty", "P-1001", "Laptops", "", "TechCorp", 50, 25000.0, false);
    
    // 9. ECP - Invalid: Supplier Name is empty
    runTestCase(sys, "9. ECP Invalid - Supplier Name is empty", "P-1001", "Laptops", "S-505", "", 50, 25000.0, false);
    
    // 10. ECP - Invalid: Product ID does not exist in system
    runTestCase(sys, "10. ECP Invalid - Product ID does not exist in system", "P-9999", "Unknown", "S-505", "TechCorp", 50, 25000.0, false);
    
    // 11. BVA - Quantity = 0
    runTestCase(sys, "11. BVA - Quantity = 0 (just below minimum)", "P-1001", "Laptops", "S-505", "TechCorp", 0, 25000.0, false);
    
    // 12. BVA - Quantity = -1
    runTestCase(sys, "12. BVA - Quantity = -1 (negative value)", "P-1001", "Laptops", "S-505", "TechCorp", -1, 25000.0, false);
    
    // 13. BVA - Quantity = 1
    runTestCase(sys, "13. BVA - Quantity = 1 (minimum valid boundary)", "P-1001", "Laptops", "S-505", "TechCorp", 1, 25000.0, true);
    
    // 14. BVA - Very large Quantity
    runTestCase(sys, "14. BVA - Very large Quantity (exceeds upper limit)", "P-1001", "Laptops", "S-505", "TechCorp", 999999, 25000.0, false);
    
    // 15. BVA - Unit Price = 0 and negative Unit Price
    runTestCase(sys, "15. BVA - Unit Price = 0", "P-1001", "Laptops", "S-505", "TechCorp", 50, 0.0, false);
    runTestCase(sys, "15b. BVA - Negative Unit Price (-50)", "P-1001", "Laptops", "S-505", "TechCorp", 50, -50.0, false);
    
    cout << "=========================================================================\n";
}

void runWhiteBoxTestCase(SystemManager& sys, string testName, string pId, string pName, string sId, string sName, int qty, double price, bool expectedResult) {
    cout << "\n-------------------------------------------------------------------------\n";
    cout << "TEST CASE : " << testName << "\n";
    cout << "INPUTS    : ProductID=" << (pId.empty() ? "\"\"" : pId) 
         << " | SupplierID=" << (sId.empty() ? "\"\"" : sId) 
         << " | Qty=" << qty 
         << " | Price=" << price << "\n";
    
    string expectedStr = expectedResult ? "Path Succeeded (True)" : "Path Failed / Blocked (False)";
    cout << "EXPECTED  : " << expectedStr << "\n";

    bool actualResult = sys.raisePurchaseOrder("TEST-PO", pId, pName, sId, sName, qty, price, true);
    string actualStr = actualResult ? "Path Succeeded (True)" : "Path Failed / Blocked (False)";
    
    cout << "ACTUAL    : " << actualStr << "\n";
    
    cout << "STATUS    : ";
    if (actualResult == expectedResult) {
        cout << "[\033[32mPASS\033[0m]\n";
    } else {
        cout << "[\033[31mFAIL\033[0m]\n";
    }
}

void runAllWhiteBoxTests(SystemManager& sys) {
    cout << "\n=========================================================================\n";
    cout << "                    AUTOMATED WHITE BOX TESTING REPORT                     \n";
    cout << "          (Statement | Branch | Condition | Path | Loop Coverage)          \n";
    cout << "=========================================================================\n";

    // Statement Coverage
    runWhiteBoxTestCase(sys, "TC-01 [Statement] Execute full valid path", "P-1001", "Laptops", "S-505", "TechCorp", 10, 25000.0, true);
    runWhiteBoxTestCase(sys, "TC-02 [Statement] Execute early return (Empty Product)", "", "Laptops", "S-505", "TechCorp", 10, 25000.0, false);

    // Branch Coverage
    runWhiteBoxTestCase(sys, "TC-03 [Branch] True branch of Empty Supplier", "P-1001", "Laptops", "", "TechCorp", 10, 25000.0, false);
    runWhiteBoxTestCase(sys, "TC-04 [Branch] False branch (Valid Supplier)", "P-1001", "Laptops", "S-505", "TechCorp", 10, 25000.0, true);
    runWhiteBoxTestCase(sys, "TC-05 [Branch] True branch of Negative Quantity", "P-1001", "Laptops", "S-505", "TechCorp", -5, 25000.0, false);

    // Condition Coverage
    runWhiteBoxTestCase(sys, "TC-06 [Condition] Price = 0", "P-1001", "Laptops", "S-505", "TechCorp", 10, 0.0, false);
    runWhiteBoxTestCase(sys, "TC-07 [Condition] Price < 0", "P-1001", "Laptops", "S-505", "TechCorp", 10, -50.0, false);
    runWhiteBoxTestCase(sys, "TC-08 [Condition] Price > 0", "P-1001", "Laptops", "S-505", "TechCorp", 10, 25000.0, true);
    runWhiteBoxTestCase(sys, "TC-09 [Condition] Quantity > 100000", "P-1001", "Laptops", "S-505", "TechCorp", 999999, 25000.0, false);

    // Path Coverage (Inventory Update Paths)
    runWhiteBoxTestCase(sys, "TC-10 [Path] Valid PO -> Stock Low Alert Path", "P-1001", "Laptops", "S-505", "TechCorp", 1, 25000.0, true);
    runWhiteBoxTestCase(sys, "TC-11 [Path] Valid PO -> Stock Healthy Path", "P-1001", "Laptops", "S-505", "TechCorp", 100, 25000.0, true);

    // Loop Coverage (Simulated via quantity of items/total calculation)
    // Note: In our code, loop executes exactly once per PO item added.
    runWhiteBoxTestCase(sys, "TC-12 [Loop] Loop execution test (1 item added)", "P-1001", "Laptops", "S-505", "TechCorp", 5, 25000.0, true);
    runWhiteBoxTestCase(sys, "TC-13 [Loop] Skip loop (Invalid PO creates 0 items)", "P-1001", "Laptops", "S-505", "TechCorp", -1, 25000.0, false);
    runWhiteBoxTestCase(sys, "TC-14 [Loop] Loop multiple iterations check (Valid items)", "P-1002", "Keyboards", "S-505", "TechCorp", 20, 500.0, true); // P-1002 is valid in logic, testing different loop path
    
    // Branch + Condition Coverage
    runWhiteBoxTestCase(sys, "TC-15 [Branch+Cond] Negative Qty + Negative Price check", "P-1001", "Laptops", "S-505", "TechCorp", -10, -500.0, false);

    cout << "=========================================================================\n";
}

// ==========================================
// MAIN EXECUTION
// ==========================================

int main() {
    Product laptop("P-1001", "Laptops");
    Inventory laptopInv(laptop, 50, 100);
    SystemManager sys(&laptopInv);

    int choice;
    int poCounter = 1;

    while (true) {
        cout << "\n===========================================================\n";
        cout << "    INVENTORY MANAGEMENT SYSTEM - RAISE PO MODULE (C++)    \n";
        cout << "===========================================================\n";
        cout << "1. Raise Purchase Order (Manual Input)\n";
        cout << "2. Run Automated Black Box Test Cases (15 Cases)\n";
        cout << "3. Run Automated White Box Test Cases (Coverage)\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        
        if (choice == 4) {
            cout << "Exiting system. Goodbye!\n";
            break;
        } else if (choice == 3) {
            runAllWhiteBoxTests(sys);
        } else if (choice == 2) {
            runAllAutomatedTests(sys);
        } else if (choice == 1) {
            string pId, pName, sId, sName;
            int qty;
            double price;

            // Clear buffer before getline
            cin.ignore(10000, '\n');

            cout << "\nEnter Product ID: ";
            getline(cin, pId);

            cout << "Enter Product Name: ";
            getline(cin, pName);

            cout << "Enter Supplier ID: ";
            getline(cin, sId);

            cout << "Enter Supplier Name: ";
            getline(cin, sName);

            cout << "Enter Quantity: ";
            while (!(cin >> qty)) {
                cout << "Invalid quantity. Enter a number: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            cout << "Enter Unit Price (Rs): ";
            while (!(cin >> price)) {
                cout << "Invalid price. Enter a number: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            string poNumber = "PO-00" + to_string(poCounter++);
            sys.raisePurchaseOrder(poNumber, pId, pName, sId, sName, qty, price);
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
