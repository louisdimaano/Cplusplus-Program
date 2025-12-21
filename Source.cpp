/*
 * Personal Expense Tracker
 * ------------------------
 * A simple C++ console application for tracking personal expenses.
 * - Add new expenses (amount, date, category, description)
 * - View all expenses
 * - View total spending
 * - View total spending by category
 * - Data is persisted in a text file: expenses.dat
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <limits>

using namespace std;

// Represents a single expense record
class Expense {
public:
    Expense() : amount(0.0) {}

    Expense(double amount,
        const string& date,
        const string& category,
        const string& description)
        : amount(amount),
        date(date),
        category(category),
        description(description) {
    }

    double GetAmount() const { return amount; }
    const string& GetDate() const { return date; }
    const string& GetCategory() const { return category; }
    const string& GetDescription() const { return description; }

    // Serialize to a single line using '|' as a separator
    string ToLine() const {
        ostringstream out;
        out << fixed << setprecision(2) << amount
            << "|" << date
            << "|" << category
            << "|" << description;
        return out.str();
    }

    // Parse an Expense from a line in the same format as ToLine()
    static bool FromLine(const string& line, Expense& result) {
        istringstream in(line);
        string amountStr;
        string dateStr;
        string categoryStr;
        string descriptionStr;

        if (!getline(in, amountStr, '|')) return false;
        if (!getline(in, dateStr, '|')) return false;
        if (!getline(in, categoryStr, '|')) return false;
        if (!getline(in, descriptionStr)) return false;

        try {
            double amt = stod(amountStr);
            result = Expense(amt, dateStr, categoryStr, descriptionStr);
            return true;
        }
        catch (const exception&) {
            return false;
        }
    }

private:
    double amount;
    string date;
    string category;
    string description;
};

// Manages the list of expenses and the user interaction
class ExpenseTracker {
public:
    explicit ExpenseTracker(const string& dataFileName)
        : dataFileName(dataFileName) {
        LoadFromFile();
    }

    // Main loop
    void Run() {
        bool running = true;

        while (running) {
            PrintMenu();
            int choice = GetMenuChoice();

            switch (choice) {
            case 1:
                AddExpenseInteractive();
                break;
            case 2:
                PrintAllExpenses();
                break;
            case 3:
                PrintTotalSpent();
                break;
            case 4:
                PrintTotalByCategory();
                break;
            case 5:
                SaveToFile();
                cout << "Data saved. Exiting..." << endl;
                running = false;
                break;
            default:
                // Should not happen due to validation
                break;
            }
        }
    }

private:
    string dataFileName;
    vector<Expense> expenses;

    // Load all expenses from the data file
    void LoadFromFile() {
        ifstream inFile(dataFileName);

        if (!inFile.is_open()) {
            // File may not exist yet; start with empty list
            cout << "No existing data file found. Starting with an empty expense list." << endl;
            return;
        }

        string line;
        int loadedCount = 0;
        while (getline(inFile, line)) {
            if (line.empty()) {
                continue;
            }
            Expense exp;
            if (Expense::FromLine(line, exp)) {
                expenses.push_back(exp);
                ++loadedCount;
            }
        }

        inFile.close();
        cout << "Loaded " << loadedCount << " expenses from " << dataFileName << "." << endl;
    }

    // Save all expenses to the data file
    void SaveToFile() const {
        ofstream outFile(dataFileName, ios::trunc);
        if (!outFile.is_open()) {
            cerr << "Error: Could not open " << dataFileName << " for writing." << endl;
            return;
        }

        for (const auto& exp : expenses) {
            outFile << exp.ToLine() << '\n';
        }

        outFile.close();
    }

    // Display the main menu
    void PrintMenu() const {
        cout << "\n========== Personal Expense Tracker ==========\n";
        cout << "1. Add a new expense\n";
        cout << "2. View all expenses\n";
        cout << "3. View total amount spent\n";
        cout << "4. View total spent by category\n";
        cout << "5. Save and exit\n";
    }

    // Get a valid menu choice from the user
    int GetMenuChoice() const {
        int choice;
        cout << "Enter your choice (1-5): ";

        while (true) {
            if (cin >> choice && choice >= 1 && choice <= 5) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return choice;
            }

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter a number between 1 and 5: ";
        }
    }

    // Prompt user for expense details and add to the list
    void AddExpenseInteractive() {
        double amount;
        string date;
        string category;
        string description;

        cout << "\n--- Add New Expense ---\n";
        cout << "Amount (e.g., 12.50): ";
        while (!(cin >> amount) || amount < 0.0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid amount. Enter a non-negative number: ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear line

        cout << "Date (e.g., 2025-12-19): ";
        getline(cin, date);

        cout << "Category (e.g., Groceries, Rent, Entertainment): ";
        getline(cin, category);

        cout << "Description: ";
        getline(cin, description);

        Expense exp(amount, date, category, description);
        expenses.push_back(exp);

        cout << "Expense added.\n";
    }

    // Print all recorded expenses in a table-like format
    void PrintAllExpenses() const {
        cout << "\n--- All Expenses ---\n";

        if (expenses.empty()) {
            cout << "No expenses recorded yet.\n";
            return;
        }

        cout << left << setw(12) << "Amount"
            << setw(12) << "Date"
            << setw(16) << "Category"
            << "Description" << '\n';
        cout << string(60, '-') << '\n';

        cout << fixed << setprecision(2);
        for (const auto& exp : expenses) {
            cout << left << setw(12) << exp.GetAmount()
                << setw(12) << exp.GetDate()
                << setw(16) << exp.GetCategory()
                << exp.GetDescription() << '\n';
        }
    }

    // Print the total amount spent
    void PrintTotalSpent() const {
        double total = 0.0;
        for (const auto& exp : expenses) {
            total += exp.GetAmount();
        }

        cout << "\n--- Total Amount Spent ---\n";
        cout << "Total: $" << fixed << setprecision(2) << total << '\n';
    }

    // Print totals by category
    void PrintTotalByCategory() const {
        cout << "\n--- Total Spent by Category ---\n";

        if (expenses.empty()) {
            cout << "No expenses recorded yet.\n";
            return;
        }

        map<string, double> categoryTotals;
        for (const auto& exp : expenses) {
            categoryTotals[exp.GetCategory()] += exp.GetAmount();
        }

        cout << left << setw(16) << "Category"
            << "Total" << '\n';
        cout << string(30, '-') << '\n';

        cout << fixed << setprecision(2);
        for (const auto& pair : categoryTotals) {
            cout << left << setw(16) << pair.first
                << "$" << pair.second << '\n';
        }
    }
};

int main() {
    const string DATA_FILE = "expenses.dat";

    ExpenseTracker tracker(DATA_FILE);
    tracker.Run();

    return 0;
}
