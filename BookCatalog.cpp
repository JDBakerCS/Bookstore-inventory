#include "BookCatalog.h"
#include "SalesRecord.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <numeric>
#include <algorithm>
using namespace std;

//This file has implmentation for the BookCatalog class and 
//the helper functions for dealing with the .csv files and managing
//the catalog.

void loadBooksFromCSV(const string& filename,
                      const string& category,
                      map<string, Book>& catalog) 
{
    ifstream file(filename);
    if (!file) {
        cerr<<"Error: Could not open file: "<<filename<<endl;
        return;
    }
    string line;
    
    if (!getline(file, line)) {
        cerr<<"Error:File is empty: "<<filename<<endl;
        return;
    }
    while(getline(file, line)) {
        if (line.empty()) continue;
    
        stringstream ss(line);
        string isbnStr, titleStr, authorStr, yearStr, priceStr, quantityStr;
            //
        if (!getline(ss, isbnStr, ',')) continue;
        if (!getline(ss, titleStr, ',')) continue;
        if (!getline(ss, authorStr, ',')) continue;
        if (!getline(ss, yearStr, ',')) continue;
        if (!getline(ss, priceStr, ',')) continue;
        if (!getline(ss, quantityStr, ',')) continue;
       
    
        try {
            int year = stoi(yearStr);
            double price = stod(priceStr);
            int quantity = stoi(quantityStr);
            
            if (catalog.count(isbnStr)) {
                cout << "Duplicate ISBN detected: " << isbnStr
                     << " in file: " << filename << endl;
    }
        
            Book b(authorStr, titleStr, year, isbnStr, price, quantity, category);
        
            catalog.emplace(isbnStr, b);
        }
        catch (const exception& e) {
            cerr<<"Parse error on file "<<filename
                <<" on line: "<<line<<"\n"
                <<"Reason: "<<e.what()<<endl;
        }
    }
}

void loadAllBookFiles(map<string, Book>& catalog) {
    loadBooksFromCSV("classic-books.txt", "CLASSIC FICTION", catalog);
    loadBooksFromCSV("contemporary-books.txt", "CONTEMPORARY FICTION", catalog);
    loadBooksFromCSV("nonfiction-bestsellers.txt", "NONFICTION", catalog);
}
void printCatalog(const map<string, Book>& catalog) {
    for (const auto& [isbn, book] : catalog) {
        book.display();
    }
}
double totalInventoryValue(const map<string, Book>& catalog) {
    return std::accumulate(
        catalog.begin(),
        catalog.end(),
        0.0,
        [](double sum, const auto& pair) {
            const Book& b = pair.second;
            return sum + (b.getPrice() * b.getQuantity());
        }
    );
}

int countCategory(const map<string, Book>& catalog, const string& cat) {
    return std::count_if(
        catalog.begin(),
        catalog.end(),
        [&](const auto& p) {
            return p.second.getCategory() == cat;
        }
    );
}
int countLowStock(const map<string, Book>& catalog) {
    return std::count_if(
        catalog.begin(),
        catalog.end(),
        [](const auto& p) {
            return p.second.getQuantity() < 10;
        }
    );
}
void printCatalogForEach(const map<string, Book>& catalog) {
    std::for_each(
        catalog.begin(),
        catalog.end(),
        [](const auto& p) {
            p.second.display();
        }
    );
}
void applyWeeklySales(map<string, Book>& catalog,
                      const vector<SalesRecord>& weekly) 
{
    std::for_each(
        weekly.begin(),
        weekly.end(),
        [&](const SalesRecord& rec) {
            if (catalog.contains(rec.isbn)) {
                catalog[rec.isbn].reduceQuantity(rec.quantitySold);
            }
        }
    );
}
Book* findByAuthor(map<string, Book>& catalog, const string& author) {
    auto it = std::find_if(
        catalog.begin(),
        catalog.end(),
        [&](const auto& p) {
            return p.second.getAuthor() == author;
        }
    );
    return (it != catalog.end()) ? &it->second : nullptr;
}
vector<Book> booksByAuthor(const map<string, Book>& catalog,
                           const string& author)
{
    // First, flatten catalog into a vector<Book>
    vector<Book> allBooks;
    allBooks.reserve(catalog.size());
    for (const auto& [isbn, book] : catalog) {
        allBooks.push_back(book);
    }

    // Now use copy_if on vector<Book> → vector<Book>
    vector<Book> out;
    std::copy_if(
        allBooks.begin(),
        allBooks.end(),
        back_inserter(out),
        [&](const Book& b) {
            return b.getAuthor() == author;
        }
    );
    return out;
}

vector<Book> sortByPrice(const map<string, Book>& catalog) {
    vector<Book> v;
    for (auto& [isbn, book] : catalog) v.push_back(book);

    std::sort(v.begin(), v.end(),
        [](const Book& a, const Book& b) {
            return a.getPrice() < b.getPrice();
        }
    );
    return v;
}
Book getMostExpensive(const map<string, Book>& catalog) {
    return std::max_element(
        catalog.begin(),
        catalog.end(),
        [](const auto& a, const auto& b) {
            return a.second.getPrice() < b.second.getPrice();
        }
    )->second;
}
Book getLeastExpensive(const map<string, Book>& catalog) {
    return std::min_element(
        catalog.begin(),
        catalog.end(),
        [](const auto& a, const auto& b) {
            return a.second.getPrice() < b.second.getPrice();
        }
    )->second;
}
vector<SalesRecord> loadWeeklySales(const string& filename) {
    vector<SalesRecord> weeklySales;
    ifstream file(filename);

    if (!file) {
        cerr << "Error: Could not open weekly sales file: "
             << filename << endl;
        return weeklySales;
    }

    string line;

    // Skip header
    if (!getline(file, line)) {
        cerr << "Error: Weekly sales file empty: " << filename << endl;
        return weeklySales;
    }

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string date, isbnStr, quantityStr;

        if (!getline(ss, date, ',')) continue;
        if (!getline(ss, isbnStr, ',')) continue;
        if (!getline(ss, quantityStr, ',')) continue;

        try {
            int quantity = stoi(quantityStr);
            weeklySales.emplace_back(date, isbnStr, quantity);
        }
        catch (const exception& e) {
            cerr << "Parse error in weekly sales file: " << filename << endl;
            cerr << "Line: " << line << endl;
            cerr << "Reason: " << e.what() << endl;
        }
    }

    return weeklySales;
}
void loadMonthlySalesHistory(
    const string& filename,
    map<string, int>& unitsMap,
    map<string, double>& revenueMap
) 
{
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Could not open monthly sales history file: "
             << filename << endl;
        return;
    }

    string line;
    
    // Skip header
    if (!getline(file, line)) {
        cerr << "Error: Monthly sales file empty: " << filename << endl;
        return;
    }

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string month, yearStr, isbn, unitsStr, revenueStr;

        if (!getline(ss, month, ',')) continue;
        if (!getline(ss, yearStr, ',')) continue;
        if (!getline(ss, isbn, ',')) continue;
        if (!getline(ss, unitsStr, ',')) continue;
        if (!getline(ss, revenueStr, ',')) continue;

        try {
            int units = stoi(unitsStr);
            double revenue = stod(revenueStr);

            // Accumulate totals across months (Jan–May)
            unitsMap[isbn] += units;
            revenueMap[isbn] += revenue;
        }
        catch (const exception& e) {
            cerr << "Parse error in monthly sales history file: "
                 << filename << endl;
            cerr << "Line: " << line << endl;
            cerr << "Reason: " << e.what() << endl;
        }
    }
}
vector<DiscountRule> loadDiscountRules(const string& filename) {
    vector<DiscountRule> rules;
    ifstream file(filename);

    if (!file) {
        cerr << "Error: Could not open discount rules file: "
             << filename << endl;
        return rules;
    }

    string line;

    
    if (!getline(file, line)) {
        cerr << "Error: Discount rules file is empty: " << filename << endl;
        return rules;
    }

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string tier, minStr, maxStr, pctStr, desc;

        if (!getline(ss, tier, ',')) continue;
        if (!getline(ss, minStr, ',')) continue;
        if (!getline(ss, maxStr, ',')) continue;
        if (!getline(ss, pctStr, ',')) continue;
        if (!getline(ss, desc, ',')) continue;

        try {
            int minSales = stoi(minStr);
            int maxSales = stoi(maxStr);
            double percent = stod(pctStr);

            rules.emplace_back(tier, minSales, maxSales, percent, desc);
        }
        catch (const exception& e) {
            cerr << "Parse error in discount rules file: " << filename << endl;
            cerr << "Line: " << line << endl;
            cerr << "Reason: " << e.what() << endl;
        }
    }

    return rules;
}
vector<Book> applyDiscounts(
    const map<string, Book>& catalog,
    const map<string, int>& monthlyUnitsSold,
    const vector<DiscountRule>& rules
) {
    vector<Book> discountedBooks;
    discountedBooks.reserve(catalog.size());

    std::transform(
        catalog.begin(),
        catalog.end(),
        back_inserter(discountedBooks),
        [&](const auto& pair) {
            const auto& [isbn, book] = pair;
            Book result = book; // copy original

            // total monthly units sold for this ISBN
            int units = 0;
            auto uIt = monthlyUnitsSold.find(isbn);
            if (uIt != monthlyUnitsSold.end()) {
                units = uIt->second;
            }

            // find matching discount rule
            auto rIt = std::find_if(
                rules.begin(),
                rules.end(),
                [&](const DiscountRule& r) {
                    return units >= r.minSales && units <= r.maxSales;
                }
            );

            if (rIt != rules.end()) {
                double pct = rIt->percent;  // 👈 field from DiscountRule.h
                double newPrice = result.getPrice() * (1.0 - pct / 100.0);
                result.setDiscountedPrice(newPrice);
            } else {
                // no rule found: no discount
                result.setDiscountedPrice(result.getPrice());
            }

            return result;
        }
    );

    return discountedBooks;
}

Book getMostExpensiveBook(const map<string, Book>& catalog) {
    auto it = std::max_element(
        catalog.begin(),
        catalog.end(),
        [](const auto& a, const auto& b) {
            return a.second.getPrice() < b.second.getPrice();
        }
    );

    return it->second;
}
Book getLeastExpensiveBook(const map<string, Book>& catalog) {
    auto it = std::min_element(
        catalog.begin(),
        catalog.end(),
        [](const auto& a, const auto& b) {
            return a.second.getPrice() < b.second.getPrice();
        }
    );

    return it->second;
}
Book getBestSeller(const map<string, Book>& catalog,
    const map<string, int>& monthlyUnitsSold) {
    auto it = std::max_element(
        monthlyUnitsSold.begin(),
        monthlyUnitsSold.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;  // compare units sold
        }
    );

    // it->first is ISBN
    string isbn = it->first;

    if (!catalog.contains(isbn)) {
        throw runtime_error("Bestseller ISBN not found in catalog.");
    }

    return catalog.at(isbn);
}
Book getSlowestSeller(const map<string, Book>& catalog,
                      const map<string, int>& monthlyUnitsSold) {
    if (monthlyUnitsSold.empty()) {
        throw runtime_error("No monthly sales data available for slowest seller.");
    }

    // Find ISBN with the smallest units
    auto it = std::min_element(
        monthlyUnitsSold.begin(),
        monthlyUnitsSold.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;  // compare units sold
        }
    );

    const string& isbn = it->first;

    auto cIt = catalog.find(isbn);
    if (cIt == catalog.end()) {
        throw runtime_error("Slowest seller ISBN not found in catalog.");
    }

    return cIt->second;
}

vector<Book> sortByTitle(const map<string, Book>& catalog) {
    vector<Book> v;
    v.reserve(catalog.size());

    for (const auto& [isbn, book] : catalog) {
        v.push_back(book);
    }

    std::sort(v.begin(), v.end(),
              [](const Book& a, const Book& b) {
                  return a.getTitle() < b.getTitle();
              });

    return v;
}
vector<Book> sortByYear(const map<string, Book>& catalog) {
    vector<Book> v;
    v.reserve(catalog.size());

    for (const auto& [isbn, book] : catalog) {
        v.push_back(book);
    }

    std::sort(v.begin(), v.end(),
              [](const Book& a, const Book& b) {
                  return a.getYear() < b.getYear();
              });

    return v;
}
vector<Book> getLowStockBooks(const map<string, Book>& catalog) {
    vector<Book> low;
    low.reserve(catalog.size());

    for (const auto& [isbn, book] : catalog) {
        if (book.getQuantity() < 10) {
            low.push_back(book);
        }
    }

    return low;
}
