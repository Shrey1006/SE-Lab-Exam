# Inventory Management System - C++ CLI Application

This is a pure C++ Object-Oriented console application designed to handle the **Raise Purchase Order** module. It strictly enforces validation rules based on **Equivalence Class Partitioning (ECP)**, **Boundary Value Analysis (BVA)**, and **White Box Coverage** methodologies.

## Project Structure
The application uses a single comprehensive file (`main.cpp`) which encapsulates the following architecture:
- **Entities:** `Product`, `Supplier`, `Inventory`, `PurchaseOrder`, `PurchaseOrderItem`
- **Controllers:** `ValidationManager` (handles Black Box logic), `SystemManager` (orchestrates operations)

## How to Compile
You must have a C++ compiler (`g++`) installed on your system. 
Open your terminal, navigate to this folder, and run:
```bash
g++ main.cpp -o main
```

## How to Run
Execute the compiled binary:
```bash
./main
```

## Features & Menu Options

Upon running the program, you will see an interactive menu with 4 options:

### 1. Raise Purchase Order (Manual Input)
Allows you to manually enter details via the terminal (`cin`). This is useful for manual testing.
- The system will ask for Product ID, Product Name, Supplier ID, Supplier Name, Quantity, and Unit Price.
- It will validate your inputs in real-time. If you enter an invalid value (e.g., `-5` quantity or `0` price), it will cleanly reject the PO and print the exact error.

### 2. Run Automated Black Box Test Cases
Automatically executes **15 pre-configured Black Box test cases**.
- Tests ECP (Empty IDs, Valid Ranges) and BVA (0, -1, 1, extremely large bounds).
- Displays detailed logs for each case showing:
  - **Inputs:** The specific values fed into the system.
  - **Expected:** What the system *should* do.
  - **Actual:** What the system *actually* did.
  - **Status:** A `[PASS]` or `[FAIL]` badge.

### 3. Run Automated White Box Test Cases
Automatically executes **15 pre-configured White Box test cases** targeting specific logic branches in the code.
- Ensures total code coverage including:
  - **Statement Coverage**
  - **Branch Coverage**
  - **Condition Coverage**
  - **Path Coverage**
  - **Loop Coverage**
- Outputs the same detailed logging format with `[PASS]` / `[FAIL]` status for complete transparency during your practical exam or viva.

### 4. Exit
Terminates the program cleanly.
