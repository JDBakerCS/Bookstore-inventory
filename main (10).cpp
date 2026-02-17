#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "BookCatalog.h"


using namespace std;

int main() {
    try {
        // Core data structures 
        map<string, Book> catalog;
        vector<SalesRecord> weeklySales;
        map<string, int> monthlyUnits;
        map<string, double> monthlyRevenue;
        vector<DiscountRule> discountRules;

        //  Initial load of all files 
        loadAllBookFiles(catalog);
        weeklySales = loadWeeklySales("sales-data-weekly.txt");
        loadMonthlySalesHistory("monthly-sales-history.txt",
                                monthlyUnits, monthlyRevenue);
        discountRules = loadDiscountRules("discount-rules.txt");

        cout << "Books loaded: " << catalog.size() << endl;
        cout << "Weekly sales records: " << weeklySales.size() << endl;
        cout << "Monthly sales entries: " << monthlyUnits.size() << endl;
        cout << "Discount rules: " << discountRules.size() << endl;

        int choice = 0;

        while (choice != 14) {
            cout << "\n===== Bookstore Analytics System =====\n";
            cout << "1. Display Inventory Statistics\n";
            cout << "2. Find Book by ISBN\n";
            cout << "3. Find Books by Author\n";
            cout << "4. Display Books Sorted by Price\n";
            cout << "5. Display Books Sorted by Title\n";
            cout << "6. Display Books Sorted by Year\n";
            cout << "7. Display Sales Analysis (Best + Slowest Seller)\n";
            cout << "8. Show Low Stock Alert (< 10 units)\n";
            cout << "9. Apply Sales-Based Discounts\n";
            cout << "10. Show Bestseller Report\n";
            cout << "11. Show Slow Seller Report\n";
            cout << "12. Update Stock From Weekly Sales\n";
            cout << "13. Reload Data Files\n";
            cout << "14. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            // clear leftover newline so getline works
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
            case 1: { // Inventory statistics
                cout << "\n--- Inventory Statistics ---\n";
                cout << "Total catalog entries: " << catalog.size() << "\n";

                int totalUnits = 0;
                for (const auto& [isbn, b] : catalog) {
                    totalUnits += b.getQuantity();
                }
                cout << "Total units in stock: " << totalUnits << "\n";

                double totalValue = totalInventoryValue(catalog);
                cout << "Total inventory value: $" << totalValue << "\n";
                break;
            }
            case 2: { // Find by ISBN
                string isbn;
                cout << "Enter ISBN: ";
                getline(cin, isbn);

                if (catalog.contains(isbn)) {
                    catalog[isbn].display();
                } else {
                    cout << "Book not found.\n";
                }
                break;
            }
            case 3: { // Find by author
                string author;
                cout << "Enter author name: ";
                getline(cin, author);

                auto byAuthor = booksByAuthor(catalog, author);
                if (byAuthor.empty()) {
                    cout << "No books found for that author.\n";
                } else {
                    cout << "\nBooks by " << author << ":\n";
                    for (const auto& b : byAuthor) b.display();
                }
                break;
            }
            case 4: { // Sort by price
                auto sorted = sortByPrice(catalog);
                cout << "\n--- Books Sorted by Price ---\n";
                for (const auto& b : sorted) b.display();
                break;
            }
            case 5: { // Sort by title
                auto sorted = sortByTitle(catalog);
                cout << "\n--- Books Sorted by Title ---\n";
                for (const auto& b : sorted) b.display();
                break;
            }
            case 6: { // Sort by year
                auto sorted = sortByYear(catalog);
                cout << "\n--- Books Sorted by Year ---\n";
                for (const auto& b : sorted) b.display();
                break;
            }
            case 7: { // Best + slowest seller
                cout << "\n--- Sales Analysis ---\n";
                auto best = getBestSeller(catalog, monthlyUnits);
                auto slow = getSlowestSeller(catalog, monthlyUnits);

                cout << "Bestseller: " << best.getTitle()
                     << " (" << monthlyUnits[best.getISBN()] << " units)\n";
                cout << "Slowest seller: " << slow.getTitle()
                     << " (" << monthlyUnits[slow.getISBN()] << " units)\n";
                break;
            }
            case 8: { // Low stock alert
                cout << "\n--- Low Stock Alert (< 10 copies) ---\n";
                auto low = getLowStockBooks(catalog);
                cout << "Low stock books: " << low.size() << "\n";
                for (const auto& b : low) b.display();
                break;
            }
            case 9: { // Apply discounts
                cout << "\n--- Discounted Prices (based on monthly sales) ---\n";
                auto discounted = applyDiscounts(catalog, monthlyUnits, discountRules);
                for (const auto& b : discounted) {
                    cout << b.getTitle()
                         << " | Original: $" << b.getPrice()
                         << " | Discounted: $" << b.getDiscountedPrice() << "\n";
                }
                break;
            }
            case 10: { // Bestseller report
                auto best = getBestSeller(catalog, monthlyUnits);
                cout << "\n--- Bestseller Report ---\n";
                cout << best.getTitle()
                     << " (" << monthlyUnits[best.getISBN()] << " units)\n";
                break;
            }
            case 11: { // Slow seller report
                auto slow = getSlowestSeller(catalog, monthlyUnits);
                cout << "\n--- Slow Seller Report ---\n";
                cout << slow.getTitle()
                     << " (" << monthlyUnits[slow.getISBN()] << " units)\n";
                break;
            }
            case 12: { // Apply weekly sales to stock
                applyWeeklySales(catalog, weeklySales);
                cout << "Weekly sales applied to inventory quantities.\n";
                break;
            }
            case 13: { // Reload files
                catalog.clear();
                monthlyUnits.clear();
                monthlyRevenue.clear();
                weeklySales.clear();
                discountRules.clear();

                loadAllBookFiles(catalog);
                weeklySales = loadWeeklySales("sales-data-weekly.txt");
                loadMonthlySalesHistory("monthly-sales-history.txt",
                                        monthlyUnits, monthlyRevenue);
                discountRules = loadDiscountRules("discount-rules.txt");
                cout << "All data files reloaded.\n";
                break;
            }
            case 14:
                cout << "Goodbye!\n";
                break;

            default:
                cout << "Invalid choice.\n";
                break;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
    }

    return 0;
}
